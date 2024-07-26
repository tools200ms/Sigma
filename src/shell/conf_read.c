

//  https://github.com/benhoyt/inih

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <ini.h>

#include "conf_read.h"
#include "../lib/messages.h"

#define MAX_NAME_LEN 64
#define MAX_VALUE_LEN 512


// Simple match function (case insensitive), supporting
// only ASCII
static short label_match( const char *label, const char *dict[] ) {
    char l_ch, de_ch; // dict entry
    short de_last_idx = -1;
    short mark[32];
    bool multi_match = false;

    for( int idx = 0; 1 ; ++idx ) {
        if( dict[ idx ] == NULL ) {
            mark[ idx ] = 0;
            break;
        }

        mark[ idx ] = 1;
    }

    for(int i_l = 0; label[i_l] != '\0'; i_l++){
        l_ch = tolower( label[i_l] );

        de_last_idx = -1;
        multi_match = false;

        for( int i_d = 0; mark[ i_d ] != 0; ++i_d ) {

            if( mark[ i_d ] == -1 ) {
                // entry does not match
                continue;
            }

            de_ch = *(dict[ i_d ] + i_l);

            if( de_ch == '\0' || de_ch != l_ch ) {
                mark[ i_d ] = -1;
                // mark entry as not matched
                continue;
            }

            if( de_last_idx != -1 ) {
                multi_match = true;
            }
            de_last_idx = i_d;
        }
    }

    if( multi_match ) {
        MSG_DEBUG_( "Multiple matches for '%s' label", label );
    }

    return de_last_idx;
}

enum {  CONF_READ_READY, // Ready to read atomic data set
        CONF_READ_WRONGSYNTAX, // Wrong syntax, can't interpret parameters
        CONF_READ_WRONGSTRUCTURE, // Wrong structure, other instruction has been expected
        CONF_READ_EXPECT_NEXT, // Remaining atomic data set in a next instruction/s
        CONF_READ_OK};  // Atomic data set has been read

static int _parse_relay( const char *value, void *conf ) {

    regex_t re;
    regmatch_t re_mat[CONFIG_MAX_PARAM_CNT], *m_idx = NULL;
    int mi_start, mi_end;
    int re_res;
    char numb_buf[CONFIG_MAX_NUMBER_LEN];

    struct config_devices* d_conf = (struct config_devices*) conf;

    re_res = regcomp(&re, "^(nopen|nclose)(([[:space:]]*[0-9]{1,3})+)$", REG_EXTENDED);

    if( re_res ) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    re_res = regexec(&re, value, CONFIG_MAX_PARAM_CNT, re_mat, 0);

    if( re_res == REG_NOMATCH ) {
        MSG_WARN( "Invalid 'relay_pins' format, expected: (nopen|nclose) <chanel 1 pin no.> <chanel 2 pin no.> ..." );
        return CONF_READ_WRONGSYNTAX;
    } else if( re_res != 0 ) {
        MSG_ERROR( "Error while matching pattern" );
        exit( 1 );
    }

    // match (and validation) OK, safe to copy parameters

    m_idx = &(re_mat[1]);
    mi_start = m_idx->rm_so;
    mi_end = m_idx->rm_eo;
    sprintf( numb_buf, "%.*s", (mi_end - mi_start), value + mi_start );

    if( strcmp(numb_buf, "nopen") == 0 ) {
        d_conf->relay_mode = DEV_RELAY_MODE_NORMALLY_OPEN;
    } else {
        d_conf->relay_mode = DEV_RELAY_MODE_NORMALLY_CLOSE;
    }

    m_idx = &(re_mat[2]);
    mi_start = m_idx->rm_so;
    mi_end = m_idx->rm_eo;

    int n_begin = -1;

    int *pin = &(d_conf->relay_pins[0]);

    for( int idx = mi_start; idx <= mi_end; ++idx ) {
        if( idx == mi_end || value[idx] == ' ' || value[idx] == '\t' ) {

            if( n_begin != -1 ) {
                sprintf( numb_buf, "%.*s", idx - n_begin, value + n_begin );
                n_begin = -1;
                *(pin++) = atoi(numb_buf);
                MSG_DEBUG_("    Found: %d", *(pin - 1) );
            }

            continue;
        }

        if( n_begin == -1 ) {
            n_begin = idx;
        }
    }

    *pin = -1;

    MSG_DEBUG( "Relay configuration added" );

    return CONF_READ_OK;
}

static int _parse_temp( const char *value, void *conf ) {

    regex_t re;
    int re_res;
    struct config_devices* d_conf = (struct config_devices*) conf;

    re_res = regcomp(&re, "^[0-9A-Fa-f]{12}$", REG_EXTENDED);
    if( re_res ) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    re_res = regexec(&re, value, 0, NULL, 0);

    if( re_res == REG_NOMATCH ) {
        MSG_WARN_( "Invalid 1wire address: %s", value );
        return CONF_READ_WRONGSYNTAX;
    } else if( re_res != 0 ) {
        MSG_ERROR( "Error while matching pattern" );
        exit( 1 );
    }

    MSG_DEBUG_( "1Wire sensor address added: %s", value );
    strcpy( d_conf->temp, value );

    return CONF_READ_OK;
}

static const char *SSIGMA_LIST[] = {
    "relay_pins", "temp1w", NULL
};

int (*SSIGMA_LIST_FUN[])( const char *, void * ) = {_parse_relay, _parse_temp};


static int cur_status = CONF_READ_READY;
static int logic_rule_no = 0;

static int _parse_logic_limit( const char *value, float *numb1, float *numb2 ) {

    regex_t re;
    regmatch_t re_mat[CONFIG_MAX_PARAM_CNT], *m_idx = NULL;
    int mi_start, mi_end, mi_dot_idx;
    int re_res;

    if( cur_status != CONF_READ_READY ) {
        return CONF_READ_WRONGSTRUCTURE;
    }

    re_res = regcomp(&re, "^([+-]?[0-9]{1,3})([\\.,][0-9]{1,4})?[[:space:]]*([+-]?[0-9]{1,3})([\\.,][0-9]{1,4})?$", REG_EXTENDED);

    if( re_res ) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    re_res = regexec(&re, value, CONFIG_MAX_PARAM_CNT, re_mat, 0);

    if( re_res == REG_NOMATCH ) {
        MSG_WARN( "Invalid '*_limit_temp' format, expected: numb1 numb2" );
        return CONF_READ_WRONGSYNTAX;
    } else if( re_res != 0 ) {
        MSG_ERROR( "Error while matching pattern" );
        return CONF_READ_WRONGSYNTAX;
    }

    // match (and validation) OK, safe to copy parameters

    char numb_buf[CONFIG_MAX_NUMBER_LEN];

    m_idx = &(re_mat[1]); // Total part
    mi_start = m_idx->rm_so;

    m_idx = &(re_mat[2]); // fractional part
    mi_dot_idx = m_idx->rm_so; // dot/comma here
    mi_end = m_idx->rm_eo;
    sprintf( numb_buf, "%.*s", (mi_end - mi_start), value + mi_start );
    if( numb_buf[mi_dot_idx - mi_start] == ',' ) {
        // change to dot for atof()
        numb_buf[mi_dot_idx - mi_start] = '.';
    }

    *numb1 = atof( numb_buf );

    m_idx = &(re_mat[3]); // Total part
    mi_start = m_idx->rm_so;

    m_idx = &(re_mat[4]); // fractional part
    mi_dot_idx = m_idx->rm_so; // dot/comma here
    mi_end = m_idx->rm_eo;
    sprintf( numb_buf, "%.*s", (mi_end - mi_start), value + mi_start );

    if( numb_buf[mi_dot_idx - mi_start] == ',' ) {
        // change to dot for atof()
        numb_buf[mi_dot_idx - mi_start] = '.';
    }

    *numb2 = atof( numb_buf );

    return CONF_READ_EXPECT_NEXT;
}


static int _parse_logic_up_limit( const char *value, void *conf ) {

    int ret;
    struct config_logic* l_conf = (struct config_logic*) conf;

    if( (ret = _parse_logic_limit( value, &(l_conf->max), &(l_conf->min) )) != CONF_READ_EXPECT_NEXT ) {
        return ret;
    }

    if( (l_conf->max - l_conf->min) < 0.1 ) {
        MSG_WARN("Max. temp. should exceed Min.")
    }

    MSG_DEBUG_( "Logic UPPER_LIMIT cond. found: %.2f, %.2f", l_conf->max, l_conf->min );

    l_conf->limit_type = UPPER_LIMIT;

    return CONF_READ_EXPECT_NEXT;
}

static int _parse_logic_low_limit( const char *value, void *conf ) {

    int ret;
    struct config_logic* l_conf = (struct config_logic*) conf;


    if( (ret = _parse_logic_limit( value, &(l_conf->min), &(l_conf->max) )) != CONF_READ_EXPECT_NEXT ) {
        return ret;
    }

    if( (l_conf->max - l_conf->min) < 0.1 ) {
        MSG_WARN("Max. temp. should exceed Min.")
    }

    MSG_DEBUG_( "Logic LOW_LIMIT cond. found: %.2f, %.2f", l_conf->min, l_conf->max );

    l_conf->limit_type = LOW_LIMIT;

    return CONF_READ_EXPECT_NEXT;
}

static int _parse_logic_action( const char *value, void *conf ) {

    regex_t re;
    regmatch_t re_mat[CONFIG_MAX_PARAM_CNT], *m_idx = NULL;
    int mi_start, mi_end;
    int re_res;

    struct config_logic* l_conf = (struct config_logic*) conf;

    if( cur_status != CONF_READ_EXPECT_NEXT ) {
        return CONF_READ_WRONGSTRUCTURE;
    }

    re_res = regcomp(&re, "^relay[[:space:]]+ch([0-9]{1,2})[[:space:]]+open$", REG_EXTENDED);

    if( re_res ) {
        fprintf(stderr, "Could not compile regex\n");
        exit( 1 );
    }

    re_res = regexec(&re, value, CONFIG_MAX_PARAM_CNT, re_mat, 0);

    if( re_res == REG_NOMATCH ) {
        MSG_WARN( "Invalid 'action' format, expected: relay ch<no.> open" );
        return CONF_READ_WRONGSYNTAX;
    } else if( re_res != 0 ) {
        MSG_ERROR( "Error while matching pattern" );
        return CONF_READ_WRONGSYNTAX;
    }

    // match (and validation) OK, safe to copy parameters

    char numb_buf[CONFIG_MAX_NUMBER_LEN];

    m_idx = &(re_mat[1]);
    mi_start = m_idx->rm_so;
    mi_end = m_idx->rm_eo;
    sprintf( numb_buf, "%.*s", (mi_end - mi_start), value + mi_start );

    l_conf->relay_ch = atof( numb_buf );

    return CONF_READ_OK;
}


static const char *SLOGIC_LIST[] = {
    "up_limit_temp", "low_limit_temp", "action_temp", NULL
};

int (*SLOGIC_LIST_FUN[])( const char *, void * ) = {_parse_logic_up_limit, _parse_logic_low_limit, _parse_logic_action};


static int _load_settings( const char *name, const char *value, void *conf ) {

    short idx;
    MSG_DEBUG_( "Loading Settings: %s", name );

    if( ( idx = label_match( name, SSIGMA_LIST ) ) < 0 ) {
        MSG_WARN_( "Unknown INI param.: '%s'", name );

        return 1;
    }

    cur_status = (SSIGMA_LIST_FUN[idx])(value, &(((Config *)conf)->dev));

    if( CONF_READ_OK == cur_status ) {
        cur_status = CONF_READ_READY;
    }

    return cur_status;
}

static int _load_logic( const char *name, const char *value, void *conf ) {

    short idx;
    MSG_DEBUG_( "Loading Logic: %s", name );

    if( ( idx = label_match( name, SLOGIC_LIST ) ) < 0 ) {
        MSG_WARN_( "Unknown INI param.: '%s'", name );

        return 1;
    }

    cur_status = (SLOGIC_LIST_FUN[idx])(value, &(((Config *)conf)->logic[logic_rule_no]));

    if( cur_status == CONF_READ_OK ) {
        ++logic_rule_no;
        cur_status = CONF_READ_READY;
    }

    return cur_status;
}


static const char *SECT_LIST[] = {
    "sigma", "logic", NULL
};

int (*SECT_LIST_FUN[])( const char *, const char *, void * ) = {_load_settings, _load_logic};


static int ini_parser_sigma_handler( void* user, const char* section, const char* name, const char* value ) {

    Config* conf = (Config*)user;

    short sect_idx;
    int param_parse_res;

    MSG_DEBUG_( "INI file, parsing: %s: %s=%s", section, name, value );

    if( ( sect_idx = label_match( section, SECT_LIST ) ) < 0 ) {
        MSG_WARN_( "Unknown INI section: '%s'", section );

        return 1;
    }

    param_parse_res = (SECT_LIST_FUN[sect_idx])( name, value, conf );

    if( param_parse_res != CONF_READ_READY ) {
        MSG_ERROR( "Errors while loading file" );
    }

    return 1;
}

Config *read_conf( const char *conf_file ) {
    Config *conf;

    if( (conf = malloc( sizeof(Config) )) == NULL ) {
        MSG_ERROR_MALLOC();
    }

    memset( conf, 0, sizeof(Config) );

    // internalize logic rule counter to '0'
    logic_rule_no = 0;
    if( ini_parse( conf_file, ini_parser_sigma_handler, conf ) < 0 ) {
        MSG_ERROR_( "Can't load config file: '%s', does file exists?", conf_file );
        return NULL;
    }

    return conf;
}


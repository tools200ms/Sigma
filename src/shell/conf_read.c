

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

static int _parse_relay( const char *value, void *dev_conf ) {

    regex_t regex;
    regmatch_t pmatch[64];
    int rret;
    struct config_devices* d_conf = (struct config_devices*) dev_conf;

    rret = regcomp(&regex, "^(nopen|nclose)(([[:space:]]*[0-9][0-9]?)+)$", REG_EXTENDED);

    if( rret ) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    rret = regexec(&regex, value, 64, pmatch, 0);

    if( rret == REG_NOMATCH ) {
        MSG_WARN_( "Invalid 1wire address: |%s|", value );
        return 1;
    } else if( rret != 0 ) {
        MSG_ERROR( "Error while maching pattern" );
        return 1;
    }

    for( int i = 0; i < regex.re_nsub; ++i ) {
        int start = pmatch[i].rm_so;
        int finish = pmatch[i].rm_eo;

        printf("Storing:  |%.*s|\n", (finish - start), value + start);
    }

    regmatch_t *m;
    int m_start, m_end;

    m = &(pmatch[1]);
    m_start = m->rm_so;
    m_end = m->rm_eo;
    sprintf( d_conf->relay_mode, "%.*s", (m_end - m_start), value + m_start );

    m = &(pmatch[2]);
    m_start = m->rm_so;
    m_end = m->rm_eo;

    int n_begin = -1;
    char numb_buf[8];

    for( int idx = m_start; idx <= m_end; ++idx ) {
        if( idx == m_end || value[idx] == ' ' || value[idx] == '\t' ) {

            if( n_begin != -1 ) {
                sprintf( numb_buf, "%.*s", idx - n_begin, value + n_begin );
                n_begin = -1;
                MSG_DEBUG_("    Found: %d", atoi(numb_buf) );
            }

            continue;
        }

        if( n_begin == -1 ) {
            n_begin = idx;
        }
    }

    MSG_DEBUG_( "Relay configuration added: %s", d_conf->relay_mode );
}

static int _parse_temp( const char *value, void *dev_conf ) {

    regex_t regex;
    int rret;
    struct config_devices* d_conf = (struct config_devices*) dev_conf;

    rret = regcomp(&regex, "^[0-9A-Fa-f]{12}$", REG_EXTENDED);
    if( rret ) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    rret = regexec(&regex, value, 0, NULL, 0);

    if( rret == REG_NOMATCH ) {
        MSG_WARN_( "Invalid 1wire address: |%s|", value );
        return 1;
    } else if( rret != 0 ) {
        MSG_ERROR( "Error while maching pattern" );
        return 1;
    }

    MSG_DEBUG_( "1Wire sensor address added: %s", value );
    strcpy( d_conf->temp, value );
    return 0;
}

static char *SSIGMA_LIST[] = {
    "relay_pins", "temp1w", NULL
};

int (*SSIGMA_LIST_FUN[3])( const char *, void * ) = {_parse_relay, _parse_temp, NULL};


static char *SLOGIC_LIST[] = {
    "up_limit_temp", "low_limit_temp", "action_temp", NULL
};


static int _load_settings( const char *name, const char *value, void *conf ) {

    short idx;
    MSG_DEBUG_( "Loading Settings: %s", name );

    if( ( idx = label_match( name, SSIGMA_LIST ) ) < 0 ) {
        MSG_WARN_( "Unknown INI param.: '%s'", name );

        return 1;
    }


    (SSIGMA_LIST_FUN[idx])(value, &(((Config *)conf)->dev));

    return 0;
}

static int _load_logic( const char *name, const char *value, void *conf ) {

    short idx;
    MSG_DEBUG_( "Loading Logic: %s", name );

    if( ( idx = label_match( name, SLOGIC_LIST ) ) < 0 ) {
        MSG_WARN_( "Unknown INI param.: '%s'", name );

        return 1;
    }

    return 0;
}


static char *SECT_LIST[] = {
    "sigma", "logic", NULL
};

int (*SECT_LIST_FUN[3])( const char *, const char *, void * ) = {_load_settings, _load_logic, NULL};


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

    return 1;
}

Config *read_conf( const char *conf_file ) {
    Config *conf;

    if( (conf = malloc( sizeof(Config) )) == NULL ) {
        MSG_ERROR_MALLOC();
    }

    if( ini_parse( conf_file, ini_parser_sigma_handler, conf ) < 0 ) {
        MSG_ERROR_( "Can't load config file: '%s', does file exists?", conf_file );
        return NULL;
    }

    return conf;
}




//  https://github.com/benhoyt/inih

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ini.h>

#include "conf_read.h"
#include "../lib/messages.h"

#define MAX_NAME_LEN 64
#define MAX_VALUE_LEN 512


static int ini_parser_sigma_handler( void* user, const char* section, const char* name, const char* value ) {

    MSG_DEBUG_( "INI", "parsing: %s: %s=%s", section, name, value );

    if( strcmp( section, "sigma" ) == 0 ) {

    }
    else if( strcmp( section, "logic" ) == 0 ) {

    }
    else {
        MSG_WARN_("Unknown section: %s", section);
    }



    return 1;
}

Config *read_conf( const char *conf_file, const char *options ) {
    Config *conf;

    if( (conf = malloc( sizeof(Config) )) == NULL ) {
        MSG_ERROR_MALLOC();
    }

    if( ini_parse( conf_file, ini_parser_sigma_handler, conf ) < 0) {
        MSG_ERROR_( "Can't load config file: %s", conf_file );
        return NULL;
    }

    return conf;
}


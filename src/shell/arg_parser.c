
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "arg_parser.h"

#include "../lib/messages.h"
#include "../lib/translations.h"

Config *parse_arguments( int argc, char *argv[] ) {
    int opt;
    int option_index = 0;
    Config *conf;
    char *config_path = SIGMA_DEFAULT_CONFIG_FILE_PATH;

    static struct option sigma_options[] = {
        {"config", required_argument, NULL, 'c'},
        {"silent", no_argument, NULL, 's'},
        {"verbose", no_argument, NULL, 'v'},
        {"debug", no_argument, NULL, 'd'},
        {"version", no_argument, NULL, 'V'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "c:svdVh", sigma_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': // '--config' option
                config_path = optarg;
            break;
            case 's': // '--silent' option
                msgSetSilentMode();
            break;
            case 'v': // '--verbose' option
                msgSetVerboseMode();
                MSG_VERBOSE( STR_VERBOSE_MODE_SET_ON );
            break;
            case 'd': // '--debug' option
                #ifdef DEBUG
                    msgSetDebuggingMode();
                    MSG_INFO( STR_DEBUG_MSG_ON );
                #else
                    MSG_INFO( STR_DEBUG_MSG_ON_NOFLAG );
                #endif
            break;
            case 'V': // '--version' option
                PRINT_SIGMA_VERSION( stdout );
                exit( 0 );
            break;
            case 'h': // '--help' option
                PRINT_SIGMA_HELP( stdout, argv[0] );
                exit( 0 );
            break;
            default:
                MSG_ERROR( STR_WRONG_SYNTAX );
                PRINT_SIGMA_HELP( stderr, argv[0] );
                exit( EXIT_FAILURE );
        }
    }

    conf = malloc( sizeof(Config) );
    // zero
    // memset (&spi, 0, sizeof (spi)) ;
    conf->config_file = strdup( config_path );

    return conf;
}

#ifndef _SIGMA_200MS_NET__LIB_TRANSLATIONS_EN_H_
#define _SIGMA_200MS_NET__LIB_TRANSLATIONS_EN_H_

/**
	definitions of message strings, all messages in this file should be in English,
	in the case of translation appropriate file should be created, e.g. messages_pl.h
	containing translated strings.
*/

/**
	configure messages:
*/

#define STR_SIGMA_SLOGAN 		"IoT control and monitoring by your rules"

#define STRTMPL_SIGMA_HELP 		"options:\n"\
								"    --conf,-c    - path to config file (default '%s')\n"\
								"    --silent,-s  - don't print any messages\n"\
								"    --verbose,-v - print verbose messages\n"\
								"    --debug,-d   - print debug messages (if compiled with 'DEBUG' flag)\n"\
								"    -V   --version - print version and exit\n\n"\
								"    -h   --help    - print this help exit\n"

// Argument parser messages:
#define STR_WRONG_SYNTAX 		"Wrong syntax\n"
#define STR_VERBOSE_MODE_SET_ON "Verbose mode is set ON"

#define STR_DEBUG_MSG_ON        "DEBUGGING MESSAGES INCLUDED, A LOT OF INFO AT STDOUT"
#define STR_DEBUG_MSG_ON_NOFLAG "THIS BINARY IS NOT COMPILED WITH 'DEBUG' ignoring '--debug' flag"


/**
* Predefined messages:
*/
#define PRINT_SIGMA_VERSION( stream ) \
	fprintf( stream, "%s - %s\n", SIGMA_NAME, STR_SIGMA_SLOGAN ); \
	fprintf( stream, "version: %s\n", SIGMA_VERSION ); \
	fprintf( stream, "    by: %s \n", SIGMA_AUTHORS ); \
	fprintf( stream, "    %s \n", SIGMA_SITE );

#define PRINT_SIGMA_HELP( stream, exec_name ) \
	fprintf( stream, "Usage: %s [-c <conf. file path>] [-s] [-v] [-d] [-V] [-h]\n", exec_name ); \
	fprintf( stream, STRTMPL_SIGMA_HELP, SIGMA_DEFAULT_CONFIG_FILE_PATH ); \
	fprintf( stream, "\n" )


#endif // _SIGMA_200MS_NET__LIB_TRANSLATIONS_EN_H_

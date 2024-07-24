#ifndef _SIGMA_200MS_NET__SHELL_ARG_PARSER_H_
#define _SIGMA_200MS_NET__SHELL_ARG_PARSER_H_

#include "../sigma.h"

/**
* Set program context acording to given parameters (silent/verbose, debug mode),
* return configuration (ini) file path.
*/
char *parse_arguments( int argc, char *argv[] );

void parser_claen( char *conf_file );

#endif // _SIGMA_200MS_NET__SHELL_ARG_PARSER_H_

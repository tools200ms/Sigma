#ifndef _SIGMA_200MS_NET__SHELL_ARG_PARSER_H_
#define _SIGMA_200MS_NET__SHELL_ARG_PARSER_H_

#include "../sigma.h"

typedef struct configuration {
    char *config_file;
}
Config;

Config *parse_arguments( int argc, char *argv[] );

#endif // _SIGMA_200MS_NET__SHELL_ARG_PARSER_H_

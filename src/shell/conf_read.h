#ifndef _SIGMA_200MS_NET__SHELL_CONF_READ_H_
#define _SIGMA_200MS_NET__SHELL_CONF_READ_H_


typedef struct configuration {
    char *config_file;
}
Config;

Config *read_conf( const char *conf_file, const char *options );

#endif // _SIGMA_200MS_NET__SHELL_CONF_READ_H_

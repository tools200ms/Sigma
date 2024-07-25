#ifndef _SIGMA_200MS_NET__SHELL_CONF_READ_H_
#define _SIGMA_200MS_NET__SHELL_CONF_READ_H_


struct config_devices {
    char temp[13], relay_mode[16];
    int relay_pins[16];
};

struct config_logic {
    int max, min;
    enum {
        UPPER_LIMIT,
        LOW_LIMIT
    } limit_type;

};

typedef struct sigma_config {

    char *config_file;
    struct config_devices dev;
    struct config_logic logic[2];
}
Config;

Config *read_conf( const char *conf_file );

#endif // _SIGMA_200MS_NET__SHELL_CONF_READ_H_

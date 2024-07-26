#ifndef _SIGMA_200MS_NET__SHELL_CONF_READ_H_
#define _SIGMA_200MS_NET__SHELL_CONF_READ_H_

#include "../drivers/relay.h"

// Maximum parameter count:
// <label>: <param 1> <param 2> <param 3> ...
#define CONFIG_MAX_PARAM_CNT 16
// tells how large buffer to prepare for processing
// numbers it applies to integer and floats:
#define CONFIG_MAX_NUMBER_LEN 16


struct config_devices {
    char temp[13];
    int relay_mode, relay_pins[DEV_RELAY_MAX_CHANEL_CNT];
};

struct config_logic {
    float max, min;
    enum {
        UPPER_LIMIT,
        LOW_LIMIT
    } limit_type;

    // relay ch1 open
    int relay_ch;
};

typedef struct sigma_config {

    struct config_devices dev;
    struct config_logic logic[2];
}
Config;

Config *read_conf( const char *conf_file );

#endif // _SIGMA_200MS_NET__SHELL_CONF_READ_H_

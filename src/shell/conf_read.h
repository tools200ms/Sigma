#ifndef _SIGMA_200MS_NET__SHELL_CONF_READ_H_
#define _SIGMA_200MS_NET__SHELL_CONF_READ_H_

#include "../lib/logic_range.h"

#include "../drivers/1w_bus.h"
#include "../drivers/relay.h"

// Maximum parameter count:
// <label>: <param 1> <param 2> <param 3> ...
#define CONFIG_MAX_PARAM_CNT 16
// tells how large buffer to prepare for processing
// numbers it applies to integer and floats:
#define CONFIG_MAX_NUMBER_LEN 16


struct config_devices {

    W1BusDev *w1bus[8];
    RelayDev *relay[8];
};


typedef struct sigma_config {

    struct config_devices dev;
    LogicRange *logic[4];
}
Config;

Config *read_conf( const char *conf_file );

#endif // _SIGMA_200MS_NET__SHELL_CONF_READ_H_

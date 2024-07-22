
/**
 * Connection:
 * https://www.waveshare.com/1.54inch-LCD-Module.htm
*/

#ifndef _SIGMA_200MS_NET__DRIVERS_1W_BUS_H_
#define _SIGMA_200MS_NET__DRIVERS_1W_BUS_H_

typedef struct w1_bus_dev {

    int pin_ref;

    struct w1_bus_dev *next, *prev;
} W1BusDev;


W1BusDev *w1_bus_add_device(const char *deviceId);

int w1_bus_temp_sensor_rawread(const W1BusDev *w1_dev);

void w1_bus_rm_device(W1BusDev *w1_dev);
void w1_bus_rm_devices();


#endif // _GARDEN_200MS_NET__DRIVERS_1W_BUS_H_


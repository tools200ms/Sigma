
/**
 * Connection:
 * https://www.waveshare.com/1.54inch-LCD-Module.htm
*/

#ifndef _SIGMA_200MS_NET__DRIVERS_1W_BUS_H_
#define _SIGMA_200MS_NET__DRIVERS_1W_BUS_H_

#include <stdbool.h>
#include <math.h>


typedef struct w1_bus_dev_ds18b20 {
    int last_read;
    float temp;
    bool has_changed;

} W1BusDevDS18B20;

typedef union w1_dev_type {
    W1BusDevDS18B20 ds18b20;
} W1DevType;

typedef struct w1_bus_dev {

    W1DevType dev;
    int pin_ref;
    char deviceId[13];

    struct w1_bus_dev *next, *prev;
} W1BusDev;


W1BusDev *w1_bus_add_device( const char *deviceId );
int w1_bus_init_device( W1BusDev *new_dev );

void w1_bus_temp_sensor_load( W1BusDev *w1_dev );
int w1_bus_temp_sensor_rawread( const W1BusDev *w1_dev );
float w1_bus_temp_sensor_read( const W1BusDev *w1_dev );

void w1_bus_rm_device(W1BusDev *w1_dev);
void w1_bus_rm_devices();


#endif // _GARDEN_200MS_NET__DRIVERS_1W_BUS_H_


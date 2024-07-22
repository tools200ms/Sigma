#include <stdio.h>

#include <malloc.h>

#include <wiringPi.h>
#include <ds18b20.h>

#include "../lib/messages.h"

#include "1w_bus.h"

#define WIRINGPI_PINBASE_1W_START 128

static W1BusDev *w1_dev_list_head = NULL;
static W1BusDev *w1_dev_list_tail = NULL;


W1BusDev *w1_bus_add_device(const char *deviceId) {
    static int pinbase_ref = WIRINGPI_PINBASE_1W_START;
    W1BusDev *new_dev;

    MSGBEGIN_INFO_( "Adding 1Wire device: %s: : ", deviceId );

    ds18b20Setup(pinbase_ref, deviceId);

    if((new_dev = malloc(sizeof(W1BusDev))) == NULL) {
        perror("ERROR");
    }

    new_dev->pin_ref = pinbase_ref++;

    if(w1_dev_list_head != NULL) {
        w1_dev_list_tail->next = new_dev;
        new_dev->next = NULL;
        new_dev->prev = w1_dev_list_tail;
    } else {
        w1_dev_list_head = w1_dev_list_tail = new_dev;

        new_dev->next = new_dev->prev = NULL;
    }

    MSGEND_INFO("\tOK");
    return new_dev;
}

inline int w1_bus_temp_sensor_rawread(const W1BusDev *w1_dev) {
    int temp;

    temp = analogRead(w1_dev->pin_ref);

    return temp;
}

void w1_bus_rm_device(W1BusDev *w1_dev) {
    W1BusDev *p, *n;

    p = w1_dev->prev;
    n = w1_dev->next;

    if( p != NULL ) {
        p->next = n;
    } else {
        w1_dev_list_head = n;
    }

    if( n != NULL ) {
        p->prev = p;
    } else {
        w1_dev_list_tail = p;
    }

    // TODO: cleanup in WiringPI
    free(w1_dev);
}

void w1_bus_rm_devices() {
    W1BusDev *tmp, *head = w1_dev_list_head;

    w1_dev_list_head = w1_dev_list_tail = NULL;

    // let's eat starting from head
    while( head != NULL ) {
        tmp = head->next;

        // TODO: cleanup in WiringPI
        free(head);

        head = tmp;
    }

}

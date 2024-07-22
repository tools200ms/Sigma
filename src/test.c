
#include <stdio.h>
#include <stdlib.h>

#define RELAY_MODE_NULL           0x80
#define RELAY_MODE_NORMALLY_OPEN  0x01 | RELAY_MODE_NULL // normally default settings for relay
#define RELAY_MODE_NORMALLY_CLOSE 0x02 | RELAY_MODE_NULL

#define RELAY_MODE_RESET_AT_INIT  0x04 | RELAY_MODE_NULL



int main() {

    unsigned long long status = 0, status_mask;
    int ch_no = 2, res;

    //status_mask = (status_mask << (ch_no - 1));

    // set on, ch2
    status_mask = 1ULL << (ch_no - 1);
    (status) |= status_mask;

    printf("Bit ON\n");

    // set on, ch1
    ch_no = 1;
    status_mask = 1ULL << (ch_no - 1);
    (status) |= status_mask;

    printf("Bit ON\n");

    ch_no = 2;
    // read bit
    status_mask = 1ULL << (ch_no - 1);

    res = (((status) & status_mask) != 0);

    printf("Bit is: %d\n", res);

    // set off
    status_mask = 1ULL << (ch_no - 1);
    (status) &= (~status_mask);

    printf("Bit OFF\n");

    int mode = RELAY_MODE_NORMALLY_OPEN | RELAY_MODE_RESET_AT_INIT;

    if( ( mode & (RELAY_MODE_NORMALLY_OPEN) ) == (RELAY_MODE_NORMALLY_OPEN) ) {
        printf("More ON");
    }

    return 0;
}

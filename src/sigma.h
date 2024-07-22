
#ifndef _SIGMA_200MS_NET___SIGMA_H_
#define _SIGMA_200MS_NET___SIGMA_H_

#include <stdint.h>

#include "about.h"

// Settings:
#define SIGMA_DEFAULT_CONFIG_FILE_PATH "/etc/sigma.ini"


// uint16_t *init_display(void);
void  Handler_clean(int signo);

// void redraw(UWORD *img_buff, float *temp, short relay_ch1_state, short relay_ch2_state);


#define IS_VALID_TEMP_VALUE(temp) (temp >= (8*10) && temp <= (54*10))

#define IS_VALID_TEMP_RANGE_VALUE(temp_upper, temp_down) ((temp_upper - temp_down) >= (2*10))

#endif // _SIGMA_200MS_NET___SIGMA_H_

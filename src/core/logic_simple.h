#ifndef _SIGMA_200MS_NET__CORE_LOGIC_SIMPLE_H_
#define _SIGMA_200MS_NET__CORE_LOGIC_SIMPLE_H_

#include <stdbool.h>


typedef struct logic_range {
    int val_min, val_max;
    void *in, *out_on, *out_off;
} LogicRange;

void logicsimple_init( LogicRange *ranges );

void logicsimple_loop();

bool logicsimple_limit_upper( LogicRange *range, int scalar );
bool logicsimple_limit_low( LogicRange *range, int scalar );

#endif // _SIGMA_200MS_NET__CORE_LOGIC_SIMPLE_H_

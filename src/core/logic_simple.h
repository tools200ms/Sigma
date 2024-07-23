#ifndef _SIGMA_200MS_NET__._CORE_LOGIC_SIMPLE_H_
#define _SIGMA_200MS_NET__._CORE_LOGIC_SIMPLE_H_


#define LOGIC_UPPER_LIMIT 1
#define LOGIC_DOWN_LIMIT  2

typeof struct logic_range {
    int val_min, val_max;
    int limit;
    void *in, *out_on, *out_off;
} LogicRange;

void logicsimple_init( LogicRange *ranges );

void logicsimple_loop();

#endif // _SIGMA_200MS_NET__._CORE_LOGIC_SIMPLE_H_

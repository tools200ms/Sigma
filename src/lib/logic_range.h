
#ifndef _SIGMA_200MS_NET__LIB_LOGIC_RANGE_H_
#define _SIGMA_200MS_NET__LIB_LOGIC_RANGE_H_

typedef enum {
    LOGIC_RANGE_UPPER_LIMIT = 3,
    LOGIC_RANGE_LOW_LIMIT = 1
} LogicRangeType;

typedef enum {
    LOGIC_DEV_INIT,
    LOGIC_TURNED_OFF,
    LOGIC_TURNED_ON,

    LOGIC_KEEP_OFF,
    LOGIC_KEEP_ON,

    LOGIC_TURN_OFF,
    LOGIC_TURN_ON
} BianryRangeLogic;

typedef struct logic_range_values {
    float max, min;
} LogicRangeValues;

typedef struct logic_range {
    LogicRangeValues limit;
    LogicRangeType type;

    BianryRangeLogic status;
    void *in, *out_on, *out_off;
} LogicRange;


LogicRange *logic_range_create( const LogicRangeType type, const float numb1, const float numb2 );

void logic_range_setio( LogicRange *lr, float numb1, float numb2 );
BianryRangeLogic logic_range_inrange( LogicRange *lr, const float val );

#endif // _SIGMA_200MS_NET__LIB_LOGIC_RANGE_H_

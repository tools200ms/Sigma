
#include <stdlib.h>

#include "messages.h"
#include "logic_range.h"

LogicRange *logic_range_create( const LogicRangeType type, const float numb1, const float numb2 ) {
    LogicRange *lr;

    if( (lr = malloc( sizeof(LogicRange) )) == NULL ) {
        MSG_ERROR_MALLOC();
    }

    switch( ( lr->type = type) ) {
        case LOGIC_RANGE_UPPER_LIMIT:
            lr->limit.max = numb1;
            lr->limit.min = numb2;
        break;

        case LOGIC_RANGE_LOW_LIMIT:
            lr->limit.min = numb1;
            lr->limit.max = numb2;
        break;
    }

    if( (lr->limit.max - lr->limit.min) < 0.1 ) {
        MSG_WARN("Max. temp. should exceed Min.");

        return NULL;
    }

    lr->status = LOGIC_DEV_INIT;
    return lr;
}

BianryRangeLogic logic_range_inrange( LogicRange *lr, const float val ) {

    if( lr->limit.max < val && lr->status != LOGIC_TURNED_ON ) {
        lr->status = LOGIC_TURNED_ON;
        return LOGIC_TURN_ON;
    } else if( lr->limit.min > val && lr->status == LOGIC_TURNED_ON ) {
        lr->status = LOGIC_TURNED_OFF;
        return LOGIC_TURN_OFF;
    }

    return (lr->status == LOGIC_TURNED_OFF) ? LOGIC_KEEP_OFF : LOGIC_KEEP_ON;
}

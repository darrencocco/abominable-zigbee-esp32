//
// Created by dcocco on 13/01/25.
//

#ifndef ABOMINABLEZIGBEEMULTISTATEINPUT_H
#define ABOMINABLEZIGBEEMULTISTATEINPUT_H

#define AZB_ZCL_CLUSTER_ID_MULTI_INPUT 0xFF12

#define AZB_ZCL_ATTR_MULTI_INPUT_STATE_TEXT_ID 0x000E
#define AZB_ZCL_ATTR_MULTI_INPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_MULTI_INPUT_NUMBER_OF_STATES_ID 0x004A
#define AZB_ZCL_ATTR_MULTI_INPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_MULTI_INPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_MULTI_INPUT_STATUS_FLAGS_ID 0x006F
#include "AbominableZigbeeMultistateBase.h"

class AbominableZigbeeMultistateInput : AbominableZigbeeMultistateBase {
public:
    AbominableZigbeeMultistateInput(uint8_t endpoint);
};

#endif //ABOMINABLEZIGBEEMULTISTATEINPUT_H
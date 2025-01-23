//
// Created by dcocco on 25/12/24.
//

#ifndef ZIGBEEMULTISTATEVALUE_H
#define ZIGBEEMULTISTATEVALUE_H

#define AZB_ZCL_CLUSTER_ID_MULTI_OUTPUT 0xFF13 // Moved into vendor specific space because Espressif weirdness.

#define AZB_ZCL_ATTR_MULTI_OUTPUT_STATE_TEXT_ID 0x000E
#define AZB_ZCL_ATTR_MULTI_OUTPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_MULTI_OUTPUT_NUMBER_OF_STATES_ID 0x004A
#define AZB_ZCL_ATTR_MULTI_OUTPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_MULTI_OUTPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_MULTI_OUTPUT_STATUS_FLAGS_ID 0x006F

#include "AbominableZigbeeMultistateBase.h"

class AbominableZigbeeMultistateOutput : public AbominableZigbeeMultistateBase {
public:
  AbominableZigbeeMultistateOutput(uint8_t endpoint);
  ~AbominableZigbeeMultistateOutput();

};

#endif //ZIGBEEMULTISTATEVALUE_H
//
// Created by dcocco on 13/01/25.
//

#ifndef ABOMINABLEBINARYOUTPUT_H
#define ABOMINABLEBINARYOUTPUT_H

#define AZB_ZCL_CLUSTER_ID_BINARY_OUTPUT 0xFF10 // Fix for more Espressif lib bugs.

#define AZB_ZCL_ATTR_BINARY_OUTPUT_ACTIVE_TEXT_ID 0x0004
#define AZB_ZCL_ATTR_BINARY_OUTPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_BINARY_OUTPUT_INACTIVE_TEXT_ID 0x002E
#define AZB_ZCL_ATTR_BINARY_OUTPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_BINARY_OUTPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_BINARY_OUTPUT_STATUS_FLAGS_ID 0x006F
#include "AbominableZigbeeBinaryBase.h"

class AbominableZigbeeBinaryOutput : public AbominableZigbeeBinaryBase {
public:
    AbominableZigbeeBinaryOutput(uint8_t endpoint);
};



#endif //ABOMINABLEBINARYOUTPUT_H

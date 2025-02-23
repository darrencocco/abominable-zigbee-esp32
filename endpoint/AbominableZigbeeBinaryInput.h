//
// Created by dcocco on 13/01/25.
//

#ifndef ABOMINABLEZIGBEEBINARYINPUT_H
#define ABOMINABLEZIGBEEBINARYINPUT_H

#define AZB_ZCL_CLUSTER_ID_BINARY_INPUT 0x000F

#define AZB_ZCL_ATTR_BINARY_INPUT_ACTIVE_TEXT_ID 0x0004
#define AZB_ZCL_ATTR_BINARY_INPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_BINARY_INPUT_INACTIVE_TEXT_ID 0x002E
#define AZB_ZCL_ATTR_BINARY_INPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_BINARY_INPUT_STATUS_FLAGS_ID 0x006F

#include "AbominableZigbeeBinaryBase.h"


class AbominableZigbeeBinaryInput : public AbominableZigbeeBinaryBase {
public:
    AbominableZigbeeBinaryInput(uint8_t endpoint);
protected:
    void _addClusterSpecificCluster(esp_zb_cluster_list_t *clusterList) override;
};



#endif //ABOMINABLEZIGBEEBINARYINPUT_H

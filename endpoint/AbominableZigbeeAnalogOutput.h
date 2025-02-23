//
// Created by dcocco on 12/01/25.
//

#ifndef ABOMINABLEZIGBEEANALOGVALUE_H
#define ABOMINABLEZIGBEEANALOGVALUE_H

#include "AbominableZigbeeAnalogBase.h"

#include "ha/esp_zigbee_ha_standard.h"

#define AZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT 0x000D

#define AZB_ZCL_ATTR_ANALOG_OUTPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_MAX_PRESENT_VALUE_ID 0x0041
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_MIN_PRESENT_VALUE_ID 0x0045
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_RESOLUTION_ID 0x006A
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_STATUS_FLAGS_ID 0x006F
#define AZB_ZCL_ATTR_ANALOG_OUTPUT_ENGINEERING_UNITS_ID 0x0075

class AbominableZigbeeAnalogOutput : public AbominableZigbeeAnalogBase {
public:
    AbominableZigbeeAnalogOutput(uint8_t endpoint);
    ~AbominableZigbeeAnalogOutput();

protected:
    void _addClusterSpecificCluster(esp_zb_cluster_list_t* clusterList) override;

    void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;
    void zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) override;
};

#endif //ABOMINABLEZIGBEEANALOGVALUE_H
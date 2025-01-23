//
// Created by dcocco on 13/01/25.
//

#ifndef ABOMINABLEZIGBEEANALOGINPUT_H
#define ABOMINABLEZIGBEEANALOGINPUT_H

#define AZB_ZCL_CLUSTER_ID_ANALOG_INPUT 0x000C

#define AZB_ZCL_ATTR_ANALOG_INPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_ANALOG_INPUT_MAX_PRESENT_VALUE_ID 0x0041
#define AZB_ZCL_ATTR_ANALOG_INPUT_MIN_PRESENT_VALUE_ID 0x0045
#define AZB_ZCL_ATTR_ANALOG_INPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_ANALOG_INPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_ANALOG_INPUT_RESOLUTION_ID 0x006A
#define AZB_ZCL_ATTR_ANALOG_INPUT_STATUS_FLAGS_ID 0x006F
#define AZB_ZCL_ATTR_ANALOG_INPUT_ENGINEERING_UNITS_ID 0x0075

#include <zcl/esp_zigbee_zcl_command.h>

#include "AbominableZigbeeAnalogBase.h"

class AbominableZigbeeAnalogInput : public AbominableZigbeeAnalogBase {
public:
    AbominableZigbeeAnalogInput(uint8_t endpoint);
    ~AbominableZigbeeAnalogInput();

protected:
    void _addClusterSpecificCluster(esp_zb_cluster_list_t* clusterList) override;

    void (*_on_config_receive)(uint16_t, uint16_t, std::list<char *>);
    void (*_on_value_receive)(uint16_t);

    void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;
    void zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) override;
};



#endif //ABOMINABLEZIGBEEANALOGINPUT_H

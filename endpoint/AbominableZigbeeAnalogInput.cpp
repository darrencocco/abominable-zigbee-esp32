//
// Created by dcocco on 13/01/25.
//

#include "AbominableZigbeeAnalogInput.h"

#include <esp_zigbee_cluster.h>

AbominableZigbeeAnalogInput::AbominableZigbeeAnalogInput(uint8_t endpoint) : AbominableZigbeeAnalogBase(endpoint) {
    _clusterId = AZB_ZCL_CLUSTER_ID_ANALOG_INPUT;
    _descriptionId = AZB_ZCL_ATTR_ANALOG_INPUT_DESCRIPTION_ID;
    _maxPresentValueId = AZB_ZCL_ATTR_ANALOG_INPUT_MAX_PRESENT_VALUE_ID;
    _minPresentValueId = AZB_ZCL_ATTR_ANALOG_INPUT_MIN_PRESENT_VALUE_ID;
    _outOfServiceId = AZB_ZCL_ATTR_ANALOG_INPUT_OUT_OF_SERVICE_ID;
    _presentValueId = AZB_ZCL_ATTR_ANALOG_INPUT_PRESENT_VALUE_ID;
    _resolutionId = AZB_ZCL_ATTR_ANALOG_INPUT_RESOLUTION_ID;
    _statusFlagsId = AZB_ZCL_ATTR_ANALOG_INPUT_STATUS_FLAGS_ID;
    _engineeringUnitsId = AZB_ZCL_ATTR_ANALOG_INPUT_ENGINEERING_UNITS_ID;

    _setup();
}

AbominableZigbeeAnalogInput::~AbominableZigbeeAnalogInput() {
}

void AbominableZigbeeAnalogInput::_addClusterSpecificCluster(esp_zb_cluster_list_t *clusterList) {
    esp_zb_cluster_list_add_analog_input_cluster(clusterList, _createCustomClusterDefinition(), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void AbominableZigbeeAnalogInput::zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) {
    AbominableZigbeeAnalogBase::zbAttributeSet(message);
}

void AbominableZigbeeAnalogInput::zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) {
    AbominableZigbeeAnalogBase::zbAttributeRead(cluster_id, attribute);
}

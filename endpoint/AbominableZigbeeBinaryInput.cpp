//
// Created by dcocco on 13/01/25.
//

#include "AbominableZigbeeBinaryInput.h"

#include <esp_zigbee_cluster.h>
#include <zcl/esp_zigbee_zcl_common.h>

AbominableZigbeeBinaryInput::AbominableZigbeeBinaryInput(uint8_t endpoint) : AbominableZigbeeBinaryBase(endpoint){
    _clusterId = AZB_ZCL_CLUSTER_ID_BINARY_INPUT;
    _activeTextId = AZB_ZCL_ATTR_BINARY_INPUT_ACTIVE_TEXT_ID;
    _descriptionId = AZB_ZCL_ATTR_BINARY_INPUT_DESCRIPTION_ID;
    _inactiveTextId = AZB_ZCL_ATTR_BINARY_INPUT_INACTIVE_TEXT_ID;
    _outOfServiceId = AZB_ZCL_ATTR_BINARY_INPUT_OUT_OF_SERVICE_ID;
    _presentValueId = AZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID;
    _statusFlagsId = AZB_ZCL_ATTR_BINARY_INPUT_STATUS_FLAGS_ID;

    _setup();
}

void AbominableZigbeeBinaryInput::_addClusterSpecificCluster(esp_zb_cluster_list_t *clusterList) {
    esp_zb_cluster_list_add_binary_input_cluster(clusterList, _createCustomClusterDefinition(), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

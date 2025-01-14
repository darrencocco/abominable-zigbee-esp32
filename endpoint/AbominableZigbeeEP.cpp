//
// Created by dcocco on 14/01/25.
//

#include "AbominableZigbeeEP.h"

#include <esp_zigbee_cluster.h>
#include <zcl/esp_zigbee_zcl_common.h>

void AbominableZigbeeEP::setDeviceType(uint16_t deviceType) {
    _device_id = (esp_zb_ha_standard_devices_t)deviceType;
    _ep_config.app_device_id = (esp_zb_ha_standard_devices_t)deviceType;
}

AbominableZigbeeEP::AbominableZigbeeEP(uint8_t endpoint) : ZigbeeEP(endpoint){}

void AbominableZigbeeEP::_setup() {
    _device_id = ESP_ZB_HA_CUSTOM_ATTR_DEVICE_ID;

    azb_zigbee_base_cfg_t endpointClusterConfig = AZB_ZIGBEE_DEFAULT_BASE_CONFIG();
    _cluster_list = _generateClusterList(&endpointClusterConfig);

    _ep_config = {
        .endpoint = _endpoint, .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID, .app_device_id = (esp_zb_ha_standard_devices_t)_device_id, .app_device_version = 0
    };
}

esp_zb_cluster_list_t * AbominableZigbeeEP::_generateClusterList(azb_zigbee_base_cfg_t *endpointClusterConfig) {
    esp_zb_basic_cluster_cfg_t *basicCfg = endpointClusterConfig ? &(endpointClusterConfig->basic_cfg) : NULL;
    esp_zb_identify_cluster_cfg_t *identifyCfg = endpointClusterConfig ? &(endpointClusterConfig->identify_cfg) : NULL;
    esp_zb_cluster_list_t *clusterList = esp_zb_zcl_cluster_list_create();
    esp_zb_cluster_list_add_basic_cluster(clusterList, esp_zb_basic_cluster_create(basicCfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    esp_zb_cluster_list_add_identify_cluster(clusterList, esp_zb_identify_cluster_create(identifyCfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    _addClusterSpecificCluster(clusterList);
    return clusterList;
}

void AbominableZigbeeEP::_addClusterSpecificCluster(esp_zb_cluster_list_t *clusterList) {
    esp_zb_cluster_list_add_custom_cluster(clusterList, _createCustomClusterDefinition(), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

esp_zb_attribute_list_t * AbominableZigbeeEP::_getCluster(uint16_t clusterId) {
    return esp_zb_cluster_list_get_cluster(_cluster_list, clusterId, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

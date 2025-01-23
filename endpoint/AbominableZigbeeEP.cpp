//
// Created by dcocco on 14/01/25.
//

#include "AbominableZigbeeEP.h"

#include <cstdio>
#include <cstring>
#include <esp_zigbee_cluster.h>
#include <esp_zigbee_core.h>
#include <nwk/esp_zigbee_nwk.h>
#include <zcl/esp_zigbee_zcl_common.h>
#include <zdo/esp_zigbee_zdo_command.h>
#include <zdo/esp_zigbee_zdo_common.h>

void AbominableZigbeeEP::presetDeviceType(uint16_t deviceType) {
    _device_id = (esp_zb_ha_standard_devices_t)deviceType;
    _ep_config.app_device_id = (esp_zb_ha_standard_devices_t)deviceType;
}

void AbominableZigbeeEP::bindToCoordinator() {
    if (_customBindingRequired && !_coordinatorBound) {
        findAndBindCoordinator();
        delay(100);
    }
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
    esp_zb_attribute_list_t *clusterDefinition = _createCustomClusterDefinition();
    esp_zb_cluster_list_add_custom_cluster(clusterList, clusterDefinition, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

esp_zb_attribute_list_t * AbominableZigbeeEP::_getCluster(uint16_t clusterId) {
    return esp_zb_cluster_list_get_cluster(_cluster_list, clusterId, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void AbominableZigbeeEP::bindCluster(uint16_t fromShort,
          esp_zb_ieee_addr_t* fromLong,
          uint8_t fromEndpoint,
          uint16_t fromCluster,
          esp_zb_ieee_addr_t* toLong,
          uint8_t toEndpoint,
          esp_zb_zdo_bind_callback_t callback,
          void *user_ctx) {
    esp_zb_zdo_bind_req_param_t bind_req;
    bind_req.dst_addr_mode = ESP_ZB_ZDO_BIND_DST_ADDR_MODE_64_BIT_EXTENDED;
    memcpy(bind_req.src_address, fromLong, sizeof(esp_zb_ieee_addr_t));
    memcpy(bind_req.dst_address_u.addr_long, toLong, sizeof(esp_zb_ieee_addr_t));
    bind_req.req_dst_addr = fromShort;
    bind_req.src_endp = fromEndpoint;
    bind_req.cluster_id = fromCluster;
    bind_req.dst_endp = toEndpoint;
    esp_zb_zdo_device_bind_req(&bind_req, callback, user_ctx);
}

void AbominableZigbeeEP::printIeeeAddress(const char* prefix, esp_zb_ieee_addr_t address) {
    log_d("%s: %2X:%2X:%2X:%2X:%2X:%2X:%2X:%2X", prefix, address[7], address[6], address[5], address[4], address[3], address[2], address[1], address[0]);
}

struct simpleTuple {
    void* self;
    void* data;
};

void AbominableZigbeeEP::fbcoord3(esp_zb_zdp_status_t zdo_status, void *user_ctx) {
    AbominableZigbeeEP* instance = static_cast<AbominableZigbeeEP*>(user_ctx);
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) {
        instance->_coordinatorBound = true;
    }
}

void AbominableZigbeeEP::fbcoord1(esp_zb_zdp_status_t zdo_status, esp_zb_zdo_ieee_addr_rsp_t *resp, void *user_ctx) {
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) {
        esp_zb_ieee_addr_t fromLong;
        esp_zb_get_long_address(fromLong);
        AbominableZigbeeEP* instance = static_cast<AbominableZigbeeEP*>(user_ctx);
        bool lockAcquired;
        do {
            lockAcquired = esp_zb_lock_acquire(portMAX_DELAY);
        } while (!lockAcquired);
        bindCluster(esp_zb_get_short_address(), &fromLong, instance->_endpoint,
            instance->_clusterId, &resp->ieee_addr, 1, fbcoord3, instance);
        esp_zb_lock_release();
    }
}

void AbominableZigbeeEP::findAndBindCoordinator() {
    bool lockAcquired;
    do {
        lockAcquired = esp_zb_lock_acquire(portMAX_DELAY);
    } while (!lockAcquired);
    esp_zb_zdo_ieee_addr_req_param_t addrReqParam;
    addrReqParam.addr_of_interest = 0x0000;
    addrReqParam.dst_nwk_addr = 0x0000;
    addrReqParam.request_type = 0;
    addrReqParam.start_index = 0;
    esp_zb_zdo_ieee_addr_req(&addrReqParam, fbcoord1, this);
    esp_zb_lock_release();
}
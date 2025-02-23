//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEEP_H
#define ABOMINABLEZIGBEEEP_H
#include <esp_zigbee_attribute.h>
#include <esp_zigbee_type.h>
#include <ZigbeeEP.h>
#include <zdo/esp_zigbee_zdo_command.h>
#include <zdo/esp_zigbee_zdo_common.h>
#include <Preferences.h>

#include "soc/soc_caps.h"

#define AZB_MAX_STRING_LEN 16

typedef enum {
    AZB_ZCL_STATUS_FLAGS_NORMAL         = 0x00, /*!< Normal (default) . */
    AZB_ZCL_STATUS_FLAGS_IN_ALARM       = 0x01, /*!< In alarm bit. */
    AZB_ZCL_STATUS_FLAGS_FAULT          = 0x02, /*!< Fault bit. */
    AZB_ZCL_STATUS_FLAGS_OVERRIDDEN     = 0x04, /*!< Overridden bit. */
    AZB_ZCL_STATUS_FLAGS_OUT_OF_SERVICE = 0x08, /*!< Out of service bit. */
} azb_zcl_status_flags_value_t;

typedef struct azb_zigbee_base_cfg_s {
    esp_zb_basic_cluster_cfg_t basic_cfg;
    esp_zb_identify_cluster_cfg_t identify_cfg;
} azb_zigbee_base_cfg_t;

#define AZB_ZIGBEE_DEFAULT_BASE_CONFIG() {                                  \
    .basic_cfg = {                                                          \
        .zcl_version = ESP_ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,          \
        .power_source = ESP_ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE,        \
    },                                                                      \
    .identify_cfg = {                                                       \
        .identify_time = ESP_ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE,   \
    },                                                                      \
}

struct bindingUserCtx {
    uint8_t endpoint;
    uint16_t clusterId;
};

class AbominableZigbeeEP : public ZigbeeEP {
public:
    void presetManufacturerAndModel(const char *name, const char *model) {
        ZigbeeEP::setManufacturerAndModel(name, model);
    }
    void presetDeviceType(uint16_t deviceType);

    void bindToCoordinator();

    bool isBoundToCoordinator() {
        return _coordinatorBound;
    }
protected:
    AbominableZigbeeEP(uint8_t endpoint);

    uint16_t _clusterId;

    bool _customBindingRequired = false;
    bool _coordinatorBound = true;
    char* _bindingStorageKey;
    Preferences storage;

    virtual void _setup();

    esp_zb_cluster_list_t* _generateClusterList(azb_zigbee_base_cfg_t *endpointClusterConfig);
    virtual void _addClusterSpecificCluster(esp_zb_cluster_list_t* clusterList);
    virtual esp_zb_attribute_list_t *_createCustomClusterDefinition() = 0;
    esp_zb_attribute_list_t *_getCluster(uint16_t clusterId);

    void printIeeeAddress(const char* prefix, esp_zb_ieee_addr_t address);

    static void fbcoord1(esp_zb_zdp_status_t zdo_status, esp_zb_zdo_ieee_addr_rsp_t *resp, void *user_ctx);

    static void fbcoord3(esp_zb_zdp_status_t zdo_status, void *user_ctx);

    static void fbcoord2(const esp_zb_zdo_binding_table_info_t *table_info, void *user_ctx);

    static void bindCluster(uint16_t fromShort, esp_zb_ieee_addr_t *fromLong, uint8_t fromEndpoint, uint16_t fromCluster,
                            esp_zb_ieee_addr_t *toLong, uint8_t toEndpoint, esp_zb_zdo_bind_callback_t callback, void *user_ctx);

    void findAndBindCoordinator();
};

#endif //ABOMINABLEZIGBEEEP_H
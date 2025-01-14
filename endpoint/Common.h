//
// Created by dcocco on 12/01/25.
//

#ifndef COMMON_H
#define COMMON_H
#include <esp_zigbee_type.h>

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
#endif //COMMON_H

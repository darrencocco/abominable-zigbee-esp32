//
// Created by dcocco on 25/12/24.
//

#include "AbominableZigbeeMultistateValue.h"
#include "esp_zigbee_core.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "../data/Marshal.h"

AbominableZigbeeMultistateValue::AbominableZigbeeMultistateValue(uint8_t endpoint) : ZigbeeEP(endpoint){
    _device_id = ESP_ZB_HA_CUSTOM_ATTR_DEVICE_ID;

    azb_zigbee_multistate_value_cfg_t endpointClusterConfig = AZB_ZIGBEE_DEFAULT_MULTISTATE_VALUE_CONFIG();
    _cluster_list = _generateClusterList(&endpointClusterConfig);

    _ep_config = {
        .endpoint = _endpoint, .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID, .app_device_id = (esp_zb_ha_standard_devices_t)_device_id, .app_device_version = 0
      };
}

void AbominableZigbeeMultistateValue::setDeviceType(uint16_t deviceType) {
    _device_id = (esp_zb_ha_standard_devices_t)deviceType;
    _ep_config.app_device_id = (esp_zb_ha_standard_devices_t)deviceType;
}

void AbominableZigbeeMultistateValue::setDescription(char *desc) {
    if (strlen(desc) > AZB_MAX_STRING_LEN) {
        throw std::invalid_argument("ZigbeeMultistateValue::Description string too long, must be 16 characters or less.");
    }
    if (_description == NULL) {
        _description = (char *)malloc(sizeof(char) * (AZB_MAX_STRING_LEN + 1));
    }
    strcpy(_description, desc);
    uint8_t* newDescription = Marshal::String::toZigbeeString(_description, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE),
       ESP_ZB_ZCL_ATTR_MULTI_VALUE_DESCRIPTION_ID, newDescription);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_DESCRIPTION_ID, esp_err_to_name(result));
}

void AbominableZigbeeMultistateValue::setOptions(const char **options, const uint16_t length) {
    size_t biggestString = 0;
    for (int i = 0; i < length; i++) {
        biggestString = std::max(strlen(options[i]), biggestString);
    }
    if (biggestString > AZB_MAX_STRING_LEN) {
        // At least one of the options passed was too long.
        throw std::invalid_argument("ZigbeeMultistateValue::setOptions no option can be more than 16 characters in length");
    }
    if (_options != NULL) {
        for (int i = 0; i < _numberOfOptions; i++) {
            free(_options[i]);
        }
        free(_options);
    }

    _options = static_cast<char **>(malloc(length * sizeof(char *)));
    for (int i = 0; i < length; i++) {
        _options[i] = static_cast<char *>(malloc(sizeof(char) * biggestString + 1));
        strcpy(_options[i], options[i]);
    }
    _numberOfOptions = length;
    _currentSelection = 0;

    uint8_t* optionsPacked = Marshal::Array::toZigbeeArray(_options, _numberOfOptions, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);

    esp_zb_attribute_list_t *multistateValueCluster = _getCluster(ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE);
    esp_err_t result = esp_zb_cluster_update_attr(multistateValueCluster, ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATE_TEXT_ID, optionsPacked);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATE_TEXT_ID, esp_err_to_name(result));
    result = esp_zb_cluster_update_attr(multistateValueCluster, ESP_ZB_ZCL_ATTR_MULTI_VALUE_NUMBER_OF_STATES_ID, &_numberOfOptions);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_NUMBER_OF_STATES_ID, esp_err_to_name(result));
    result = esp_zb_cluster_update_attr(multistateValueCluster, ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID, &_currentSelection);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID, esp_err_to_name(result));
}

void AbominableZigbeeMultistateValue::setNumberOfOptions(uint16_t count) {
    _numberOfOptions = count;
    uint16_t numberOfOptions = _numberOfOptions;
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE),
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_NUMBER_OF_STATES_ID, &numberOfOptions);
    free(&numberOfOptions);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_NUMBER_OF_STATES_ID, esp_err_to_name(result));
}

void AbominableZigbeeMultistateValue::setSelection(const uint16_t selection) {
    if (selection > _numberOfOptions) {
        throw std::invalid_argument("ZigbeeMultistateValue::setSelection out of range");
    }
    _currentSelection = selection;
    uint16_t currentSelection = _currentSelection;
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE),
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID, &currentSelection);
    free(&currentSelection);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID, esp_err_to_name(result));
}

char *AbominableZigbeeMultistateValue::getDescription() {
    return _description;
}

char ** AbominableZigbeeMultistateValue::getOptions() {
    return _options;
}

uint16_t AbominableZigbeeMultistateValue::getNumberOfOptions() {
    return _numberOfOptions;
}

uint16_t AbominableZigbeeMultistateValue::getSelection() {
    return _currentSelection;
}

uint8_t AbominableZigbeeMultistateValue::getStatusFlags() {
    return ESP_ZB_ZCL_MULTI_VALUE_STATUS_FLAGS_NORMAL
        + _flagInAlarm * ESP_ZB_ZCL_MULTI_VALUE_STATUS_FLAGS_IN_ALARM
        + _flagFault * ESP_ZB_ZCL_MULTI_VALUE_STATUS_FLAGS_FAULT
        + _flagOverriden * ESP_ZB_ZCL_MULTI_VALUE_STATUS_FLAGS_OVERRIDDEN
        + _flagOutOfService * ESP_ZB_ZCL_MULTI_VALUE_STATUS_FLAGS_OUT_OF_SERVICE;
}

bool AbominableZigbeeMultistateValue::isOutOfService() {
    return _flagOutOfService;
}

void AbominableZigbeeMultistateValue::setReporting(uint16_t minInterval, uint16_t maxInterval) {
    // clang-format off
    esp_zb_zcl_reporting_info_t reportingInfo = {
        .direction = ESP_ZB_ZCL_CMD_DIRECTION_TO_SRV,
        .ep = _endpoint,
        .cluster_id = ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE,
        .cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        .attr_id = ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID,
        .u =
          {
            .send_info =
              {
                .min_interval = minInterval,
                .max_interval = maxInterval,
                .delta =
                  {
                    .u16 = 1,
                  },
                .def_min_interval = 0,
                .def_max_interval = 30,
              },
          },
        .dst =
          {
            .profile_id = ESP_ZB_AF_HA_PROFILE_ID,
          },
        .manuf_code = ESP_ZB_ZCL_ATTR_NON_MANUFACTURER_SPECIFIC,
      };
    // clang-format on
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_update_reporting_info(&reportingInfo);
    esp_zb_lock_release();
}

void AbominableZigbeeMultistateValue::zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) {
    if (message->info.cluster == ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE) {
        switch (message->attribute.id) {
            case ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID:
                // _currentSelection = *((uint16_t*)message->attribute.data.value);
                // _on_value_receive(ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID);
                break;
            case ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATE_TEXT_ID:
                break;
            case ESP_ZB_ZCL_ATTR_MULTI_VALUE_OUT_OF_SERVICE_ID:
            //     _flagOutOfService = *((bool*)message->attribute.data.value);
            // _on_value_receive(ESP_ZB_ZCL_ATTR_MULTI_VALUE_OUT_OF_SERVICE_ID);
                break;
            case ESP_ZB_ZCL_ATTR_MULTI_VALUE_DESCRIPTION_ID:
                break;
            default:
                break;
        }
    }
}

void AbominableZigbeeMultistateValue::zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) {

}

void AbominableZigbeeMultistateValue::_updateFlags() {
    uint8_t flags = getStatusFlags();
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE),
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATUS_FLAGS_ID, &flags);
    log_d("Updating %02X: %s", ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATUS_FLAGS_ID, esp_err_to_name(result));
}

esp_zb_cluster_list_t *AbominableZigbeeMultistateValue::_generateClusterList(azb_zigbee_multistate_value_cfg_t* endpointClusterConfig) {
    esp_zb_basic_cluster_cfg_t *basic_cfg = endpointClusterConfig ? &(endpointClusterConfig->basic_cfg) : NULL;
    esp_zb_identify_cluster_cfg_t *identify_cfg = endpointClusterConfig ? &(endpointClusterConfig->identify_cfg) : NULL;
    esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();
    esp_zb_cluster_list_add_basic_cluster(cluster_list, esp_zb_basic_cluster_create(basic_cfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    esp_zb_cluster_list_add_identify_cluster(cluster_list, esp_zb_identify_cluster_create(identify_cfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    esp_zb_cluster_list_add_multistate_value_cluster(cluster_list, _createCustomClusterDefinition(), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    return cluster_list;
}

esp_zb_attribute_list_t* AbominableZigbeeMultistateValue::_createCustomClusterDefinition() {
    esp_zb_attribute_list_t *customCluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_MULTI_VALUE);

    uint8_t noOptions[] = {0, 0};
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATE_TEXT_ID,
        ESP_ZB_ZCL_ATTR_TYPE_ARRAY,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        noOptions); // Must EXPLICITLY be the empty version of the datatype, NULL/nullptr causes failure

    uint8_t noDescription[] = {0};
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_DESCRIPTION_ID,
        ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        noDescription);

    uint16_t numberOfStates = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_NUMBER_OF_STATES_ID,
        ESP_ZB_ZCL_ATTR_TYPE_U16,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &numberOfStates);

    bool outOfService = false;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_OUT_OF_SERVICE_ID,
        ESP_ZB_ZCL_ATTR_TYPE_BOOL,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &outOfService);

    uint16_t presentValue = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_PRESENT_VALUE_ID,
        ESP_ZB_ZCL_ATTR_TYPE_U16,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &presentValue);

    uint8_t statusFlags = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATUS_FLAGS_ID,
        ESP_ZB_ZCL_ATTR_TYPE_8BITMAP,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &statusFlags);

    return customCluster;
}

esp_zb_attribute_list_t* AbominableZigbeeMultistateValue::_getCluster(uint16_t clusterId) {
    return esp_zb_cluster_list_get_cluster(_cluster_list, clusterId, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}
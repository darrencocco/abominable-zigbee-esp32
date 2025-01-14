//
// Created by dcocco on 14/01/25.
//

#include "AbominableZigbeeMultistateBase.h"

#include <cstring>
#include <esp_zigbee_attribute.h>
#include <esp_zigbee_core.h>
#include <stdexcept>

#include "../data/Marshal.h"

AbominableZigbeeMultistateBase::AbominableZigbeeMultistateBase(uint8_t endpoint) : AbominableZigbeeEP(endpoint) {}

AbominableZigbeeMultistateBase::~AbominableZigbeeMultistateBase() {}

void AbominableZigbeeMultistateBase::setDescription(char *desc) {
    if (strlen(desc) > AZB_MAX_STRING_LEN) {
        throw std::invalid_argument("ZigbeeMultistateBase::Description string is too long, must be 16 characters or less.");
    }
    if (_description == NULL) {
        _description = (char *)malloc(sizeof(char) * (AZB_MAX_STRING_LEN + 1));
    }
    strcpy(_description, desc);
    uint8_t* newDescription = Marshal::String::toZigbeeString(_description, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(_clusterId),
       _descriptionId, newDescription);
    log_d("Updating %02X: %s", _descriptionId, esp_err_to_name(result));
}

void AbominableZigbeeMultistateBase::setOptions(const char **options, const uint16_t length) {
    size_t biggestString = 0;
    for (int i = 0; i < length; i++) {
        biggestString = std::max(strlen(options[i]), biggestString);
    }
    if (biggestString > AZB_MAX_STRING_LEN) {
        // At least one of the options passed was too long.
        throw std::invalid_argument("ZigbeeMultistateBase::setOptions no option can be more than 16 characters in length");
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

    esp_zb_attribute_list_t *multistateValueCluster = _getCluster(_clusterId);
    esp_err_t result = esp_zb_cluster_update_attr(multistateValueCluster, _stateTextId, optionsPacked);
    log_d("Updating %02X: %s", _stateTextId, esp_err_to_name(result));
    result = esp_zb_cluster_update_attr(multistateValueCluster, _numberOfStatesId, &_numberOfOptions);
    log_d("Updating %02X: %s", _numberOfStatesId, esp_err_to_name(result));
    result = esp_zb_cluster_update_attr(multistateValueCluster, _presentValueId, &_currentSelection);
    log_d("Updating %02X: %s", _presentValueId, esp_err_to_name(result));
}

void AbominableZigbeeMultistateBase::setNumberOfOptions(uint16_t count) {
    _numberOfOptions = count;
    uint16_t numberOfOptions = _numberOfOptions;
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _numberOfStatesId, &numberOfOptions);
    free(&numberOfOptions);
    log_d("Updating %02X: %s", _numberOfStatesId, esp_err_to_name(result));
}

void AbominableZigbeeMultistateBase::setSelection(const uint16_t selection) {
    if (selection > _numberOfOptions) {
        throw std::invalid_argument("ZigbeeMultistateBase::setSelection out of range");
    }
    _currentSelection = selection;
    uint16_t currentSelection = _currentSelection;
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _presentValueId, &currentSelection);
    free(&currentSelection);
    log_d("Updating %02X: %s", _presentValueId, esp_err_to_name(result));
}

char *AbominableZigbeeMultistateBase::getDescription() {
    return _description;
}

char ** AbominableZigbeeMultistateBase::getOptions() {
    return _options;
}

uint16_t AbominableZigbeeMultistateBase::getNumberOfOptions() {
    return _numberOfOptions;
}

uint16_t AbominableZigbeeMultistateBase::getSelection() {
    return _currentSelection;
}

uint8_t AbominableZigbeeMultistateBase::getStatusFlags() {
    return AZB_ZCL_STATUS_FLAGS_NORMAL
        + _flagInAlarm * AZB_ZCL_STATUS_FLAGS_IN_ALARM
        + _flagFault * AZB_ZCL_STATUS_FLAGS_FAULT
        + _flagOverriden * AZB_ZCL_STATUS_FLAGS_OVERRIDDEN
        + _flagOutOfService * AZB_ZCL_STATUS_FLAGS_OUT_OF_SERVICE;
}

bool AbominableZigbeeMultistateBase::isOutOfService() {
    return _flagOutOfService;
}

void AbominableZigbeeMultistateBase::setReporting(uint16_t minInterval, uint16_t maxInterval) {
    // clang-format off
    esp_zb_zcl_reporting_info_t reportingInfo = {
        .direction = ESP_ZB_ZCL_CMD_DIRECTION_TO_SRV,
        .ep = _endpoint,
        .cluster_id = _clusterId,
        .cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        .attr_id = _presentValueId,
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

void AbominableZigbeeMultistateBase::_updateFlags() {
    uint8_t flags = getStatusFlags();
    esp_err_t result = esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _statusFlagsId, &flags);
    log_d("Updating %02X: %s", _statusFlagsId, esp_err_to_name(result));
}

esp_zb_attribute_list_t * AbominableZigbeeMultistateBase::_createCustomClusterDefinition() {
    esp_zb_attribute_list_t *customCluster = esp_zb_zcl_attr_list_create(_clusterId);

    uint8_t noOptions[] = {0, 0};
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _stateTextId,
        ESP_ZB_ZCL_ATTR_TYPE_ARRAY,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        noOptions); // Must EXPLICITLY be the empty version of the datatype, NULL/nullptr causes failure

    uint8_t noDescription[] = {0};
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _descriptionId,
        ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        noDescription);

    uint16_t numberOfStates = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _numberOfStatesId,
        ESP_ZB_ZCL_ATTR_TYPE_U16,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &numberOfStates);

    bool outOfService = false;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _outOfServiceId,
        ESP_ZB_ZCL_ATTR_TYPE_BOOL,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &outOfService);

    uint16_t presentValue = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _presentValueId,
        ESP_ZB_ZCL_ATTR_TYPE_U16,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &presentValue);

    uint8_t statusFlags = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _statusFlagsId,
        ESP_ZB_ZCL_ATTR_TYPE_8BITMAP,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_ONLY | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &statusFlags);

    return customCluster;
}
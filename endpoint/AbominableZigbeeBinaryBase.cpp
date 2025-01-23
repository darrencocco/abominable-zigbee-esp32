//
// Created by dcocco on 14/01/25.
//

#include "AbominableZigbeeBinaryBase.h"

#include <cstring>
#include <esp_zigbee_attribute.h>
#include <esp_zigbee_core.h>
#include <stdexcept>
#include <zcl/esp_zigbee_zcl_command.h>

#include "../data/Marshal.h";

AbominableZigbeeBinaryBase::AbominableZigbeeBinaryBase(uint8_t endpoint) : AbominableZigbeeEP(endpoint) {}

AbominableZigbeeBinaryBase::~AbominableZigbeeBinaryBase() {}

void AbominableZigbeeBinaryBase::presetDescription(char *description) {
    if (strlen(description) > AZB_MAX_STRING_LEN) {
        throw std::invalid_argument("ZigbeeBinaryBase::Description string is too long, must be 16 characters or less.");
    }
    if (_description == nullptr) {
        _description = (char *)malloc(sizeof(char) * (AZB_MAX_STRING_LEN + 1));
    }
    strcpy(_description, description);
    uint8_t* newDescription = Marshal::String::toZigbeeString(_description, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
       _descriptionId, newDescription);
}

void AbominableZigbeeBinaryBase::presetTrueText(char *string) {
    if (strlen(string) > AZB_MAX_STRING_LEN) {
        throw std::invalid_argument("ZigbeeBinaryBase::ActiveText string is too long, must be 16 characters or less.");
    }
    if (_activeText == nullptr) {
        _activeText = (char *)malloc(sizeof(char) * (AZB_MAX_STRING_LEN + 1));
    }
    strcpy(_activeText, string);
    uint8_t* newActiveText = Marshal::String::toZigbeeString(_activeText, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
       _activeTextId, newActiveText);
}

void AbominableZigbeeBinaryBase::presetFalseText(char *string) {
    if (strlen(string) > AZB_MAX_STRING_LEN) {
        throw std::invalid_argument("ZigbeeBinaryBase::ActiveText string is too long, must be 16 characters or less.");
    }
    if (_inactiveText == nullptr) {
        _inactiveText = (char *)malloc(sizeof(char) * (AZB_MAX_STRING_LEN + 1));
    }
    strcpy(_inactiveText, string);
    uint8_t* newInactiveText = Marshal::String::toZigbeeString(_inactiveText, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
       _inactiveTextId, newInactiveText);
}

void AbominableZigbeeBinaryBase::presetValue(bool value) {
    _presentValue = value;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _presentValueId, &_presentValue);
}

void AbominableZigbeeBinaryBase::presetOutOfService(bool outOfService) {
    _outOfService = outOfService;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _outOfServiceId, &_outOfService);
}

void AbominableZigbeeBinaryBase::presetStatusFlags(uint8_t flags) {
    _statusFlags = flags;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _statusFlagsId, &_statusFlags);
}

void AbominableZigbeeBinaryBase::setValue(bool value) {
    _presentValue = value;
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_set_attribute_val(_endpoint, _clusterId, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        _presentValueId, &_presentValue, false);
    esp_zb_lock_release();
    reportValue();
}

void AbominableZigbeeBinaryBase::setReporting(uint16_t minInterval, uint16_t maxInterval) {
    // clang-format off
    esp_zb_zcl_reporting_info_t reportingInfo = {
        .direction = ESP_ZB_ZCL_REPORT_DIRECTION_SEND,
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
                .def_min_interval = minInterval,
                .def_max_interval = maxInterval,
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

char * AbominableZigbeeBinaryBase::getDescription() {
    return _description;
}

char * AbominableZigbeeBinaryBase::getTrueText() {
    return _activeText;
}

char * AbominableZigbeeBinaryBase::getFalseText() {
    return _inactiveText;
}

bool AbominableZigbeeBinaryBase::getValue() {
    return _presentValue;
}

uint8_t AbominableZigbeeBinaryBase::getStatusFlags() {
    return _statusFlags;
}

bool AbominableZigbeeBinaryBase::isOutOfService() {
    return _outOfService;
}

void AbominableZigbeeBinaryBase::reportValue() {
    esp_zb_zcl_report_attr_cmd_t report_attr_cmd;
    report_attr_cmd.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
    report_attr_cmd.attributeID = _presentValueId;
    report_attr_cmd.direction = ESP_ZB_ZCL_CMD_DIRECTION_TO_CLI;
    report_attr_cmd.clusterID = _clusterId;
    report_attr_cmd.zcl_basic_cmd.src_endpoint = _endpoint;

    bool lockAcquired = false;
    do {
        delay(100);
        lockAcquired = esp_zb_lock_acquire(portMAX_DELAY);
    } while (!lockAcquired);
    esp_zb_zcl_report_attr_cmd_req(&report_attr_cmd);
    esp_zb_lock_release();
}

esp_zb_attribute_list_t * AbominableZigbeeBinaryBase::_createCustomClusterDefinition() {
    esp_zb_attribute_list_t *customCluster = esp_zb_zcl_attr_list_create(_clusterId);

    uint8_t emptyString[] = {0};
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _activeTextId,
        ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        emptyString);

    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _descriptionId,
        ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        emptyString);

    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _inactiveTextId,
        ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        emptyString);

    bool defaultOutOfService = false;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _outOfServiceId,
        ESP_ZB_ZCL_ATTR_TYPE_BOOL,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &defaultOutOfService);

    bool defaultPresentValue = false;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _presentValueId,
        ESP_ZB_ZCL_ATTR_TYPE_BOOL,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &defaultPresentValue);

    uint8_t statusFlags = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _statusFlagsId,
        ESP_ZB_ZCL_ATTR_TYPE_8BITMAP,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_ONLY | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &statusFlags);

    return customCluster;
}

void AbominableZigbeeBinaryBase::zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) {
    AbominableZigbeeEP::zbAttributeSet(message);
    if (message->info.cluster == _clusterId) {
        if (message->attribute.id == _presentValueId) {
            bool value = *(bool*)message->attribute.data.value;
            _presentValue = value;
            _onValueSet(value);
            reportValue();
        } else if (message->attribute.id == _outOfServiceId) {

        } else if (message->attribute.id == _descriptionId) {

        } else {

        }
    }
}

//
// Created by dcocco on 14/01/25.
//

#include "AbominableZigbeeBinaryBase.h"

#include <cstring>
#include <esp_zigbee_attribute.h>
#include <stdexcept>

#include "../data/Marshal.h";

AbominableZigbeeBinaryBase::AbominableZigbeeBinaryBase(uint8_t endpoint) : AbominableZigbeeEP(endpoint) {}

void AbominableZigbeeBinaryBase::setDescription(char *description) {
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

void AbominableZigbeeBinaryBase::setTrueText(char *string) {
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

void AbominableZigbeeBinaryBase::setFalseText(char *string) {
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

void AbominableZigbeeBinaryBase::setValue(bool value) {
    _presentValue = value;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _presentValueId, &_presentValue);
}

void AbominableZigbeeBinaryBase::setOutOfService(bool outOfService) {
    _outOfService = outOfService;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _outOfServiceId, &_outOfService);
}

void AbominableZigbeeBinaryBase::setStatusFlags(uint8_t flags) {
    _statusFlags = flags;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _statusFlagsId, &_statusFlags);
}

void AbominableZigbeeBinaryBase::setReporting(uint16_t minInterval, uint16_t maxInterval) {
    // TODO: Implement me.
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

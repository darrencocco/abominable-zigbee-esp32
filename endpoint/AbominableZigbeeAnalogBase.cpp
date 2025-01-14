//
// Created by dcocco on 14/01/25.
//

#include "AbominableZigbeeAnalogBase.h"
#include "../data/Marshal.h"

#include <cstring>
#include <esp_zigbee_attribute.h>
#include <esp_zigbee_cluster.h>
#include <esp_zigbee_core.h>
#include <stdexcept>

AbominableZigbeeAnalogBase::AbominableZigbeeAnalogBase(uint8_t endpoint) : AbominableZigbeeEP(endpoint) {}

AbominableZigbeeAnalogBase::~AbominableZigbeeAnalogBase() {
}

void AbominableZigbeeAnalogBase::setDeviceType(uint16_t deviceType) {
    _device_id = (esp_zb_ha_standard_devices_t)deviceType;
    _ep_config.app_device_id = (esp_zb_ha_standard_devices_t)deviceType;
}

void AbominableZigbeeAnalogBase::setDescription(char *description) {
    if (strlen(description) > AZB_MAX_STRING_LEN) {
        throw std::invalid_argument("ZigbeeAnalogOutput::Description length is too long, must be 16 characters or less.");
    }
    if (_description == NULL) {
        _description = (char *)malloc(sizeof(char) * (AZB_MAX_STRING_LEN + 1));
    }
    strcpy(_description, description);
    uint8_t* newDescription = Marshal::String::toZigbeeString(_description, ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING);
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
       _descriptionId, newDescription);
}

void AbominableZigbeeAnalogBase::setMin(float_t min) {
    _min = min;
    _minSet = true;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _minPresentValueId, &_min);
}

void AbominableZigbeeAnalogBase::setMax(float_t max) {
    _max = max;
    _maxSet = true;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _maxPresentValueId, &_max);
}

void AbominableZigbeeAnalogBase::setValue(float_t value) {
    _value = value;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _presentValueId, &_value);
}

void AbominableZigbeeAnalogBase::setResolution(float_t resolution) {
    _resolution = resolution;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _resolutionId, &_resolution);
}

void AbominableZigbeeAnalogBase::setUnitType(uint16_t unitTypeId) {
    _unitTypeId = unitTypeId;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _engineeringUnitsId, &_unitTypeId);
}

void AbominableZigbeeAnalogBase::setStatusFlags(uint8_t flags) {
    _statusFlags = flags;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _statusFlagsId, &_statusFlags);
}

void AbominableZigbeeAnalogBase::setOutOfService(bool outOfService) {
    _outOfService = outOfService;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _outOfServiceId, &_outOfService);
}

void AbominableZigbeeAnalogBase::setReporting(uint16_t minInterval, uint16_t maxInterval, float_t delta) {
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
                    .u16 = (uint16_t)(delta * 100), // TODO: is this right, the property that is changing is a single(float)
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
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_update_reporting_info(&reportingInfo);
    esp_zb_lock_release();
}

char * AbominableZigbeeAnalogBase::getDescription() {
}

float_t AbominableZigbeeAnalogBase::getMin() {
}

float_t AbominableZigbeeAnalogBase::getMax() {
}

float_t AbominableZigbeeAnalogBase::getValue() {
}

float_t AbominableZigbeeAnalogBase::getResolution() {
}

uint16_t AbominableZigbeeAnalogBase::getUnitType() {
}

uint8_t AbominableZigbeeAnalogBase::getStatusFlags() {
}

bool AbominableZigbeeAnalogBase::isOutOfService() {
}

esp_zb_attribute_list_t * AbominableZigbeeAnalogBase::_createCustomClusterDefinition() {
    esp_zb_attribute_list_t *customCluster = esp_zb_zcl_attr_list_create(_clusterId);

    uint8_t noDescription[] = {0};
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _descriptionId,
        ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        noDescription);

    float_t undefinedSingle = 0xFF;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _maxPresentValueId,
        ESP_ZB_ZCL_ATTR_TYPE_SINGLE,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &undefinedSingle);

    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _minPresentValueId,
        ESP_ZB_ZCL_ATTR_TYPE_SINGLE,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &undefinedSingle);

    bool outOfService = false;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _outOfServiceId,
        ESP_ZB_ZCL_ATTR_TYPE_BOOL,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &outOfService);

    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _presentValueId,
        ESP_ZB_ZCL_ATTR_TYPE_SINGLE,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &undefinedSingle);

    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _resolutionId,
        ESP_ZB_ZCL_ATTR_TYPE_SINGLE,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &undefinedSingle);

    uint8_t statusFlags = 0;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _statusFlagsId,
        ESP_ZB_ZCL_ATTR_TYPE_8BITMAP,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_ONLY | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &statusFlags);

    uint16_t undefinedType = 0x00FF;
    esp_zb_custom_cluster_add_custom_attr(customCluster,
        _engineeringUnitsId,
        ESP_ZB_ZCL_ATTR_TYPE_U16,
        ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE | ESP_ZB_ZCL_ATTR_ACCESS_REPORTING,
        &undefinedType);

    return customCluster;
}
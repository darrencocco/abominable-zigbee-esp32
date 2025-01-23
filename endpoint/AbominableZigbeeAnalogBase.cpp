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

void AbominableZigbeeAnalogBase::presetDescription(char *description) {
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

void AbominableZigbeeAnalogBase::presetMin(float_t min) {
    _min = min;
    _minSet = true;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _minPresentValueId, &_min);
}

void AbominableZigbeeAnalogBase::presetMax(float_t max) {
    _max = max;
    _maxSet = true;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _maxPresentValueId, &_max);
}

void AbominableZigbeeAnalogBase::presetValue(float_t value) {
    _value = value;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _presentValueId, &_value);
}

void AbominableZigbeeAnalogBase::presetResolution(float_t resolution) {
    _resolution = resolution;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _resolutionId, &_resolution);
}

void AbominableZigbeeAnalogBase::presetUnitType(uint16_t unitTypeId) {
    _unitTypeId = unitTypeId;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _engineeringUnitsId, &_unitTypeId);
}

void AbominableZigbeeAnalogBase::presetStatusFlags(uint8_t flags) {
    _statusFlags = flags;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _statusFlagsId, &_statusFlags);
}

void AbominableZigbeeAnalogBase::presetOutOfService(bool outOfService) {
    _outOfService = outOfService;
    esp_zb_cluster_update_attr(_getCluster(_clusterId),
        _outOfServiceId, &_outOfService);
}

void AbominableZigbeeAnalogBase::setValue(float_t value) {
    _value = value;
    esp_zb_lock_acquire(portMAX_DELAY);
    esp_zb_zcl_set_attribute_val(_endpoint, _clusterId, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,
        _presentValueId, &_value, false);
    esp_zb_lock_release();
    reportValue();
}

void AbominableZigbeeAnalogBase::setReporting(uint16_t minInterval, uint16_t maxInterval, float_t delta) {
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
                    .u16 = (uint16_t)(delta * 100), // TODO: is this right, the property that is changing is a single(float)
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
    return _value;
}

float_t AbominableZigbeeAnalogBase::getResolution() {
}

uint16_t AbominableZigbeeAnalogBase::getUnitType() {
}

uint8_t AbominableZigbeeAnalogBase::getStatusFlags() {
}

bool AbominableZigbeeAnalogBase::isOutOfService() {
}

void AbominableZigbeeAnalogBase::reportValue() {
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

void AbominableZigbeeAnalogBase::zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) {
    AbominableZigbeeEP::zbAttributeSet(message);
    if (message->info.cluster == _clusterId) {
        if (message->attribute.id == _presentValueId) {
            float_t value = *(float_t*)message->attribute.data.value;
            if ((_maxSet && value > _max) || (_minSet && value < _min) ) {
                setValue(_value);
            } else {
                _value = value;
                _onValueSet(value);
                reportValue();
            }
        } else if (message->attribute.id == _outOfServiceId) {

        } else if (message->attribute.id == _descriptionId) {

        } else {

        }
    }
}
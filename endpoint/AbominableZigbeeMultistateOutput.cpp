//
// Created by dcocco on 25/12/24.
//

#include "AbominableZigbeeMultistateOutput.h"
#include "esp_zigbee_core.h"

AbominableZigbeeMultistateOutput::AbominableZigbeeMultistateOutput(uint8_t endpoint) : AbominableZigbeeMultistateBase(endpoint) {
    _clusterId = AZB_ZCL_CLUSTER_ID_MULTI_OUTPUT;
    _stateTextId = AZB_ZCL_ATTR_MULTI_OUTPUT_STATE_TEXT_ID;
    _descriptionId = AZB_ZCL_ATTR_MULTI_OUTPUT_DESCRIPTION_ID;
    _numberOfStatesId = AZB_ZCL_ATTR_MULTI_OUTPUT_NUMBER_OF_STATES_ID;
    _outOfServiceId = AZB_ZCL_ATTR_MULTI_OUTPUT_OUT_OF_SERVICE_ID;
    _presentValueId = AZB_ZCL_ATTR_MULTI_OUTPUT_PRESENT_VALUE_ID;
    _statusFlagsId = AZB_ZCL_ATTR_MULTI_OUTPUT_STATUS_FLAGS_ID;

    _customBindingRequired = true;
    _coordinatorBound = false;

    _setup();
}

AbominableZigbeeMultistateOutput::~AbominableZigbeeMultistateOutput() {}
//
// Created by dcocco on 13/01/25.
//

#include "AbominableZigbeeMultistateInput.h"

AbominableZigbeeMultistateInput::AbominableZigbeeMultistateInput(uint8_t endpoint) : AbominableZigbeeMultistateBase(endpoint) {
    _clusterId = AZB_ZCL_CLUSTER_ID_MULTI_INPUT;
    _stateTextId = AZB_ZCL_ATTR_MULTI_INPUT_STATE_TEXT_ID;
    _descriptionId = AZB_ZCL_ATTR_MULTI_INPUT_DESCRIPTION_ID;
    _numberOfStatesId = AZB_ZCL_ATTR_MULTI_INPUT_NUMBER_OF_STATES_ID;
    _outOfServiceId = AZB_ZCL_ATTR_MULTI_INPUT_OUT_OF_SERVICE_ID;
    _presentValueId = AZB_ZCL_ATTR_MULTI_INPUT_PRESENT_VALUE_ID;
    _statusFlagsId = AZB_ZCL_ATTR_MULTI_INPUT_STATUS_FLAGS_ID;

    _customBindingRequired = true;
    _coordinatorBound = false;

    _setup();
}

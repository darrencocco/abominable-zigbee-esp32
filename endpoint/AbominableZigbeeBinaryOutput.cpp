//
// Created by dcocco on 13/01/25.
//

#include "AbominableZigbeeBinaryOutput.h"

AbominableZigbeeBinaryOutput::AbominableZigbeeBinaryOutput(uint8_t endpoint) : AbominableZigbeeBinaryBase(endpoint) {
    _clusterId = AZB_ZCL_CLUSTER_ID_BINARY_OUTPUT;
    _activeTextId = AZB_ZCL_ATTR_BINARY_OUTPUT_ACTIVE_TEXT_ID;
    _descriptionId = AZB_ZCL_ATTR_BINARY_OUTPUT_DESCRIPTION_ID;
    _inactiveTextId = AZB_ZCL_ATTR_BINARY_OUTPUT_INACTIVE_TEXT_ID;
    _outOfServiceId = AZB_ZCL_ATTR_BINARY_OUTPUT_OUT_OF_SERVICE_ID;
    _presentValueId = AZB_ZCL_ATTR_BINARY_OUTPUT_PRESENT_VALUE_ID;
    _statusFlagsId = AZB_ZCL_ATTR_BINARY_OUTPUT_STATUS_FLAGS_ID;

    _customBindingRequired = true;
    _coordinatorBound = false;

    _setup();
}

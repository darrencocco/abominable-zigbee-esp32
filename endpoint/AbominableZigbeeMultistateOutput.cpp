//
// Created by dcocco on 25/12/24.
//

#include "AbominableZigbeeMultistateOutput.h"
#include "esp_zigbee_core.h"

#define AZB_ZCL_CLUSTER_ID_MULTI_OUTPUT 0x0013

#define AZB_ZCL_ATTR_MULTI_OUTPUT_STATE_TEXT_ID 0x000E
#define AZB_ZCL_ATTR_MULTI_OUTPUT_DESCRIPTION_ID 0x001C
#define AZB_ZCL_ATTR_MULTI_OUTPUT_NUMBER_OF_STATES_ID 0x004A
#define AZB_ZCL_ATTR_MULTI_OUTPUT_OUT_OF_SERVICE_ID 0x0051
#define AZB_ZCL_ATTR_MULTI_OUTPUT_PRESENT_VALUE_ID 0x0055
#define AZB_ZCL_ATTR_MULTI_OUTPUT_STATUS_FLAGS_ID 0x006F

AbominableZigbeeMultistateOutput::AbominableZigbeeMultistateOutput(uint8_t endpoint) : AbominableZigbeeMultistateBase(endpoint) {
    _clusterId = AZB_ZCL_CLUSTER_ID_MULTI_OUTPUT;
    _stateTextId = AZB_ZCL_ATTR_MULTI_OUTPUT_STATE_TEXT_ID;
    _descriptionId = AZB_ZCL_ATTR_MULTI_OUTPUT_DESCRIPTION_ID;
    _numberOfStatesId = AZB_ZCL_ATTR_MULTI_OUTPUT_NUMBER_OF_STATES_ID;
    _presentValueId = AZB_ZCL_ATTR_MULTI_OUTPUT_OUT_OF_SERVICE_ID;
    _statusFlagsId = AZB_ZCL_ATTR_MULTI_OUTPUT_STATUS_FLAGS_ID;

    _setup();
}

AbominableZigbeeMultistateOutput::~AbominableZigbeeMultistateOutput() {}

void AbominableZigbeeMultistateOutput::zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) {
    if (message->info.cluster == AZB_ZCL_CLUSTER_ID_MULTI_OUTPUT) {
        switch (message->attribute.id) {
            case AZB_ZCL_ATTR_MULTI_OUTPUT_PRESENT_VALUE_ID:
                // _currentSelection = *((uint16_t*)message->attribute.data.value);
                // _on_value_receive(AZB_ZCL_ATTR_MULTI_OUTPUT_PRESENT_VALUE_ID);
                break;
            case AZB_ZCL_ATTR_MULTI_OUTPUT_STATE_TEXT_ID:
                break;
            case AZB_ZCL_ATTR_MULTI_OUTPUT_OUT_OF_SERVICE_ID:
            //     _flagOutOfService = *((bool*)message->attribute.data.value);
            // _on_value_receive(AZB_ZCL_ATTR_MULTI_OUTPUT_OUT_OF_SERVICE_ID);
                break;
            case AZB_ZCL_ATTR_MULTI_OUTPUT_DESCRIPTION_ID:
                break;
            default:
                break;
        }
    }
}

void AbominableZigbeeMultistateOutput::zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) {

}
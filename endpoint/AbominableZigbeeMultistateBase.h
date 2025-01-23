//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEMULTISTATEBASE_H
#define ABOMINABLEZIGBEEMULTISTATEBASE_H

#include <zcl/esp_zigbee_zcl_command.h>
#include <zdo/esp_zigbee_zdo_command.h>

#include "AbominableZigbeeEP.h"

class AbominableZigbeeMultistateBase : public AbominableZigbeeEP {
public:
    AbominableZigbeeMultistateBase(uint8_t endpoint);
    virtual ~AbominableZigbeeMultistateBase();

    void presetDescription(char* desc);
    void presetOptions(const char **options, uint16_t length);
    void presetNumberOfOptions(uint16_t count);
    void presetSelection(uint16_t selection);

    void setSelection(uint16_t selection);
    void setReporting(uint16_t minInterval, uint16_t maxInterval);

    char* getDescription();
    char** getOptions();
    uint16_t getNumberOfOptions();
    uint16_t getSelection();
    uint8_t getStatusFlags();
    bool isOutOfService();

    void onSelectionSet(void (*callback)(uint16_t)) {
        _onSelectionSet = callback;
    }

    void reportSelection();

    void fuckingBinding();

protected:
    uint16_t _stateTextId;
    uint16_t _descriptionId;
    uint16_t _numberOfStatesId;
    uint16_t _outOfServiceId;
    uint16_t _presentValueId;
    uint16_t _statusFlagsId;

    char** _options = NULL;
    uint16_t _numberOfOptions = 0;
    uint16_t _currentSelection = 0;
    bool _flagInAlarm = false;
    bool _flagFault = false;
    bool _flagOverriden = false;
    bool _flagOutOfService = false;
    char* _description = NULL;

    void _updateFlags();

    void (*_onSelectionSet)(uint16_t);

    void (*_fuckingwat)(esp_zb_zdp_status_t, esp_zb_zdo_ieee_addr_rsp_t*, void*);

    esp_zb_attribute_list_t *_createCustomClusterDefinition() override;

    void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;
};

#endif //ABOMINABLEZIGBEEMULTISTATEBASE_H
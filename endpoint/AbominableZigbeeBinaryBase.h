//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEBINARYBASE_H
#define ABOMINABLEZIGBEEBINARYBASE_H
#include <cstdint>
#include <zcl/esp_zigbee_zcl_command.h>

#include "AbominableZigbeeEP.h"

class AbominableZigbeeBinaryBase : public AbominableZigbeeEP {
public:
    AbominableZigbeeBinaryBase(uint8_t endpoint);
    virtual ~AbominableZigbeeBinaryBase();

    void presetDescription(char* description);
    void presetTrueText(char* activeText);
    void presetFalseText(char* inactiveText);
    void presetValue(bool value);
    void presetOutOfService(bool outOfService);
    void presetStatusFlags(uint8_t flags);

    void setValue(bool value);;
    void setReporting(uint16_t minInterval, uint16_t maxInterval);

    char* getDescription();
    char* getTrueText();
    char* getFalseText();
    bool getValue();
    uint8_t getStatusFlags();
    bool isOutOfService();

    void onValueSet(void (*callback)(bool)) {
        _onValueSet = callback;
    }

    void reportValue();

protected:
    char* _activeText = nullptr;
    char* _description = nullptr;
    char* _inactiveText = nullptr;
    bool _outOfService = false;
    bool _presentValue; // FIXME: Spec omits the definition but I doubt we can leave it undefined.
    uint8_t _statusFlags = 0x00;

    uint16_t _activeTextId;
    uint16_t _descriptionId;
    uint16_t _inactiveTextId;
    uint16_t _outOfServiceId;
    uint16_t _presentValueId;
    uint16_t _statusFlagsId;

    void (*_onValueSet)(bool);

    esp_zb_attribute_list_t *_createCustomClusterDefinition() override;

    void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;
};

#endif //ABOMINABLEZIGBEEBINARYBASE_H
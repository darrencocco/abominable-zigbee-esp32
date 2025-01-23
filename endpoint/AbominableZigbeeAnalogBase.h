//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEANALOGBASE_H
#define ABOMINABLEZIGBEEANALOGBASE_H

#include <cstdint>
#include <zcl/esp_zigbee_zcl_command.h>

#include "AbominableZigbeeEP.h"

class AbominableZigbeeAnalogBase : public AbominableZigbeeEP {
public:
    AbominableZigbeeAnalogBase(uint8_t endpoint);
    ~AbominableZigbeeAnalogBase();

    void presetDescription(char* description);
    void presetMin(float_t min);
    void presetMax(float_t max);
    void presetValue(float_t value);
    void presetResolution(float_t resolution);
    void presetUnitType(uint16_t unitTypeId);
    void presetStatusFlags(uint8_t flags);
    void presetOutOfService(bool outOfService);

    void setValue(float_t value);
    void setReporting(uint16_t minInterval, uint16_t maxInterval, float_t delta);

    char* getDescription();
    float_t getMin();
    float_t getMax();
    float_t getValue();
    float_t getResolution();
    uint16_t getUnitType();
    uint8_t getStatusFlags();
    bool isOutOfService();

    void onValueSet(void (*callback)(float_t)) {
        _onValueSet = callback;
    }

    void reportValue();

protected:
    char* _description = nullptr;
    float_t _min = 0.0f;
    bool _minSet = false;
    float_t _max = 0.0f;
    bool _maxSet = false;
    float_t _value = 0.0f;
    float_t _resolution = 0.0f;
    bool _resolutionSet = false;
    uint16_t _unitTypeId = 0x0000;
    uint8_t _statusFlags = 0x00;
    bool _outOfService = false;

    uint16_t _descriptionId;
    uint16_t _maxPresentValueId;
    uint16_t _minPresentValueId;
    uint16_t _outOfServiceId;
    uint16_t _presentValueId;
    uint16_t _resolutionId;
    uint16_t _statusFlagsId;
    uint16_t _engineeringUnitsId;

    void (*_onValueSet)(float_t);

    esp_zb_attribute_list_t *_createCustomClusterDefinition() override;

    void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;

};

#endif //ABOMINABLEZIGBEEANALOGBASE_H
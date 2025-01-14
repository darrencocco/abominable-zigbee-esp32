//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEANALOGBASE_H
#define ABOMINABLEZIGBEEANALOGBASE_H
#include <cstdint>
#include <list>

#include "AbominableZigbeeEP.h"

class AbominableZigbeeAnalogBase : public AbominableZigbeeEP {
public:
    AbominableZigbeeAnalogBase(uint8_t endpoint);
    ~AbominableZigbeeAnalogBase();

    void setDeviceType(uint16_t deviceType);
    void setDescription(char* description);
    void setMin(float_t min);
    void setMax(float_t max);
    void setValue(float_t value);
    void setResolution(float_t resolution);
    void setUnitType(uint16_t unitTypeId);
    void setStatusFlags(uint8_t flags);
    void setOutOfService(bool outOfService);
    void setReporting(uint16_t minInterval, uint16_t maxInterval, float_t delta);

    char* getDescription();
    float_t getMin();
    float_t getMax();
    float_t getValue();
    float_t getResolution();
    uint16_t getUnitType();
    uint8_t getStatusFlags();
    bool isOutOfService();

    void onConfigReceive(void (*callback)(uint16_t, uint16_t, std::list<char *>)) {
        _on_config_receive = callback;
    }

    void onValueReceive(void (*callback)(uint16_t)) {
        _on_value_receive = callback;
    }

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

    void (*_on_config_receive)(uint16_t, uint16_t, std::list<char *>);
    void (*_on_value_receive)(uint16_t);

    esp_zb_attribute_list_t *_createCustomClusterDefinition() override;

};

#endif //ABOMINABLEZIGBEEANALOGBASE_H
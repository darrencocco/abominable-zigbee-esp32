//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEBINARYBASE_H
#define ABOMINABLEZIGBEEBINARYBASE_H
#include <cstdint>
#include <list>

#include "AbominableZigbeeEP.h"
#include "soc/soc_caps.h"

class AbominableZigbeeBinaryBase : public AbominableZigbeeEP {
public:
    AbominableZigbeeBinaryBase(uint8_t endpoint);
    virtual ~AbominableZigbeeBinaryBase();

    void setDescription(char* description);
    void setTrueText(char* activeText);
    void setFalseText(char* inactiveText);
    void setValue(bool value);
    void setOutOfService(bool outOfService);
    void setStatusFlags(uint8_t flags);
    void setReporting(uint16_t minInterval, uint16_t maxInterval);

    char* getDescription();
    char* getTrueText();
    char* getFalseText();
    bool getValue();
    uint8_t getStatusFlags();
    bool isOutOfService();

    void onConfigReceive(void (*callback)(uint16_t, uint16_t, std::list<char *>)) {
        _on_config_receive = callback;
    }

    void onValueReceive(void (*callback)(uint16_t)) {
        _on_value_receive = callback;
    }

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

    void (*_on_config_receive)(uint16_t, uint16_t, std::list<char *>);
    void (*_on_value_receive)(uint16_t);

    esp_zb_attribute_list_t *_createCustomClusterDefinition() override;
};



#endif //ABOMINABLEZIGBEEBINARYBASE_H

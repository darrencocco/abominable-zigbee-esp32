//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEMULTISTATEBASE_H
#define ABOMINABLEZIGBEEMULTISTATEBASE_H

#include <list>
#include <zcl/esp_zigbee_zcl_command.h>

#include "AbominableZigbeeEP.h"

class AbominableZigbeeMultistateBase : public AbominableZigbeeEP {
public:
    AbominableZigbeeMultistateBase(uint8_t endpoint);
    virtual ~AbominableZigbeeMultistateBase();

    void setDescription(char* desc);
    void setOptions(const char **options, uint16_t length);
    void setNumberOfOptions(uint16_t count);
    void setSelection(uint16_t selection);

    char* getDescription();
    char** getOptions();
    uint16_t getNumberOfOptions();
    uint16_t getSelection();
    uint8_t getStatusFlags();
    bool isOutOfService();

    void onConfigReceive(void (*callback)(uint16_t, uint16_t, std::list<char *>)) {
        _on_config_receive = callback;
    }

    void onValueReceive(void (*callback)(uint16_t)) {
        _on_value_receive = callback;
    }

    void setReporting(uint16_t minInterval, uint16_t maxInterval);

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

    void (*_on_config_receive)(uint16_t, uint16_t, std::list<char *>);
    void (*_on_value_receive)(uint16_t);

    esp_zb_attribute_list_t *_createCustomClusterDefinition() override;
};



#endif //ABOMINABLEZIGBEEMULTISTATEBASE_H

//
// Created by dcocco on 25/12/24.
//

#ifndef ZIGBEEMULTISTATEVALUE_H
#define ZIGBEEMULTISTATEVALUE_H

#include "AbominableZigbeeMultistateBase.h"

class AbominableZigbeeMultistateOutput : public AbominableZigbeeMultistateBase {
public:
  AbominableZigbeeMultistateOutput(uint8_t endpoint);
  ~AbominableZigbeeMultistateOutput();

protected:
  void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;
  void zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) override;
};

#endif //ZIGBEEMULTISTATEVALUE_H
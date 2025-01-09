//
// Created by dcocco on 25/12/24.
//

#ifndef ZIGBEEMULTISTATEVALUE_H
#define ZIGBEEMULTISTATEVALUE_H

#include "soc/soc_caps.h"

#include "ZigbeeEP.h"
#include "ha/esp_zigbee_ha_standard.h"

#include <list>

#define ESP_ZB_ZCL_ATTR_MULTI_VALUE_STATE_TEXT_ID 0x000EU
#define ESP_ZB_ZCL_ATTR_MULTI_VALUE_NUMBER_OF_STATES_ID 0x004AU

#define AZB_MAX_STRING_LEN 16

typedef struct azb_zigbee_multistate_value_cfg_s {
  esp_zb_basic_cluster_cfg_t basic_cfg;
  esp_zb_identify_cluster_cfg_t identify_cfg;
  esp_zb_multistate_value_cluster_cfg_t multistate_value_cfg;
} azb_zigbee_multistate_value_cfg_t;

#define AZB_ZIGBEE_DEFAULT_MULTISTATE_VALUE_CONFIG()                      \
  {                                                                       \
    .basic_cfg =                                                          \
      {                                                                   \
        .zcl_version = ESP_ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,        \
        .power_source = ESP_ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE,      \
      },                                                                  \
    .identify_cfg =                                                       \
      {                                                                   \
        .identify_time = ESP_ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE, \
      },                                                                  \
  }
// clang-format on

class AbominableZigbeeMultistateValue : public ZigbeeEP {
public:
  AbominableZigbeeMultistateValue(uint8_t endpoint);
  ~AbominableZigbeeMultistateValue();

  void setDeviceType(uint16_t deviceType);
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

private:
  char** _options = NULL;
  uint16_t _numberOfOptions = 0;
  uint16_t _currentSelection = 0;
  bool _flagInAlarm = false;
  bool _flagFault = false;
  bool _flagOverriden = false;
  bool _flagOutOfService = false;
  char* _description = NULL;

  void (*_on_config_receive)(uint16_t, uint16_t, std::list<char *>);
  void (*_on_value_receive)(uint16_t);

  void zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) override;
  void zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute) override;

  void _updateFlags();
  esp_zb_cluster_list_t* _generateClusterList(azb_zigbee_multistate_value_cfg_t *endpointClusterConfig);

  esp_zb_attribute_list_t *_createCustomClusterDefinition();

  esp_zb_attribute_list_t *_getCluster(uint16_t clusterId);
};

#endif //ZIGBEEMULTISTATEVALUE_H
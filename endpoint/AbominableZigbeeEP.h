//
// Created by dcocco on 14/01/25.
//

#ifndef ABOMINABLEZIGBEEEP_H
#define ABOMINABLEZIGBEEEP_H
#include <ZigbeeEP.h>

#include "Common.h"

#include "soc/soc_caps.h"

class AbominableZigbeeEP : public ZigbeeEP {
public:
    void setDeviceType(uint16_t deviceType);
protected:
    AbominableZigbeeEP(uint8_t endpoint);

    uint16_t _clusterId;

    virtual void _setup();

    esp_zb_cluster_list_t* _generateClusterList(azb_zigbee_base_cfg_t *endpointClusterConfig);
    virtual void _addClusterSpecificCluster(esp_zb_cluster_list_t* clusterList);
    virtual esp_zb_attribute_list_t *_createCustomClusterDefinition() = 0;
    esp_zb_attribute_list_t *_getCluster(uint16_t clusterId);
};

#endif //ABOMINABLEZIGBEEEP_H

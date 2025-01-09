//
// Created by dcocco on 2/01/25.
//

#include "Marshal.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "zcl/esp_zigbee_zcl_common.h"

char * Marshal::String::toCString(void *data, uint16_t type) {
    uint16_t stringLength;
    uint8_t lengthSize;
    switch (type) {
        case ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING:
            uint8_t interim;
            memcpy(&interim, data, sizeof(uint8_t));
            stringLength = interim;
            lengthSize = 1;
            break;
        case ESP_ZB_ZCL_ATTR_TYPE_LONG_CHAR_STRING:
            memcpy(&stringLength, data, sizeof(uint16_t));
            lengthSize = 2;
            break;
        default:
            throw std::invalid_argument("Unsupported zcl attribute type");
    }
    char * output = (char*)malloc(sizeof(char) * (stringLength + 1));
    memcpy(output, data+lengthSize, stringLength);
    output[stringLength+1] = '\0';
    return output;
}

uint8_t * Marshal::String::toZigbeeString(void *data, uint16_t type) {
    uint32_t stringLength;
    uint8_t lengthSize;
    switch (type) {
        case ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING:
            stringLength = strlen((char*)data);
            lengthSize = 1;
            if (stringLength > 0xFFU) {
                throw std::invalid_argument("String too long of for CHAR_STRING");
            }
            break;
        case ESP_ZB_ZCL_ATTR_TYPE_LONG_CHAR_STRING:
            stringLength = strlen((char*)data);
            lengthSize = 2;
            if (stringLength > 0xFFFFU) {
                throw std::invalid_argument("String too long of for LONG_CHAR_STRING");
            }
            break;
        default:
            throw std::invalid_argument("Unsupported zcl attribute type");
    }
    uint8_t* output = (uint8_t*)malloc(sizeof(uint8_t) * (stringLength + lengthSize));
    if (lengthSize == 1) {
        uint8_t arrayLength = stringLength;
        memcpy(output, &arrayLength, sizeof(uint8_t));
    } else {
        uint16_t arrayLength = stringLength;
        memcpy(output, &arrayLength, sizeof(uint16_t));
    }
    memcpy(output + lengthSize, data, stringLength);
    return output;
}


uint8_t * Marshal::Array::toZigbeeArray(void *data, uint16_t length, uint16_t type) {
    switch (type) {
        case ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING:
            return _toZigbeeArrayString(data, length);
        case ESP_ZB_ZCL_ATTR_TYPE_LONG_CHAR_STRING:
            return _toZigbeeArrayStringLong(data, length);
        default:
            throw std::invalid_argument("Unsupported zcl attribute type");
    }
}

uint8_t * Marshal::Array::_toZigbeeArrayString(void *data, uint16_t length) {
    // Count how much space we need and allocate it.
    uint16_t overallLength = 2;
    char** strings = (char**)data;
    uint16_t lengths[length];
    uint16_t longestStringLength = 0;
    for (uint16_t i = 0; i < length; i++) {
        char* string = strings[i];
        lengths[i] = strlen(string);
        longestStringLength = std::max(longestStringLength, lengths[i]);
        overallLength += lengths[i] + 1;
    }
    if (longestStringLength > 0xFFU) {
        throw std::invalid_argument("One or more of the strings are too long");
    }
    if (overallLength > 0xFFFFU) {
        throw std::invalid_argument("The overall packed size of the strings exceeds the max length of the array");
    }

    uint8_t* output = (uint8_t*)malloc(sizeof(uint8_t) * overallLength);

    // Copy the overall length into the first two bytes as per the standard.
    memcpy(output, &overallLength, sizeof(overallLength));
    // Start packing the strings (without the null terminator)
    uint16_t offset = 2;
    for (uint16_t i = 0; i < length; i++) {
        char* string = strings[i];
        uint8_t stringLength = (uint8_t)lengths[i];
        memcpy(output + offset, &stringLength, sizeof(stringLength));
        offset++;
        memcpy(output + offset, string, stringLength);
        offset += stringLength;
    }

    return output;
}

uint8_t * Marshal::Array::_toZigbeeArrayStringLong(void *data, uint16_t length) {
    // Count how much space we need and allocate it.
    uint16_t overallLength = 2;
    char** strings = (char**)data;
    uint32_t lengths[length];
    uint32_t longestStringLength = 0;
    for (uint16_t i = 0; i < length; i++) {
        char* string = strings[i];
        lengths[i] = strlen(string);
        longestStringLength = std::max(longestStringLength, lengths[i]);
        overallLength += lengths[i] + 1;
    }
    if (longestStringLength > 0xFFFFU) {
        throw std::invalid_argument("One or more of the strings are too long");
    }
    if (overallLength > 0xFFFFU) {
        throw std::invalid_argument("The overall packed size of the strings exceeds the max length of the array");
    }

    uint8_t* output = (uint8_t*)malloc(sizeof(uint8_t) * overallLength);

    // Copy the overall length into the first two bytes as per the standard.
    memcpy(output, &overallLength, sizeof(overallLength));
    // Start packing the strings (without the null terminator)
    uint16_t offset = 2;
    for (uint16_t i = 0; i < length; i++) {
        char* string = strings[i];
        uint16_t stringLength = (uint16_t)lengths[i];
        memcpy(output + offset, &stringLength, sizeof(stringLength));
        offset += 2;
        memcpy(output + offset, string, stringLength);
        offset += stringLength;
    }

    return output;
}

void * Marshal::Array::toCArray(void *data, uint16_t type) {
    switch (type) {
        case ESP_ZB_ZCL_ATTR_TYPE_CHAR_STRING:
            return _toCArrayString(data, false);
        case ESP_ZB_ZCL_ATTR_TYPE_LONG_CHAR_STRING:
            return _toCArrayString(data, true);
        default:
            throw std::invalid_argument("Unsupported zcl attribute type");
    }
}

void * Marshal::Array::_toCArrayString(void *data, bool isLong) {
    uint16_t length;
    uint16_t offset = sizeof(length);
    memcpy(&length, data, offset);
    uint16_t initialSize = isLong ? length/3 : length/2;
    char** output = (char**)malloc(sizeof(char*) * initialSize);
    uint16_t outputIndex = 0;
    while (offset < length) {
        uint16_t stringLength;
        if (isLong) {
            memcpy(&stringLength, data + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
        } else {
            uint8_t tempLength;
            memcpy(&tempLength, data + offset, sizeof(uint8_t));
            stringLength = tempLength;
            offset += sizeof(uint8_t);
        }
        if (offset + stringLength > length) {
            throw std::out_of_range("String too long");
        }
        char* newString = (char*)malloc(sizeof(char) * (stringLength + 1));
        memcpy(newString, data + offset, stringLength);
        *(newString + stringLength) = '\0';
        *(output + outputIndex) = newString;
        offset += stringLength;
        outputIndex++;
    }
    // FIXME: Lots of memory needs to be freed here.
    return realloc(output, sizeof(char*) * (outputIndex + 1));
}

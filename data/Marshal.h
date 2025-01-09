//
// Created by dcocco on 2/01/25.
//

#ifndef MARSHAL_H
#define MARSHAL_H
#include <cstdint>

class Marshal {
    public:
    class String {
    public:
        static char* toCString(void* data, uint16_t type);
        static uint8_t* toZigbeeString(void* data, uint16_t type);
    private:
        String();
        ~String();
    };
    class Array {
    public:
        static uint8_t* toZigbeeArray(void* data, uint16_t length, uint16_t type);
        static void* toCArray(void* data, uint16_t type);



    private:
        static uint8_t * _toZigbeeArrayString(void *data, uint16_t length);
        static uint8_t * _toZigbeeArrayStringLong(void * data, uint16_t length);
        static void *_toCArrayString(void *data, bool isLong);
        Array();
        ~Array();
    };

    private:
    Marshal();
    ~Marshal();
};
#endif //MARSHAL_H

#include "I2CHapticDxe.h"

UINT8 dw_wav1_data[] = {
    0x00, 0x01, 0x02, 0x7C, 0x00, 0x8D, 0x4B, /* header info */
    0x00, 0x06, 0x0B, 0x11, 0x16, 0x1C, 0x22, 0x27, 0x2C, 0x32, 0x37, 0x3C, 0x41, 0x45, 0x4A, 0x4F, 0x53, 0x57, 0x5B, 0x5F,
    0x63, 0x66, 0x69, 0x6C, 0x6F, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7B, 0x7D, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7D,
    0x7C, 0x7B, 0x79, 0x78, 0x76, 0x73, 0x71, 0x6E, 0x6B, 0x68, 0x65, 0x61, 0x5D, 0x5A, 0x55, 0x51, 0x4D, 0x48, 0x43, 0x3F,
    0x3A, 0x35, 0x2F, 0x2A, 0x25, 0x1F, 0x1A, 0x14, 0x0F, 0x09, 0x03, 0xFE, 0xF8, 0xF2, 0xED, 0xE7, 0xE2, 0xDC, 0xD7, 0xD2,
    0xCC, 0xC7, 0xC2, 0xBD, 0xB9, 0xB4, 0xB0, 0xAB, 0xA7, 0xA3, 0x9F, 0x9C, 0x98, 0x95, 0x92, 0x90, 0x8D, 0x8B, 0x89, 0x87,
    0x85, 0x84, 0x83, 0x82, 0x81, 0x81, 0x81, 0x81, 0x82, 0x82, 0x83, 0x84, 0x86, 0x87, 0x89, 0x8B, 0x8E, 0x90, 0x93, 0x96,
    0x99, 0x9D, 0xA0, 0xA4, 0xA8, 0xAC, 0xB1, 0xB5, 0xBA, 0xBE, 0xC3, 0xC8, 0xCE, 0xD3, 0xD8, 0xDE, 0xE3, 0xE9, 0xEE, 0xF4,
    0xF9};

UINT32 dw_ram_wav1_len = sizeof(dw_wav1_data) - 7;

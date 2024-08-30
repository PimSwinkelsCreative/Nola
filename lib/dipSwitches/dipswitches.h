#pragma once
#include <Arduino.h>

#define EXPANDER_I2CADDR 0x20

void setupDipswitches();

uint8_t getAddrress();

uint8_t getConfig();
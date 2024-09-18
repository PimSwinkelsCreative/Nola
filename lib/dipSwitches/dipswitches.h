#pragma once
#include <Arduino.h>

#define EXPANDER_I2CADDR 0x20

void setupDipswitches();

uint8_t getAddress();

uint8_t getConfig();
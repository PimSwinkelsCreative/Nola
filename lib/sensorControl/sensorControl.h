#pragma once
#include <Arduino.h>

#define PIR_STARTUP_TIME 10000

void setupSensors(void (*callbackFunction)());

bool readPIRSensor();
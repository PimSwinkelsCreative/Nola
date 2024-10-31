#include "sensorControl.h"

#include "pinout.h"

void setupSensors(void (*callbackFunction)()) {
  pinMode(SPARE_GPIO_1, INPUT_PULLDOWN);
  attachInterrupt(SPARE_GPIO_1, callbackFunction, RISING);
}

bool readPIRSensor() { return (digitalRead(SPARE_GPIO_1)); }
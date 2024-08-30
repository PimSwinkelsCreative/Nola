#include <dipswitches.h>

#include "pinout.h"
#include <Wire.h>
#include <clsPCA9555.h>

PCA9555 dipswitches(EXPANDER_I2CADDR,-1,I2C_SCL,I2C_SDA);

uint16_t dipswitchBuffer = 0;

void setupDipswitches()
{
    dipswitches.begin();
    dipswitches.setClock(400000);
    for (int i = 0; i < 16; i++) {
        dipswitches.pinMode(i, INPUT);
    }
}

uint8_t getAddrress()
{
    uint8_t addr = 0;
    for (int i = 0; i < 8; i++) {
        if (dipswitches.digitalRead(i) == LOW) {
            addr += 1 << i;
        }
    }
    return addr;
}

uint8_t getConfig()
{
    uint8_t config = 0;
    for (int i = 0; i < 8; i++) {
        if (dipswitches.digitalRead(i + 8) == LOW) {
            config += 1 << i;
        }
    }
    return config;
}
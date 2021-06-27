#include "fan_tacho.h"
#include <Arduino.h>

#define FAN_TACHO_0 0

void FanTacho::setup() {
}

float FanTacho::fan0() {
    int current = digitalRead(FAN_TACHO_0);
    int counter_ms = 0;
    while (digitalRead(FAN_TACHO_0) == current) {
        delay(1);
        counter_ms++;
        if (counter_ms > 100) {
            break;
        }
    }
    current = digitalRead(FAN_TACHO_0);
    counter_ms = 0;
    while (digitalRead(FAN_TACHO_0) == current) {
        delay(1);
        counter_ms++;
        if (counter_ms > 100) {
            break;
        }
    }
    return 1000/(counter_ms*2) * 60;
}
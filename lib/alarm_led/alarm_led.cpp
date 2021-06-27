#include <Arduino.h>

#include "alarm_led.h"

#define ALARM_LED 16

void AlarmLed::setup() {
    pinMode(ALARM_LED, OUTPUT);
}

void AlarmLed::flash(float frequency_hz, float duration_sec) {
    if (frequency_hz < 0.5) frequency_hz = 0.5;
    float interval = 1/frequency_hz;

    int intervals = duration_sec / interval;
    if (intervals < 1) intervals = 1;

    for(int i = 0; i < intervals; i++) {
        digitalWrite(ALARM_LED, HIGH);
        int count_ms = 0;
        int interval_ms = interval * 500;
        while (count_ms < interval_ms) {
            delay(10);
            count_ms = count_ms + 10;
        }
        digitalWrite(ALARM_LED, LOW);
        count_ms = 0;
        while (count_ms < interval_ms) {
            delay(10);
            count_ms = count_ms + 10;
        }
    }
}

void AlarmLed::on() {
    digitalWrite(ALARM_LED, HIGH);
}

void AlarmLed::off() {
    digitalWrite(ALARM_LED, LOW);
}

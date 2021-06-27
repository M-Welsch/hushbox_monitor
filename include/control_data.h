#include <Arduino.h>

struct ControlData {
    float temperature_ambient;
    float temperature_probe0;
    float temperature_probe1;
    float voltage_lightsensor0;
    float voltage_lightsensor1;
    float voltage_fans;
    uint16_t rdac;
    float overtemperature_threshold_0;
    float overtemperature_hysteresis_0;
    float overtemperature_threshold_1;
    float overtemperature_hysteresis_1;
    float lightsensor0_threshold;
    float lightsensor1_threshold;

};

void setup_thresholds(ControlData *control_data);
void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, float data);
void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, int32 data);
String concat_data_table(ControlData *control_data);
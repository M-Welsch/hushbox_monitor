#include "MAX31855.h"
#include "temperature_readout.h"

#define nCS0 15
#define nCS1 2

void Temperature_Sensors::read_sensor(int cs, Max31855_Readouts *max31855_readouts) {
    digitalWrite(cs, LOW);
    max31855_readouts->ambient_temperature = sens.readAmbient();
    digitalWrite(cs, HIGH);
    digitalWrite(cs, LOW);
    max31855_readouts->probe_temperature = sens.readProbe();
    digitalWrite(cs, HIGH);
    digitalWrite(cs, LOW);
    max31855_readouts->fault_code = sens.fault();
    digitalWrite(cs, HIGH);
}

Temperature_Sensors::Temperature_Sensors(void) {
    struct Temperature_Data temperature_data;
}

void Temperature_Sensors::setup() {
    MAX31855_Class sens;
    Serial.print(F("Initializing MAX31855 sensor\n"));
    pinMode(nCS0, OUTPUT);
    pinMode(nCS1, OUTPUT);
    digitalWrite(nCS0, HIGH);
    digitalWrite(nCS1, HIGH);
    sens.begin(nCS0);
//   while (!sens.begin(nCS0)) {
//     Serial.println(F("Unable to start MAX31855. Waiting 3 seconds."));
//     delay(3000);
//   }
}

void Temperature_Sensors::read_temperature() {
    read_sensor(nCS0, &temperature_data.sensor0);
    read_sensor(nCS1, &temperature_data.sensor1);
}


int32 Temperature_Sensors::get_ambient_temperature() {
    return (temperature_data.sensor0.ambient_temperature + temperature_data.sensor1.ambient_temperature) / 2;
}

float Temperature_Sensors::get_ambient_temperature_degrees() {
    return (float) get_ambient_temperature()/1000;
}

int32 Temperature_Sensors::get_probe0_temperature() {
    return temperature_data.sensor0.probe_temperature;
}

float Temperature_Sensors::get_probe0_temperature_degrees() {
    return (float) get_probe0_temperature()/1000;
}

int32 Temperature_Sensors::get_probe1_temperature() {
    return temperature_data.sensor1.probe_temperature;
}

float Temperature_Sensors::get_probe1_temperature_degrees() {
    return (float) get_probe1_temperature()/1000;
}
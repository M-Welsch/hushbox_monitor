#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>

#include "adc.h"
#include "control_data.h"
#include "fan_voltage_control.h"
#include "temperature_readout.h"
#include "web_frontend.h"


#define AD5272_ADDR 0x2F

/* globals */
Temperature_Sensors temp_sensors;
Adc adc;
AD5272_Class fan_voltage_control;
ESP8266WebServer server(80);
struct ControlData control_data;

void handleRoot() {
  String s = concat_data_table(&control_data);
  server.send(200, "text/html", s); //Send web page
}

void add_to_json(char *buffer, String *json_buffer, const char *name, float data, bool last) {
  sprintf(buffer, "\"%s\": %f", name, data);
  json_buffer->concat(buffer);
  if (!last) {json_buffer->concat(","); }
}

void add_to_json(char *buffer, String *json_buffer, const char *name, int32 data, bool last) {
  sprintf(buffer, "\"%s\": %d", name, data);
  json_buffer->concat(buffer);
  if (!last) {json_buffer->concat(","); }
}

void handleRaw() {
  String json_buffer = "{";
  char buffer[256];

  add_to_json(buffer, &json_buffer, "Ambient Temperature", control_data.temperature_ambient, false);
  add_to_json(buffer, &json_buffer, "Probe 0 Temperature", control_data.temperature_probe0, false);
  add_to_json(buffer, &json_buffer, "Probe 1 Temperature", control_data.temperature_probe1, false);
  add_to_json(buffer, &json_buffer, "Lightsensor 0 Voltage", control_data.voltage_lightsensor0, false);
  add_to_json(buffer, &json_buffer, "Lightsensor 1 Voltage", control_data.voltage_lightsensor1, false);
  add_to_json(buffer, &json_buffer, "Fan Voltage", control_data.voltage_fans, false);
  add_to_json(buffer, &json_buffer, "Rdac", control_data.rdac, false);
  add_to_json(buffer, &json_buffer, "Overtemperature Threshold 0", control_data.overtemperature_threshold_0, false);
  add_to_json(buffer, &json_buffer, "Overtemperature Threshold 1", control_data.overtemperature_threshold_1, false);
  add_to_json(buffer, &json_buffer, "Lightsensor 0 Threshold", control_data.lightsensor0_threshold, false);
  add_to_json(buffer, &json_buffer, "Lightsensor 1 Threshold", control_data.lightsensor1_threshold, true);
  json_buffer.concat("}");

  Serial.println("Got Request on Raw Data");
  server.send(200, "application/json", json_buffer);
}


const char* ssid     = "WERKSTATT_WLAN";
const char* password = "XL12ABZXYGKIDO";

/* exclude this to web_frontend.cpp causes exception 9 ... */
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/raw", handleRaw);

  server.begin();
}

void sweep_rdac() {
  delay(5000);
  for(float i = 11; i>0; i = i - 0.2) {
    char buffer[32];
    fan_voltage_control.set_fan_voltage(i);
    delay(10);
    float vol = adc.get_voltage_fans();
    sprintf(buffer, "%f, %f\n", i, vol);
    Serial.print(buffer);
  }
}

void setup() {
  setup_thresholds(&control_data);
  Serial.begin(9600);
  Wire.begin();
  temp_sensors.setup();
  adc.setup();
  fan_voltage_control.setup(AD5272_ADDR);
  fan_voltage_control.set_rdac(0);
  control_data.rdac = 0;
  // setup_wifi();
  sweep_rdac();
}



void loop() {
  temp_sensors.read_temperature();
  control_data.temperature_ambient = temp_sensors.get_ambient_temperature_degrees();
  control_data.temperature_probe0 = temp_sensors.get_probe0_temperature_degrees();
  control_data.temperature_probe1 = temp_sensors.get_probe1_temperature_degrees();
  control_data.voltage_lightsensor0 = adc.get_voltage_lightsensor0(); 
  control_data.voltage_lightsensor1 = adc.get_voltage_lightsensor1();
  control_data.voltage_fans = adc.get_voltage_fans();

  Serial.printf("Ambient Temperature: %f°C\n", temp_sensors.get_ambient_temperature_degrees());
  Serial.printf("Probe 0 Temperature: %f°C\n", temp_sensors.get_probe0_temperature_degrees());
  Serial.printf("Probe 1 Temperature: %f°C\n", temp_sensors.get_probe1_temperature_degrees());
  Serial.printf("V_lightsensor_0: %f\n", adc.get_voltage_lightsensor0()); 
  Serial.printf("V_lightsensor_1: %f\n", adc.get_voltage_lightsensor1());
  Serial.printf("V_fan: %f\n", adc.get_voltage_fans());

  server.handleClient();          //Handle client requests

  delay(1000);
}
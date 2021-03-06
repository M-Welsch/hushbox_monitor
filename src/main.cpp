#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <string>

#include "adc.h"
#include "control_data.h"
#include "fan_voltage_control.h"
#include "temperature_readout.h"
#include "web_frontend.h"
#include "fan_tacho.h"
#include "alarm_led.h"


#define AD5272_ADDR 0x2F

/* globals */
Temperature_Sensors temp_sensors;
Adc adc;
AD5272_Class fan_voltage_control;
ESP8266WebServer server(80);
struct ControlData control_data;
FanTacho fan_tacho;
AlarmLed alarm_led;

void handleRoot() {
  String s = HEAD;
  s.concat(heading("Hushbox", 1));
  s.concat(heading("Current Data", 2));
  s.concat(concat_data_table(&control_data));
  s.concat(heading("Change Parameters", 2));
  s.concat(FORM);
  s.concat(FOOT);
  server.send(200, "text/html", s); //Send web page
}

void handleChangeValues() {
  
  String ot0 = server.arg("ot0");
  if (ot0.compareTo("") != 0) {
    control_data.overtemperature_threshold_0 = ot0.toFloat();
  }
  String ot1 = server.arg("ot1");
  if (ot1.compareTo("") != 0) {
    control_data.overtemperature_threshold_1 = ot1.toFloat();
  }
  String ot_hyst0 = server.arg("ot_hyst0");
  if (ot_hyst0.compareTo("") != 0) {
    control_data.overtemperature_hysteresis_0 = ot_hyst0.toFloat();
  }
  String ot_hyst1 = server.arg("ot_hyst1");
  if (ot_hyst1.compareTo("") != 0) {
    control_data.overtemperature_hysteresis_1 = ot_hyst1.toFloat();
  }
  String light_th0 = server.arg("light_th0");
  if (light_th0.compareTo("") != 0) {
    control_data.lightsensor0_threshold = light_th0.toFloat();
  }
  String light_th1 = server.arg("light_th1");
  if (light_th1.compareTo("") != 0) {
    control_data.lightsensor1_threshold = light_th1.toFloat();
  }
  String vfan = server.arg("vfan");
  if (vfan.compareTo("") != 0) {
      Serial.printf("setting vfan to %fV", vfan.toFloat());
      fan_voltage_control.set_fan_voltage(vfan.toFloat());
      control_data.rdac = fan_voltage_control.get_rdac();
  }
  handleRoot();
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
  server.on("/change_values", handleChangeValues);

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
  setup_wifi();
  alarm_led.setup();
  // fan_tacho.setup(); // doesn't work
  // sweep_rdac();
}

void fill_data_table() {
  control_data.temperature_ambient = temp_sensors.get_ambient_temperature_degrees();
  control_data.temperature_probe0 = temp_sensors.get_probe0_temperature_degrees();
  control_data.temperature_probe1 = temp_sensors.get_probe1_temperature_degrees();
  control_data.voltage_lightsensor0 = adc.get_voltage_lightsensor0(); 
  control_data.voltage_lightsensor1 = adc.get_voltage_lightsensor1();
  control_data.voltage_fans = adc.get_voltage_fans();
  control_data.rdac = fan_voltage_control.get_rdac();
}

void print_data() {
  Serial.printf("Ambient Temperature: %.2f??C\n", temp_sensors.get_ambient_temperature_degrees());
  Serial.printf("Probe 0 Temperature: %.2f??C\n", temp_sensors.get_probe0_temperature_degrees());
  Serial.printf("Probe 1 Temperature: %.2f??C\n", temp_sensors.get_probe1_temperature_degrees());
  Serial.printf("V_lightsensor_0: %.2f\n", adc.get_voltage_lightsensor0()); 
  Serial.printf("V_lightsensor_1: %.2f\n", adc.get_voltage_lightsensor1());
  Serial.printf("V_fan: %.2f\n", adc.get_voltage_fans());
}

void handle_fault_conditions() {
  if (control_data.overtemperature_0 or control_data.overtemperature_1 or control_data.light_0_on or control_data.light_1_on) {
    alarm_led.flash(5, 2);
    fan_voltage_control.set_fan_voltage(12);
  }
}

void loop() {
  temp_sensors.read_temperature();
  fill_data_table();
  check_for_fault_conditions(&control_data);
  handle_fault_conditions();
  print_data();
  server.handleClient();          //Handle client requests
  delay(1000);
}
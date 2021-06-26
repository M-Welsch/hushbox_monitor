#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>

#include "adc.h"
#include "fan_voltage_control.h"
#include "temperature_readout.h"
#include "web_frontend.h"


#define AD5272_ADDR 0x2F


/* globals */
Temperature_Sensors temp_sensors;
Adc adc;
AD5272_Class fan_voltage_control;
ESP8266WebServer server(80);

void handleRoot() {
  Serial.println("Main!!");
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleRaw() {
  Serial.println("Got Request on Raw Data");
  String s = "Blablub";
  server.send(200, "text/html", s);
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


void setup() {
  Serial.begin(9600);
  Wire.begin();
  temp_sensors.setup();
  adc.setup();
  fan_voltage_control.setup(AD5272_ADDR);
  setup_wifi();
}


void loop() {
  temp_sensors.read_temperature();
  Serial.printf("Ambient Temperature: %f°C\n", temp_sensors.get_ambient_temperature_degrees());
  Serial.printf("Probe 0 Temperature: %f°C\n", temp_sensors.get_probe0_temperature_degrees());
  Serial.printf("Probe 1 Temperature: %f°C\n", temp_sensors.get_probe1_temperature_degrees());
  Serial.printf("V_fan: %f\n", adc.get_voltage_fans());
  Serial.printf("V_lightsensor_0: %f\n", adc.get_voltage_lightsensor0()); 
  Serial.printf("V_lightsensor_1: %f\n", adc.get_voltage_lightsensor1());

  server.handleClient();          //Handle client requests

  delay(1000);
}
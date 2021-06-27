#include <Wire.h>
#include "fan_voltage_control.h"
#include <math.h>


void AD5272_Class::write(uint16_t data) {
  Wire.beginTransmission(addr);
  uint8_t data_h = data >> 8;
  uint8_t data_l = data;
  Wire.write(data_h);
  Wire.write(data_l);
  Wire.endTransmission(addr);
}

void AD5272_Class::setup(uint8_t address) {
  addr = address;
  lift_write_protection();
}

void AD5272_Class::lift_write_protection() {
  write(0x1c07);
}

void AD5272_Class::set_rdac(uint16_t val) {
  if (val > 1023) val = 1023;
  rdac = val;
  uint16_t cmd = 1<<10;
  cmd |= val;
  write(cmd);
}

void AD5272_Class::set_fan_voltage(float voltage) {
  // uint16_t rdac_pre;
  // rdac_pre = (uint16_t) 139259 * exp(voltage * (-1) * 1.6811); /* see approx_vfan_rdac for reference, approximation not practicable */ 
  set_rdac(vol2dig(voltage));
}

int AD5272_Class::vol2dig(float vol) {
    if (vol >= 11.5) return 0;
    if (vol >= 11) return 1;
    if (vol >= 10) return 7;
    if (vol >= 9) return 14;
    if (vol >= 8) return 24;
    if (vol >= 7) return 39;
    if (vol >= 6) return 62;
    if (vol >= 5) return 106;
    if (vol >= 4) return 215;
    else return 978;
}

uint16_t AD5272_Class::get_rdac() {
  return rdac;
}
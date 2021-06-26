#include <Wire.h>
#include "fan_voltage_control.h"


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
  uint16_t cmd = 1<<10;
  cmd |= val;
  write(cmd);
}
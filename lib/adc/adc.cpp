#include "adc.h"
#include <ADS1115_WE.h>

#define ADS1115_ADDR 0x48

// Fixme: why can't I make 'adc_ic' a private variable of Adc??
ADS1115_WE adc_ic = ADS1115_WE(ADS1115_ADDR);

void Adc::setup() {
    if(!adc_ic.init()){
        Serial.println("ADS1115 not connected!");
    }
    adc_ic.setVoltageRange_mV(ADS1115_RANGE_2048);
    adc_ic.setCompareChannels(ADS1115_COMP_0_GND);

    adc_ic.setMeasureMode(ADS1115_CONTINUOUS);
}

float Adc::get_voltage_lightsensor0() {
    adc_ic.setCompareChannels(ADS1115_COMP_0_GND);
    return adc_ic.getResult_V();
}

float Adc::get_voltage_lightsensor1() {
    adc_ic.setCompareChannels(ADS1115_COMP_1_GND);
    return adc_ic.getResult_V();
}

float Adc::get_voltage_fans_raw() {
    adc_ic.setCompareChannels(ADS1115_COMP_2_GND);
    return adc_ic.getResult_V();
}

float Adc::get_voltage_fans() {
    return get_voltage_fans_raw() * 6;
}
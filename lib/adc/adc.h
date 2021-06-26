#include <ADS1115_WE.h>
#define _ADC_H_

class Adc {
        float get_voltage_fans_raw();
    public:
        void setup();
        float get_voltage_lightsensor0();
        float get_voltage_lightsensor1();
        float get_voltage_fans();
};
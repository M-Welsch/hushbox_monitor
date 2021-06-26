#include "MAX31855.h"

/***************************************************************************************************
** Declare all program constants                                                                  **
***************************************************************************************************/
const uint32_t SERIAL_SPEED{115200};   ///< Set the baud rate for Serial I/O
const uint8_t  SPI_CHIP_SELECT{2};     ///< Chip-Select PIN for SPI
const uint8_t  SPI_MISO{MISO};         ///< Master-In, Slave-Out PIN for SPI
const uint8_t  SPI_SYSTEM_CLOCK{SCK};  ///< System Clock PIN for SPI

struct Max31855_Readouts {
    int32_t ambient_temperature;
    int32_t probe_temperature;
    int8_t fault_code;
    bool fault_not_connected;
    bool fault_short_to_vcc;
    bool fault_short_to_gnd;
};

struct Temperature_Data {
    struct Max31855_Readouts sensor0;
    struct Max31855_Readouts sensor1;
};

class Temperature_Sensors {
        MAX31855_Class sens;
        struct Temperature_Data temperature_data;
        void read_sensor(int cs, Max31855_Readouts *max31855_readouts);
    public:
        Temperature_Sensors();
        void setup();
        void read_temperature();
        int32 get_ambient_temperature();
        int32 get_probe0_temperature();
        int32 get_probe1_temperature();
        float get_ambient_temperature_degrees();
        float get_probe0_temperature_degrees();
        float get_probe1_temperature_degrees();
};
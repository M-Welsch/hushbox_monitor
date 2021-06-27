class AD5272_Class {
    uint8_t addr;
    uint16_t rdac;
    void write(uint16_t);
    int vol2dig(float);
  public:
    void setup(uint8_t);
    void lift_write_protection(void);
    void set_rdac(uint16_t);
    void set_fan_voltage(float);
    uint16_t get_rdac();
};


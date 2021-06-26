class AD5272_Class {
    uint8_t addr;
    void write(uint16_t);
  public:
    void setup(uint8_t);
    void lift_write_protection(void);
    void set_rdac(uint16_t);
};


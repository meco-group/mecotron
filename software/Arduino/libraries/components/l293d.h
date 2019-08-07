#ifndef L293D_H
#define L293D_H

#include "hbridge_interface.h"

class L293D : public HBridgeInterface {
  private:
    const uint8_t _pin_in1;
    const uint8_t _pin_in2;
    const uint8_t _pin_pwm;

    void setOutputs();

  public:
    L293D(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm, uint16_t battery_voltage, uint8_t ID = 0);

    bool init();
};

#endif //L293D_H

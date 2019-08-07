#ifndef POLOLUTB6612FNG_H
#define POLOLUTB6612FNG_H

#include "hbridge_interface.h"

class PololuTB6612FNG : public HBridgeInterface {
  private:
    const uint8_t _PIN_IN1;
    const uint8_t _PIN_IN2;
    const uint8_t _PIN_PWM;
    const uint8_t _PIN_STBY;

    // static bool _standby;

    void setOutputs();

  public:
    PololuTB6612FNG(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm, int16_t battery_voltage);
    PololuTB6612FNG(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm, uint8_t pin_stby, int16_t battery_voltage);

    bool init();
    void standby_on();
    void standby_off();
};

#endif // POLOLUTB6612FNG_H

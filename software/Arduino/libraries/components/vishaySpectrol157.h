#ifndef VISHAY_SPECTROL_157
#define VISHAY_SPECTROL_157

#include "analog_sensor.h"

class VishaySpectrol157: public AnalogSensor
{
public:
    VishaySpectrol157(uint8_t pin, uint8_t ID = 0);

    float readCalibratedValue();
};

#endif //VISHAY_SPECTROL_157
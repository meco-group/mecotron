#include "vishaySpectrol157.h"
#include "math.h"

VishaySpectrol157::VishaySpectrol157(uint8_t pin, uint8_t ID):
    AnalogSensor(pin,ID)
{
    this->setScale(2.0*M_PI/1023.0);
}

float VishaySpectrol157::readCalibratedValue()
{
    // include additional wrapping such that jump in raw value is always wrapped to pi -> -pi
	
    _calibrated_value = (this->readRawValue() - _offset) * _scale;
    if(_calibrated_value > M_PI){
        _calibrated_value = _calibrated_value - 2.0*M_PI;
    }
    else if(_calibrated_value <= -M_PI){
        _calibrated_value = _calibrated_value + 2.0*M_PI;
    }

   return _calibrated_value;
}

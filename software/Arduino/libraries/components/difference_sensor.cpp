#include "difference_sensor.h"

DifferenceSensor::DifferenceSensor(Sensor1D* sensor, uint16_t min_timelapse, uint8_t ID, float alpha) :
	Sensor1D(ID), _sensor(sensor), _raw_value_old(0), _alpha(alpha), _min_timelapse(min_timelapse)
{
	_last_update = micros();

	// set calibration of the difference sensor
	_offset = 0;
	_scale = _sensor->getScale();
}

int32_t DifferenceSensor::readRawValue()
{
	int32_t raw_value_new = _sensor->readRawValue();
	uint32_t time = micros();

	// calculate the finite difference
	if(time-_last_update > _min_timelapse){
		_raw_value = (1.0-_alpha)*_raw_value + _alpha*((raw_value_new - _raw_value_old)*1000000.0)/(time-_last_update);
		_last_update = time;
		_raw_value_old = raw_value_new;
	}

	return _raw_value;
}

bool DifferenceSensor::init() {
  _last_update = 0;
  _raw_value_old = 0;
  return true;
}

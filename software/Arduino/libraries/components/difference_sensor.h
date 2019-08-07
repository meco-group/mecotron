#ifndef DIFFERENCE_SENSOR_H
#define DIFFERENCE_SENSOR_H

#include "sensor1D.h"

class DifferenceSensor : public Sensor1D
{
private:
	Sensor1D* _sensor;
	uint32_t _last_update;
	int32_t _raw_value_old;

	const float _alpha;
	const uint16_t _min_timelapse;

public:
	DifferenceSensor(Sensor1D* sensor, const uint16_t _min_timelapse = 1, uint8_t ID = 0, const float alpha = 1.0);

	int32_t readRawValue();

  bool init();

};

#endif //DIFFERENCE_SENSOR_H

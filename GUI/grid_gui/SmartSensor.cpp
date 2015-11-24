#include "SmartSensor.h"

//constructors , destructors
SmartSensor::SmartSensor()
{

}
SmartSensor::SmartSensor(const uint64_t id, const std::pair<double, double> &position,
	const std::vector<std::pair<std::string, double> > &measurments) :
	m_id(id),
	m_position(position),
	m_measurments(measurments)
{

}

SmartSensor::~SmartSensor()
{

}

//setters
void SmartSensor::setSensorPosition(double latitude, double longitude)
{
	m_position = std::make_pair(latitude, longitude);
}

void SmartSensor::setSensorId(uint64_t id)
{
	m_id = id;
}

std::pair<double, double> SmartSensor::getPosition()const
{
	return m_position;
}

double SmartSensor::getPositionLong()const
{
	return m_position.first;
}

double SmartSensor::getPositionLat()const
{
	return m_position.second;
}

uint64_t SmartSensor::getId()const
{
	return m_id;
}

#include "SmartSensor.h"

//constructors , destructors
SmartSensor::SmartSensor()
{

}
SmartSensor::SmartSensor(uint64_t id, const std::pair<double, double> &positionGPS,
	double range, const std::vector<std::pair<std::string, double> > &measurments) :
	m_id(id),
	m_positionGPS(positionGPS),
	m_measurments(measurments),
	m_range(range)
{

}

SmartSensor::~SmartSensor()
{

}

//setters
void SmartSensor::setSensorPositionXY(double pos1, double pos2)
{
	m_positionXY = std::make_pair(pos1, pos2);
}
void SmartSensor::setSensorPositionGPS(const std::pair<double, double> &pos)
{
	m_positionGPS = pos;
}

void SmartSensor::setSensorId(uint64_t id)
{
	m_id = id;
}

void SmartSensor::setSensorRadius(double radius)
{
	m_range = radius;
}

std::pair<double, double> SmartSensor::getPositionGPS()const
{
	return m_positionGPS;
}

std::pair<double, double> SmartSensor::getPositionXY()const
{
	return m_positionXY;
}

double SmartSensor::getPositionLong()const
{
	return m_positionGPS.second;
}

double SmartSensor::getPositionLat()const
{
	return m_positionGPS.first;
}

uint64_t SmartSensor::getId()const
{
	return m_id;
}

double SmartSensor::getRange()const
{
	return m_range;
}
/*	Created just for testing purposes
*	Header file contains positions of varius
*	example points for calculating distances beetween them
*   and visualizing them in gui application
*
*/
#ifndef SMARTSENSOR_H
#define SMARTSENSOR_H
#include <utility>
#include <vector>
#include <string>
#include <stdint.h>

class SmartSensor
{
public:
	//constructors , destructors
	SmartSensor(); //default constructor
	SmartSensor(const uint64_t id, const std::pair<double, double> &position,
		const std::vector<std::pair<std::string, double> > &measurments);
	~SmartSensor();

	//setters
	void setSensorPosition(double latitude, double longitude);
	void setSensorId(uint64_t id);

	//getters
	std::pair<double, double> getPosition()const;
	double getPositionLong()const;
	double getPositionLat()const;
	uint64_t getId()const;

	// just for now, later probably should be vector of objects, that can hold diffrent values, e.g. int, string, double,...
	std::vector<std::pair<std::string, double> > m_measurments;
private:
	uint64_t m_id;
	std::pair<double, double> m_position; // <latitude, longitude>
};

#endif // SMARTSENSOR_H

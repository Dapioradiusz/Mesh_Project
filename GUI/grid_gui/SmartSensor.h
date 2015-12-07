/*	Created just for testing purposes
*	Header file contains positions of varius
*	example points for calculating distances beetween them
*   and visualizing them in gui application
*
*/
#ifndef SMARTSENSOR_H
#define SMARTSENSOR_H
#include <utility>
#include <string>
#include <vector>
#include <stdint.h>

class SmartSensor
{
public:
	//constructors , destructors
	SmartSensor(); //default constructor
	SmartSensor(uint64_t id, const std::pair<double, double> &positionGPS,
		double range, const std::vector<std::pair<std::string, double> > &measurments);
	~SmartSensor();

	//setters
	void setSensorPositionXY(double pos1, double pos2);
	void setSensorPositionGPS(const std::pair<double, double> &pos);
	void setSensorId(uint64_t id);
	void setSensorRadius(double radius);

	//getters
	std::pair<double, double> getPositionGPS()const;
	std::pair<double, double> getPositionXY()const;
	double getPositionLong()const;
	double getPositionLat()const;
	uint64_t getId()const;
	double getRange()const;

	std::vector<std::pair<std::string, double> >  m_measurments;
	// just for now, later probably should be vector of objects, that can hold diffrent values, e.g. int, string, double,...
private:
	uint64_t m_id;
	std::pair<double, double> m_positionGPS; // <latitude, longitude>
	std::pair<double, double> m_positionXY; // position on map image
	double m_range; // in meters - sensor range
	
};

#endif // SMARTSENSOR_H

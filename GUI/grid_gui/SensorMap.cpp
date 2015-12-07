#include "SensorMap.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream> 

const double Lat_100m = 0.000899;
const double Long_100m = 0.001244;
const double earth_R = 6371000; // meters

SensorMap::SensorMap()
{

}

SensorMap::~SensorMap()
{

}

Mat SensorMap::getMap(std::vector<SmartSensor> &SensorList)
{
	
	std::pair<double, double> coorTab[2];
	findMinMaxCordinates(SensorList, coorTab); // {<minx,miny> , <maxx, maxy>} // maybe make coordinates a separate object later
	
	double maxX = longLatToDist(coorTab[0].first, coorTab[0].first,
								coorTab[0].second, coorTab[1].second);
	double maxY = longLatToDist(coorTab[0].first, coorTab[1].first,
								coorTab[0].second, coorTab[0].second);
	bool flip = false;
	if (maxX < maxY)
	{
		std::swap(maxX, maxY);
		flip = true;
	}

	anglePositionToMeters(SensorList, coorTab[0], flip);
	//get the maxX maxY in meters
	
	//set the start and end of the map rectangle in meters


	//sensor map c.d - desingn map - image size in pixels based on maxX, maxY, number and position of Sensors and their range
	// picturebox size is 490 x 320 (width(horizontal), length(vertical)) - this should be minimal size of image
	Mat matImage = Mat::zeros(480, 735, CV_8UC3); //  rows-length cols -width format
	bool isTransposed = false;

	//now scale the coordinates
	double scaleX = maxX / matImage.cols;
	double scaleY = maxY / matImage.rows;
	coorTab[0] = std::make_pair(0, 0);
	coorTab[1] = std::make_pair(maxX * scaleX, maxY * scaleY);

	for (std::vector<SmartSensor>::iterator sensorItr = SensorList.begin(); sensorItr != SensorList.end(); sensorItr++)
	{
		sensorItr->setSensorPositionXY(sensorItr->getPositionXY().first * scaleX, 
									   sensorItr->getPositionXY().second * scaleY);
		//now draw stuff on the image - circle and text
		Point sensorLocation(sensorItr->getPositionXY().first, sensorItr->getPositionXY().second);
		std::stringstream sensorText;
		sensorText << "Sensor ID : " << sensorItr->getId();
		int fontFace = CV_FONT_HERSHEY_PLAIN;
		double fontScale = 1.2;
		int thickness = 1;
		circle(matImage, sensorLocation, 5 , Scalar(255,0,0), 3, 8);
		putText(matImage, sensorText.str(), sensorLocation, fontFace, fontScale,
				Scalar::all(255), thickness, 8);
		
	}
	return matImage;
}

void SensorMap::findMinMaxCordinates(const std::vector<SmartSensor> &SensorList, std::pair<double, double>* coordTab)
{
	double minLat = 100; // minimum latitude: -80 antarctica
	double minLong = 180; //minimun longitiude -168 bearing sea
	double maxLat = -100;
	double maxLong = -180;
	for (std::vector<SmartSensor>::const_iterator sensorItr = SensorList.begin(); sensorItr != SensorList.end(); sensorItr++)
	{
		double currLat = sensorItr->getPositionGPS().first;
		double currLong = sensorItr->getPositionGPS().second;
		if ( currLat < minLat)
		{
			minLat = currLat;
		}
		if (currLat > maxLat)
		{
			maxLat = currLat;
		}
		if (currLong < minLong)
		{
			minLong = currLong;
		}
		if (currLong > maxLong)
		{
			maxLong = currLong;
		}
	}
	// add border - 100 m - 0.001244 - longitude 0.000899 latidute
	double borderLong = 2 * Long_100m; // 200m
	double borderLat = 2 * Lat_100m;
	minLong -= borderLong;
	maxLong += borderLong;
	minLat -= borderLat;
	maxLat += borderLat;

	coordTab[0] = std::make_pair(minLat, minLong);
	coordTab[1] = std::make_pair(maxLat, maxLong);
}

double SensorMap::angleToRadian(double angle)
{
	return (angle*M_PI)/180;
}

void SensorMap::anglePositionToMeters(std::vector<SmartSensor> &SensorList, const std::pair<double, double> &coorStart, bool flip)
{
	for (std::vector<SmartSensor>::iterator sensorItr = SensorList.begin(); sensorItr != SensorList.end(); sensorItr++)
	{
		//change
		double distanceX = longLatToDist(coorStart.first, coorStart.first,
										 coorStart.second, sensorItr->getPositionGPS().second);
		double distanceY = longLatToDist(coorStart.first, sensorItr->getPositionGPS().first, 
										 coorStart.second, coorStart.second);
		if (flip)
		{
			sensorItr->setSensorPositionXY(distanceX, distanceY);
		}
		else
		{
			sensorItr->setSensorPositionXY(distanceY, distanceX);
		}		
	}
}

double SensorMap::longLatToDist(double lat1, double lat2, double long1, double long2)
{
	double phi1 = angleToRadian(lat1);
	double phi2 = angleToRadian(lat2);
	double lamdba1 = angleToRadian(long1);
	double lamdba2 = angleToRadian(long2);

	double dphi = phi2 - phi1;
	double dlamdba = lamdba2 - lamdba1;
	double a = (pow(sin(dphi / 2), 2)) + (cos(phi1)*cos(phi2)*(pow(sin(dlamdba / 2), 2)));
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	
	return earth_R * c; //meters
}

void SensorMap::drawSensor(const SmartSensor &sensor, Mat &map)
{

}
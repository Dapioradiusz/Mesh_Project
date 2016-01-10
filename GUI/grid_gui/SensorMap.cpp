#include "SensorMap.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream> 
#include <string>

const double Lat_100m = 0.000899;
const double Long_100m = 0.001244;
const double earth_R = 6371000; // meters

SensorMap::SensorMap()
{

}

SensorMap::~SensorMap()
{

}
double SensorMap::getScale(double axisMax, double imgSize)
{
	double scale = axisMax / imgSize;
	if (axisMax  > imgSize)
	{
		scale = 1 / scale;
	}
	return scale;
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

	//get the maxX maxY in meters
	anglePositionToMeters(SensorList, coorTab[0], flip, maxX, maxY);
	
	//sensor map c.d - desingn map - image size in pixels based on maxX, maxY, number and position of Sensors and their range
	// picturebox size is 490 x 320 (width(horizontal), length(vertical)) - this should be minimal size of image
	Mat map = Mat::zeros(480, 735, CV_8UC3); //  rows-length cols -width format

	double scaleX = getScale(maxX, map.cols);
	double scaleY = getScale(maxY, map.rows);
	

	coorTab[0] = std::make_pair(0, 0);
	coorTab[1] = std::make_pair(maxX * scaleX, maxY * scaleY);

	for (std::vector<SmartSensor>::iterator sensorItr = SensorList.begin(); sensorItr != SensorList.end(); sensorItr++)
	{
		sensorItr->setSensorPositionXY(sensorItr->getPositionXY().first * scaleX, 
									   sensorItr->getPositionXY().second * scaleY);
		//now draw stuff on the image - circle and text
		drawSensor(*sensorItr, map);
	}
	return map;
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
	// add border - 100 m - 0.001244 - longitude; 0.000899 - latidute
	double scaleLong = (maxLong - minLong) / (10 * Long_100m);
	double scaleLat = (maxLat - minLat) / (10 * Lat_100m);
	double borderLong = 2 * scaleLong * Long_100m; // 200m for every 1km
	double borderLat = 2 * scaleLat * Lat_100m;// 200m for every 1km
	double scaleLatLong = (longLatToDist(maxLat, minLat, minLong, minLong)) / (longLatToDist(minLat, minLat, maxLong, minLong));
	double border2 = 0;
	if (scaleLatLong > 2)
	{
		border2 = (scaleLatLong / 10) * (3 * (maxLong - minLong));
		minLong -= border2;
		maxLong += border2;
		minLat -= borderLat;
		maxLat += borderLat;
	}
	else if (scaleLatLong < 0.5)
	{
		border2 = (scaleLatLong / 10)  * (3 * (maxLat - minLat));
		minLong -= borderLong;
		maxLong += borderLong;
		minLat -= border2;
		maxLat += border2;
	}
	else
	{
		minLong -= borderLong;
		maxLong += borderLong;
		minLat -= borderLat;
		maxLat += borderLat;
	}
	coordTab[0] = std::make_pair(maxLat, minLong);
	coordTab[1] = std::make_pair(minLat, maxLong);
}

double SensorMap::angleToRadian(double angle)
{
	return (angle*M_PI)/180;
}

void SensorMap::anglePositionToMeters(std::vector<SmartSensor> &SensorList, 
									  const std::pair<double, double> &coorStart, 
									  bool flip, double maxX, double maxY)
{
	double distanceX, distanceY;
	for (std::vector<SmartSensor>::iterator sensorItr = SensorList.begin(); sensorItr != SensorList.end(); sensorItr++)
	{
		//change
		distanceX = longLatToDist(coorStart.first, coorStart.first,
			coorStart.second, sensorItr->getPositionGPS().second);
		distanceY = longLatToDist(coorStart.first, sensorItr->getPositionGPS().first,
			coorStart.second, coorStart.second);
		if (flip)
		{
			distanceX = maxY - distanceX;
			sensorItr->setSensorPositionXY(distanceY, distanceX);
		}
		else
		{
			sensorItr->setSensorPositionXY(distanceX, distanceY);
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
	Point sensorLocation(sensor.getPositionXY().first, sensor.getPositionXY().second);
	std::stringstream sText;
	sText << "Sensor ID : " << sensor.getId();

	circle(map, sensorLocation, 5, Scalar(255, 0, 0), 3, 8);
	int fontFace = CV_FONT_HERSHEY_PLAIN;
	double fontScale = 1.2;
	int thickness = 1;

	putText(map, sText.str(), sensorLocation, fontFace, fontScale,
		Scalar::all(255), thickness, 8);
	/*CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6);
	IplImage *tmp = cvCloneImage(&(IplImage)map);
	cvPutText(tmp, sText.str().c_str(), cvPoint(sensor.getPositionXY().first, 
			  sensor.getPositionXY().second), &font, cvScalar(255, 255, 255));
	map = cv::cvarrToMat(tmp);*/
}
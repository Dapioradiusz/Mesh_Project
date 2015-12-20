#ifndef SENSORMAP_H
#define SENSORMAP_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "SmartSensor.h"
#include <vector>

using namespace cv;

class SensorMap
{
public:
	SensorMap();
	~SensorMap();

	Mat getMap(std::vector<SmartSensor> &SensorList);

private:
	void findMinMaxCordinates(const std::vector<SmartSensor> &SensorList, std::pair<double, double>* coordTab);

	double angleToRadian(double angle);
	void anglePositionToMeters(std::vector<SmartSensor> &SensorList, const std::pair<double, double> &coorStart, bool flip);
	double longLatToDist(double lat1, double lat2, double long1, double long2);
	
	double getScale(double axisMax, double imgSize);

	void drawSensor(const SmartSensor &sensor, Mat &map);
};

#endif // SENSORMAP_H



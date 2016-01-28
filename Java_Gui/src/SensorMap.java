/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import java.util.List;
import java.lang.Math;
import java.nio.charset.Charset;
import java.nio.file.Files;
import static java.nio.file.Files.lines;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import org.opencv.imgproc.Imgproc;
/**
 *
 * @author Radek
 */



public class SensorMap {
    //constants
    private static final double Lat_100m = 0.000899;
    private static final double Long_100m = 0.001244;
    private static final double earth_R = 6371000; // meters
    private static final String HUB_LABEL = "HUB";
    
    private Mat map;
    private Position coorTab[];
    private List<SmartSensor> SensorList;
    
    public SensorMap()
    {
        SensorList = new ArrayList<SmartSensor>();
    }
    
    public SensorMap(List<SmartSensor> SensorList)
    {
        this.SensorList = SensorList;
    }
    
    private double getScale(double axisMax, double imgSize)
    {
            double scale = axisMax / imgSize;
            if (imgSize < axisMax)
            {
                    scale = 1 / scale;
            }
            return scale;
    }

    public Mat getMap()
    {	
	coorTab = new Position[2];
	findMinMaxCordinates(); // {<minx,miny> , <maxx, maxy>} // maybe make coordinates a separate object later
	
	double maxX = longLatToDist(coorTab[0].positionX, coorTab[0].positionX,
								coorTab[0].positionY, coorTab[1].positionY);
	double maxY = longLatToDist(coorTab[0].positionX, coorTab[1].positionX,
								coorTab[0].positionY, coorTab[0].positionY);
	boolean flip = false;
	if (maxX < maxY)
	{
		double temp = maxX;
                maxX = maxY;
                maxY = temp;
		flip = true;
	}

	//get the maxX maxY in meters
	anglePositionToMeters(flip, maxX, maxY);
	
	//sensor map c.d - desingn map - image size in pixels based on maxX, maxY, number and position of Sensors and their range
	// picturebox size is 490 x 320 (width(horizontal), length(vertical)) - this should be minimal size of image
	map =  Mat.zeros(320, 490, CvType.CV_8UC3); //  rows-length cols -width format
	
        double scaleX = getScale(maxX, map.cols());
	double scaleY = getScale(maxY, map.rows());
        for(SmartSensor sensor : this.SensorList)
        {
            if(sensor.setPositionGPS)
            {
                sensor.setSensorPositionXY(sensor.getPositionXY().positionX * scaleX,
                                           sensor.getPositionXY().positionY * scaleY);
                drawSensor(sensor);
            }
        }
                
	return map;
    }

    private void findMinMaxCordinates()
    {
	double minLat = 100; // minimum latitude: -80 antarctica
	double minLong = 180; //minimun longitiude -168 bearing sea
	double maxLat = -100;
	double maxLong = -180;
        for(SmartSensor sensor : SensorList)
        {
            if(sensor.setPositionGPS)
            {
                double currLat = sensor.getPositionGPS().positionX;
                double currLong = sensor.getPositionGPS().positionY;
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
        }
	
	// add border - 100 m - 0.001244 - longitude; 0.000899 - latidute
	double scaleLong = Math.abs((Math.abs(maxLong) - Math.abs(minLong)) / (10 * Long_100m));
	double scaleLat = Math.abs((Math.abs(maxLat) - Math.abs(minLat)) / (10 * Lat_100m));
	double borderLong = 4 * scaleLong * Long_100m; // 200m for every 1km
	double borderLat = 4 * scaleLat * Lat_100m;// 200m for every 1km
	double scaleLatLong = (longLatToDist(maxLat, minLat, minLong, minLong)) / (longLatToDist(minLat, minLat, maxLong, minLong));
	double border2 = 0;
	if (scaleLatLong > 2)
	{
		border2 = (scaleLatLong / 10) * (4.5 * (maxLong - minLong));
		minLong -= border2;
		maxLong += border2;
		minLat -= borderLat;
		maxLat += borderLat;
	}
	else if (scaleLatLong < 0.7)
	{
		border2 = (scaleLatLong / 10)  * (4.5 * (maxLat - minLat));
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
	coorTab[0] = new Position(maxLat, minLong);
	coorTab[1] = new Position(minLat, maxLong);
    }

    private double angleToRadian(double angle)
    {
        return (angle*Math.PI)/180;
    }

    private void anglePositionToMeters(boolean flip, double maxX, double maxY)
    {
	double distanceX, distanceY;
        Position coorStart = coorTab[0];
        for(SmartSensor sensor : SensorList)
        {
            if(sensor.setPositionGPS)
            {
                distanceX = longLatToDist(coorStart.positionX, coorStart.positionX,
                            coorStart.positionY, sensor.getPositionGPS().positionY);
                distanceY = longLatToDist(coorStart.positionX, sensor.getPositionGPS().positionX,
                        coorStart.positionY, coorStart.positionY);
                if (flip)
                {
                    distanceX = maxY - distanceX;
                    sensor.setSensorPositionXY(distanceY, distanceX);
                }
                else
                {
                    sensor.setSensorPositionXY(distanceX, distanceY);
                }
            }
        }
    }

    private double longLatToDist(double lat1, double lat2, double long1, double long2)
    {
        double phi1 = angleToRadian(lat1);
        double phi2 = angleToRadian(lat2);
        double lamdba1 = angleToRadian(long1);
        double lamdba2 = angleToRadian(long2);

        double dphi = phi2 - phi1;
        double dlamdba = lamdba2 - lamdba1;
        double a = (Math.pow(Math.sin(dphi / 2), 2)) + (Math.cos(phi1)*Math.cos(phi2)*(Math.pow(Math.sin(dlamdba / 2), 2)));
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

        return earth_R * c; //meters
    }

    private void drawSensor(SmartSensor sensor)
    {
        Point sensorLocation = new Point(sensor.getPositionXY().positionX - 50, sensor.getPositionXY().positionY + 30);
        
        int measNr = sensor.getMeasurmentToDraw();
        String id = sensor.getId();
        String sTextName = "Sensor ID : " + id.substring(id.length() - 4, id.length());
        int fontFace = Core.FONT_HERSHEY_PLAIN;
        double fontScale = 0.7;
        Imgproc.putText(map, sTextName, sensorLocation, fontFace, fontScale, Scalar.all(255));
        Scalar circleColor = new Scalar(255, 255, 255);
        if(sensor.hasMeasurements)
        {
            double value = sensor.m_measurments.get(measNr).value;
            String measName = sensor.m_measurments.get(measNr).name;
            String unit = sensor.m_measurments.get(measNr).unit;
        
            String sTextMeas =  measName + " = " + value + "[" + unit + "]"; 
            Imgproc.putText(map, sTextMeas, new Point(sensor.getPositionXY().positionX - 50, 
                           (sensor.getPositionXY().positionY + 50)), fontFace, 
                            fontScale, Scalar.all(255));
            double upperLimit = sensor.m_measurments.get(measNr).upperLimit;
            double lowerLimit = sensor.m_measurments.get(measNr).lowerLimit;
            //check value
            
            if(value > upperLimit)
            {
                circleColor = new Scalar(0, 0, 255);
            }
            else if(value < lowerLimit)
            {
                circleColor = new Scalar(255, 0, 0);
            }
            else
            {
                circleColor = new Scalar(0, 255, 0);
            }
        }
        else
        {
            Imgproc.putText(map, HUB_LABEL, new Point(sensor.getPositionXY().positionX - 50, 
                           (sensor.getPositionXY().positionY + 12)), fontFace, 
                            fontScale, Scalar.all(255));
        }
        
        Imgproc.circle(map, sensorLocation, 10, circleColor, 2, fontFace, 0);
        
                
    }
}

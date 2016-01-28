/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import java.util.ArrayList;
import java.util.List;


public class SmartSensor {
        public ArrayList<Measurment>  m_measurments;
	private String m_id;
	private Position m_positionGPS; // <latitude, longitude>
	private Position m_positionXY; // position on map image
	private double m_range; // in meters - sensor range 
        public boolean sleepEnable;
        private double sleepTime;
        public boolean setPositionGPS;
        public boolean setPositionXY;
        private int measurmentToDraw;
        public boolean hasMeasurements;
        
	public SmartSensor()
        {
            
        }
        
        public SmartSensor(String id)
        {
            m_id = id;
            m_positionGPS = new Position();
            m_range = 0;
            m_measurments = new ArrayList<Measurment>();
            sleepEnable = false;
            sleepTime = 0;
            setPositionGPS = false;
            setPositionXY = false;
            measurmentToDraw = 0;
            hasMeasurements = false;
        }

        public SmartSensor(String id, Position positionGPS, double range)
        {
            m_id = id;
            m_positionGPS = positionGPS;
            m_range = range;
            sleepEnable = false;
            sleepTime = 0;
            setPositionGPS = true;
            setPositionXY = false;
            measurmentToDraw = 0;
            hasMeasurements = false;
        }
        
	public SmartSensor(String id, Position positionGPS, double range, ArrayList<Measurment> measurments)
        {
            m_id = id;
            m_positionGPS = positionGPS;
            m_range = range;
            m_measurments = measurments;
            sleepEnable = false;
            sleepTime = 0;
            setPositionGPS = true;
            setPositionXY = false;
            measurmentToDraw = 0;
            hasMeasurements = true;
        }

	//setters
	public void setSensorPositionXY(double pos1, double pos2)
        {
            m_positionXY = new Position(pos1, pos2);
            setPositionXY = true;
            
        }
        
	public void setSensorPositionGPS(Position pos)
        {
            m_positionGPS = pos;
            setPositionGPS = true;
        }
        
	public void setSensorId(String id)
        {
            m_id = id;
        }
        
	public void setSensorRadius(double radius)
        {
            m_range = radius;
        }
        
        public void setSensorSleepTime(double sleepTime)
        {
            if(sleepTime > 0)
            {
                sleepEnable = true;
            }
            else
            {
                sleepEnable = false;
            }
            this.sleepTime = sleepTime;          
        }
        
        public void setMeasurmentToDraw(int nr)
        {
            measurmentToDraw = nr;
        }
        
	//getters
	public Position getPositionGPS()
        {
            return m_positionGPS;
        }
        
	public Position getPositionXY()
        {
           return m_positionXY; 
        }
        
	public double getPositionLong()
        {
            return m_positionGPS.positionY;
        }
        
	public double getPositionLat()
        {
            return m_positionGPS.positionX;
        }
	String getId()
        {
            return m_id;
        }
	
        double getRange()
        {
            return m_range;
        }
        
        double getSleepTime()
        {
            return sleepTime;
        }
        
        int getMeasurmentToDraw()
        {
            return measurmentToDraw;
        }
}

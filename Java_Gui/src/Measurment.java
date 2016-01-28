/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

/**
 *
 * @author Radek
 */
public class Measurment {
    public String name;
    public double value;
    public String unit;
    public double upperLimit;
    public double lowerLimit;
            
    public Measurment()
    {
        
    }
    
    public Measurment(String name, double value, String unit)
    {
        this.name = name;
        this.value = value;
        this.upperLimit = value + (0.5 * value);
        this.lowerLimit = value - (0.5 * value);
    }
    
    public void setUpperLimit(double upper)
    {
        upperLimit = upper;
    }
    
    public void setLowerLimit(double lower)
    {
        lowerLimit = lower;
    }
}

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
public class Position 
{

    public Position()
    {
        
    }
    
    public Position(double position1, double position2) 
    {
       this.positionX = position1;
       this.positionY = position2;
    }
    
    public double positionX;
    public double positionY;
    
}

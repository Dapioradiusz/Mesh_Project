/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import com.digi.xbee.api.models.XBee64BitAddress;
/**
 *
 * @author Radek
 */
public class FragmentedMessage {
    
    private String xbeeID; 
    private String message;
    private boolean complete;
    
    public FragmentedMessage()
    {
        
    }
    
    public FragmentedMessage(String xbeeID, String message, boolean complete)
    {
        this.complete = complete;
        this.message = message;
        this.xbeeID = xbeeID;
    }
    
    public void finshMessage(String text)
    {
        message += text;
        complete = true;
    }
    public void appendMessage(String text)
    {
        message += text;
        complete = false;
    }
    
    public boolean idCheck(String xbeeID)
    {
        return this.xbeeID.equals(xbeeID);
    }
    
    public void startFragmentedMessage(String text, String xbeeID)
    {
        this.xbeeID = xbeeID;
        complete = false;
        message = text;
    }
    
    public String getMsg()
    {
        return message;
    }
}

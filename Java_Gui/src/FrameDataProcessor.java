/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import static digimesh.xbee.gui.FrameConstantsInterface.MEASURMENTS_END_TAG;
import static digimesh.xbee.gui.FrameConstantsInterface.MEASURMENTS_START_TAG;
import java.util.ArrayList;

/**
 *
 * @author Radek
 */
public class FrameDataProcessor implements FrameConstantsInterface
{   
    private ArrayList<SmartSensor> smartSensors;
    private ArrayList<FragmentedMessage> messagesBuffer;
    
    public FrameDataProcessor(ArrayList<SmartSensor> smartSensors, 
                              ArrayList<FragmentedMessage> messagesBuffer)
    {
        this.smartSensors = smartSensors;
        this.messagesBuffer = messagesBuffer;
    }
    
    
    
    private String extractTagValue(String data, String tagName, int index)
    {
        int value_index = data.indexOf(tagName, index);
        int value_start = data.indexOf(VALUE_INDICATOR, value_index);
        int value_end = data.indexOf(VALUE_INDICATOR, (value_start + 1));
        return data.substring((value_start + 1), value_end);
    }
    
    public boolean extractMsgData(String rxData, String sensorID)
    {
        int index = 0;
        int m_start = rxData.indexOf(MEASURMENT_START_TAG.toString(), index);
        boolean sensorUpdated = false;
        int m_end = rxData.indexOf(MEASURMENTS_END_TAG.toString(), index);
        while(m_start > -1 && m_start < m_end ) //fix in arduino
        {
            //get name, value and unit            
            String name = extractTagValue(rxData, NAME_TAG.toString(), index);           
            //get value
            String value = extractTagValue(rxData, VALUE_TAG.toString(), index);
            double d_value = Double.parseDouble(value);
            //get unit
            String unit = extractTagValue(rxData, UNIT_TAG.toString(), index);
            //find sensor Nr with this ID
            int sensorNr = getSensorNr(sensorID);
            if(sensorNr > -1)
            {
                sensorUpdated = true;
                boolean foundMeasurment = false;
                for(Measurment meas : smartSensors.get(sensorNr).m_measurments)
                {
                    if(meas.name.equals(name))
                    {
                        meas.value = d_value;
                        meas.unit = unit;
                        foundMeasurment = true;
                        break;
                    }
                }
                if(!foundMeasurment) // add new measurment
                {
                   smartSensors.get(sensorNr).m_measurments.add(new Measurment(name, d_value , unit));
                   smartSensors.get(sensorNr).hasMeasurements = true;
                }
            }
            index = rxData.indexOf(MEASURMENT_END_TAG.toString(), (index + 1));
            m_start = rxData.indexOf(MEASURMENT_START_TAG.toString(), index);
        }
        return sensorUpdated;
    }
    
    private int getSensorNr(String sensorID)
    {
        int sensorNr = 0;
        for(SmartSensor sensor: smartSensors)
        {
            if(sensor.getId().contains(sensorID))
            {
                return sensorNr;
            }
            sensorNr++;
        }
        return -1;
    }
    
    public int findFragmentedMsgNr(String sensorID)
    {
        int msgNr = 0;
        for(FragmentedMessage msg : messagesBuffer)
        {
            if(msg.idCheck(sensorID))
            {
                return msgNr;
            }
            msgNr++;
        }
        return -1;
    }
    
    public boolean checkIfRxMsgIsFragment(String rxData)
    {
        boolean start = rxData.contains(MEASURMENTS_START_TAG);
        boolean end = rxData.contains(MEASURMENTS_END_TAG);
        if(start && end)
        {
            return false;
        } 
        return true;
    }
    
}

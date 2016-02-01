/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import com.digi.xbee.api.models.XBeeMessage;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.List;
import javax.swing.SwingWorker;

/**
 *
 * @author Radek
 */
public class XbeeWorker extends SwingWorker<Void, String> implements FrameConstantsInterface
{
    private MeshGui mainGui;
    private FrameDataProcessor frameHandler;
    public XbeeWorker(MeshGui mainGui)
    {
        this.mainGui = mainGui;
        this.frameHandler = new FrameDataProcessor(mainGui.smartSensors, mainGui.messagesBuffer);
    }
        
    @Override
    public Void doInBackground() throws InterruptedException, FileNotFoundException, UnsupportedEncodingException, IOException 
    {
        publish("Started Network Communication");
        while(true)
        {                           
            XBeeMessage xbeeMessage = mainGui.digiModule.readData();

            if (xbeeMessage != null)
            {
                publish("Received Data");
                String rxData = new String(xbeeMessage.getData());
                String sensorAdress = xbeeMessage.getDevice().get64BitAddress().toString();
                publish("From " + xbeeMessage.getDevice().get64BitAddress()
                        + " >> "  + rxData);

               //decode the message
                if(!frameHandler.checkIfRxMsgIsFragment(rxData))
                {
                    //extract data from message
                    publish("Received whole message");
                    frameHandler.extractMsgData(rxData, sensorAdress);
                }
                else
                {

                    if(rxData.contains(MEASURMENTS_START_TAG)) // Starts with Measurments\n
                    {
                        //create new element in buffer
                        publish("Received fragment start message");
                        mainGui.messagesBuffer.add(new FragmentedMessage(sensorAdress, rxData, false));
                    }
                    else if(!rxData.contains(MEASURMENTS_START_TAG) && !rxData.contains(MEASURMENTS_END_TAG)) //no Measurments\n or Measurments_end\n
                   {
                       //append to fragment to existing msg buffer
                       //find message with speciefied ID
                        publish("Received fragment middle message");
                        int msgNr = frameHandler.findFragmentedMsgNr(sensorAdress);
                        if(msgNr > -1)
                        {
                            mainGui.messagesBuffer.get(msgNr).appendMessage(rxData);
                        }
                    }
                    else if(rxData.contains(MEASURMENTS_END_TAG))
                    {
                        //append it to existing, pop it from buffer
                        // and extract sensor data
                        publish("Received fragment end message");
                        int msgNr = frameHandler.findFragmentedMsgNr(sensorAdress);
                        if(msgNr > -1)
                        {
                            FragmentedMessage finishedMsg = mainGui.messagesBuffer.remove(msgNr);
                            finishedMsg.finshMessage(rxData);
                            frameHandler.extractMsgData(finishedMsg.getMsg(), sensorAdress);
                        }         
                    }   
                }
           }
            Thread.sleep(500); 
        }
    }

    @Override
    public void done() 
    {
        mainGui.appendConnectionLog("Finished worker thread");//.
    }

    @Override
    public void process(List<String> data)
    {
        for(String text : data)
        {
            mainGui.appendConnectionLog(text);
        }     
    }
                  
}

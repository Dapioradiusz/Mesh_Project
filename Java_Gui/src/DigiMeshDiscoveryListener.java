/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import com.digi.xbee.api.RemoteDigiMeshDevice;
import com.digi.xbee.api.RemoteXBeeDevice;
import com.digi.xbee.api.listeners.IDiscoveryListener;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Radek
 */
class DigiMeshDiscoveryListener implements IDiscoveryListener {
        
    private MeshGui mainGui;
    public DigiMeshDiscoveryListener(MeshGui mainGui)
    {
        this.mainGui = mainGui;
    }
    
    @Override
    public void deviceDiscovered(RemoteXBeeDevice discoveredDevice) {
        String deviceID = discoveredDevice.get64BitAddress().toString();
        
        mainGui.appendConnectionLog("Device discovered: " + deviceID);
        //add discovered device to main gui sensorList
        //check if it is already added to sensor List
        if(!mainGui.checkIfSensorAdded(deviceID))
        {
            mainGui.smartSensors.add(new SmartSensor(deviceID));
            mainGui.updateSensorComboBox();
            mainGui.appendConnectionLog("Device " + deviceID + " added to sensor list");
        }

    }


    @Override
    public void discoveryError(String error) {
        mainGui.appendConnectionLog("There was an error discovering devices: " + error);
    }

    @Override
    public void discoveryFinished(String error) {
        if (error == null)
        {
                mainGui.appendConnectionLog("Discovery process finished successfully.");
        }
        else
        {
                mainGui.appendConnectionLog("Discovery process finished due to the following error: " + error);
        }
        mainGui.digiModule.close();
    }
    
}


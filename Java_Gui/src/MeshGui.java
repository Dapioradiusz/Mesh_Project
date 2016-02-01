/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import com.digi.xbee.api.DigiMeshDevice;
import com.digi.xbee.api.DigiMeshNetwork;
import com.digi.xbee.api.XBeeDevice;
import com.digi.xbee.api.XBeeNetwork;
import com.digi.xbee.api.exceptions.XBeeException;
import com.digi.xbee.api.models.XBeeMessage;
import com.digi.xbee.api.utils.HexUtils;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import java.awt.Image;
import java.awt.event.ItemEvent;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import javax.swing.ImageIcon;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultComboBoxModel;
import javax.swing.SwingWorker;

/**
 *
 * @author Radek
 */
public class MeshGui extends javax.swing.JFrame {

    /**
     * Creates new form MeshGui
     */
    private static final String HUB_64ADRESS = "0013A20040B49A62";
    private static final long DISCOVERY_TIMEOUT = 15000;
    
    
    public ArrayList<SmartSensor> smartSensors;
    private boolean workerThreadStarted;
    private XbeeWorker worker;
    private ArrayList<String> hubDataToSend;
    private boolean broadcastData;
    private boolean canAddData;
    public boolean connectedToHubModule;
    public ArrayList<FragmentedMessage> messagesBuffer;
    private Mat sensorMap;
    public boolean sensorSettingsOpen;
    public boolean serialGuiOpen;
    private int caretPosition;
    private int prevCaretPos;
    
    public DigiMeshDevice digiModule;
    
    public MeshGui() {
        initComponents();
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        sensorMap = Mat.zeros(320, 490, CvType.CV_8UC3);
        BufferedImage image  =  Mat2BufferedImage(sensorMap);
        ImageIcon icon = new ImageIcon(image);       
        jLMapImage.setIcon(icon);
        //init memeber variable
        workerThreadStarted = false;
        broadcastData = false;
        canAddData = true;
        hubDataToSend = new ArrayList<>();
        smartSensors = new ArrayList<>();
        smartSensors.add(new SmartSensor(HUB_64ADRESS));
        cbSensorList.addItem("HUB");
        messagesBuffer = new ArrayList<>();
        connectedToHubModule = false;
        serialGuiOpen = false;
        sensorSettingsOpen = false;
        Enumeration ports = CommPortIdentifier.getPortIdentifiers();
        ArrayList<String> portNames = new ArrayList<>();
        while(ports.hasMoreElements())
        {
            portNames.add(((CommPortIdentifier)ports.nextElement()).getName());
        }
        cbPortName.setModel(new DefaultComboBoxModel(portNames.toArray()));
        caretPosition = 0;
        prevCaretPos = 0;
    }
    
    public void drawMap()
    {
        if(getNumberOfSensorsSetGPs() > 1)
        {
            SensorMap sMap = new SensorMap(smartSensors);
            sensorMap = sMap.getMap();
            BufferedImage map = Mat2BufferedImage(sensorMap);
            ImageIcon icon = new ImageIcon(map);       
            jLMapImage.setIcon(icon);
        }
    }
    
    private int getNumberOfSensorsSetGPs()
    {
        int counter = 0;
        for(SmartSensor sensor : smartSensors)
        {
            if(sensor.setPositionGPS)
            {
                ++counter;
            }
        }
        return counter;
    }
                      
    public void changeSensorMeasurment(int sensorNr,
                                       int measurmentNr,   
                                       Measurment measurment)
    {
        smartSensors.get(sensorNr).m_measurments.set(measurmentNr, measurment);
    }
   
    public void changeSensorSleepSettings(int sensorNr, double sleepTime)
    {
       smartSensors.get(sensorNr).setSensorSleepTime(sleepTime);
    }
    
    private BufferedImage Mat2BufferedImage(Mat m){
        int type = BufferedImage.TYPE_BYTE_GRAY;
        if ( m.channels() > 1 ) {
            type = BufferedImage.TYPE_3BYTE_BGR;
        }
        int bufferSize = m.channels()*m.cols()*m.rows();
        byte [] b = new byte[bufferSize];
        m.get(0,0,b); // get all the pixels
        BufferedImage image = new BufferedImage(m.cols(),m.rows(), type);
        final byte[] targetPixels = ((DataBufferByte) image.getRaster().getDataBuffer()).getData();
        System.arraycopy(b, 0, targetPixels, 0, b.length);  
        return image;
    }
    
    public void appendConnectionLog(String text)
    {
        txConnectionLog.append(">> " + text + "\n");
        if(cbxAutoScroll.isSelected())
        {
            caretPosition = txConnectionLog.getDocument().getLength();
            txConnectionLog.setCaretPosition(caretPosition);
            
        }   
    }
       
    public void updateSensorComboBox()
    {
        int sensNr = cbSensorList.getItemCount() + 1;
        cbSensorList.addItem("Sensor " + Integer.toString(sensNr));
    }
    
    private void showSensorProperities(int sensorNr)
    {
        String text = new String();
        text += "Sensor ID : " + smartSensors.get(sensorNr).getId() + "\n"
                + "Position GPS : " + smartSensors.get(sensorNr).getPositionLat() +
                " " + smartSensors.get(sensorNr).getPositionLong() + "\nMesurments:\n";
        if(smartSensors.get(sensorNr).hasMeasurements)
        {
            for(Measurment meas : smartSensors.get(sensorNr).m_measurments)
            {
                text += meas.name + " = " + meas.value + 
                    " Upper Limit = " + meas.upperLimit +
                    " Lower Limit = " + meas.lowerLimit + "\n\n";
            }
        }
        
        txSensorData.setText(text);
    }
    
     public boolean checkIfSensorAdded(String id)
    {
        for(SmartSensor sensor : smartSensors)
        {
            if(sensor.getId().equals(id))
            {
                return true;
            }
        }
        return false;
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLMapImage = new javax.swing.JLabel();
        btnStartReadWrite = new javax.swing.JButton();
        btnShowAvailableSensors = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        txConnectionLog = new javax.swing.JTextArea();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        jScrollPane2 = new javax.swing.JScrollPane();
        txSensorData = new javax.swing.JTextArea();
        cbSensorList = new javax.swing.JComboBox<>();
        btnSensorStatus = new javax.swing.JButton();
        btnSensorSettings = new javax.swing.JButton();
        cbMeasToDraw = new javax.swing.JComboBox<>();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        btnClearLog = new javax.swing.JButton();
        cbxAutoScroll = new javax.swing.JCheckBox();
        btnConnect = new javax.swing.JButton();
        btnCancel = new javax.swing.JButton();
        cbPortName = new javax.swing.JComboBox<>();
        txBaudRate = new javax.swing.JTextField();
        jLabel6 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        btnStartReadWrite.setText("Start Read/Write");
        btnStartReadWrite.setActionCommand("");
        btnStartReadWrite.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnStartReadWriteActionPerformed(evt);
            }
        });

        btnShowAvailableSensors.setText("Discover Sensors");
        btnShowAvailableSensors.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnShowAvailableSensorsActionPerformed(evt);
            }
        });

        txConnectionLog.setEditable(false);
        txConnectionLog.setColumns(20);
        txConnectionLog.setRows(5);
        jScrollPane1.setViewportView(txConnectionLog);

        jLabel1.setFont(new java.awt.Font("Tahoma", 0, 12)); // NOI18N
        jLabel1.setText("Sensor Map");

        jLabel2.setFont(new java.awt.Font("Tahoma", 0, 12)); // NOI18N
        jLabel2.setText("Connection Log");

        txSensorData.setEditable(false);
        txSensorData.setColumns(20);
        txSensorData.setRows(5);
        jScrollPane2.setViewportView(txSensorData);

        cbSensorList.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cbSensorListItemStateChanged(evt);
            }
        });

        btnSensorStatus.setText("Sensor Status");
        btnSensorStatus.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSensorStatusActionPerformed(evt);
            }
        });

        btnSensorSettings.setText("Sensor Properities");
        btnSensorSettings.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSensorSettingsActionPerformed(evt);
            }
        });

        cbMeasToDraw.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cbMeasToDrawItemStateChanged(evt);
            }
        });

        jLabel3.setText("Sensor list");

        jLabel4.setText("Measurment to Draw");

        btnClearLog.setText("Clear Log");
        btnClearLog.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnClearLogActionPerformed(evt);
            }
        });

        cbxAutoScroll.setText("autoscroll");

        btnConnect.setText("Open Port");
        btnConnect.setActionCommand("");
        btnConnect.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnConnectActionPerformed(evt);
            }
        });

        btnCancel.setText("Close Port");
        btnCancel.setActionCommand("");
        btnCancel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCancelActionPerformed(evt);
            }
        });

        cbPortName.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cbPortNameActionPerformed(evt);
            }
        });

        txBaudRate.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txBaudRateActionPerformed(evt);
            }
        });

        jLabel6.setText("Baud Rate");

        jLabel5.setText("Port name");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(69, 69, 69)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLMapImage, javax.swing.GroupLayout.PREFERRED_SIZE, 490, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel5)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                    .addComponent(btnConnect, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 115, Short.MAX_VALUE)
                                    .addComponent(cbPortName, javax.swing.GroupLayout.Alignment.LEADING, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                                .addGap(55, 55, 55)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(btnCancel, javax.swing.GroupLayout.PREFERRED_SIZE, 115, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel6)
                                    .addComponent(txBaudRate, javax.swing.GroupLayout.PREFERRED_SIZE, 115, javax.swing.GroupLayout.PREFERRED_SIZE))))
                        .addGap(44, 44, 44)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(btnStartReadWrite, javax.swing.GroupLayout.DEFAULT_SIZE, 161, Short.MAX_VALUE)
                            .addComponent(btnShowAvailableSensors, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                .addGap(102, 102, 102)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addGroup(layout.createSequentialGroup()
                            .addComponent(btnSensorSettings, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(btnSensorStatus, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addComponent(jLabel2)
                        .addComponent(jScrollPane1)
                        .addComponent(jScrollPane2)
                        .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(cbSensorList, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(jLabel3))
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 50, Short.MAX_VALUE)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                .addComponent(jLabel4)
                                .addComponent(cbMeasToDraw, 0, 150, Short.MAX_VALUE)
                                .addComponent(cbxAutoScroll, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
                    .addComponent(btnClearLog, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(123, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(60, 60, 60)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(jLabel2))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLMapImage, javax.swing.GroupLayout.PREFERRED_SIZE, 320, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 306, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(btnClearLog, javax.swing.GroupLayout.DEFAULT_SIZE, 29, Short.MAX_VALUE)
                            .addComponent(cbxAutoScroll))))
                .addGap(11, 11, 11)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel3)
                            .addComponent(jLabel4))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(cbSensorList, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(cbMeasToDraw, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(8, 8, 8)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 141, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel6)
                            .addComponent(jLabel5))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(cbPortName, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(txBaudRate, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(btnShowAvailableSensors, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(17, 17, 17)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(btnConnect, javax.swing.GroupLayout.PREFERRED_SIZE, 37, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(btnCancel, javax.swing.GroupLayout.PREFERRED_SIZE, 35, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(btnStartReadWrite, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addGap(24, 24, 24)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnSensorSettings, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(btnSensorStatus, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(72, 72, 72))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnStartReadWriteActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnStartReadWriteActionPerformed
        if(!workerThreadStarted)
        {
            try {
                if(!digiModule.isOpen())
                {
                    digiModule.open();
                }
                worker = new XbeeWorker(this);
                worker.execute();
                workerThreadStarted = true;
            } catch (XBeeException ex) {
                Logger.getLogger(MeshGui.class.getName()).log(Level.SEVERE, null, ex);               
            }
       }
        else
        {
            //stop read write xbee operations
            worker.cancel(true);
            digiModule.close();
            workerThreadStarted = false;
        }
    }//GEN-LAST:event_btnStartReadWriteActionPerformed

    private void btnShowAvailableSensorsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnShowAvailableSensorsActionPerformed
        // TODO add your handling code here:
        //Digimesh network discovery
        if(connectedToHubModule)
        {
            try
            {
                digiModule.open();			
                XBeeNetwork digiNetwork = digiModule.getNetwork();

                digiNetwork.setDiscoveryTimeout(DISCOVERY_TIMEOUT);

                digiNetwork.addDiscoveryListener(new DigiMeshDiscoveryListener(this));

                digiNetwork.startDiscoveryProcess();

                appendConnectionLog("Discovering remote XBee devices..."); 
            }
            catch (XBeeException e) 
            {
                e.printStackTrace();
                digiModule.close();
            }
        }
					
    }//GEN-LAST:event_btnShowAvailableSensorsActionPerformed

    private void btnSensorSettingsActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSensorSettingsActionPerformed
        // TODO add your handling code here: 
        if(!sensorSettingsOpen)
        {
            SensorProperities settings = new SensorProperities(this);
            settings.setVisible(true);
            sensorSettingsOpen = true;
        }
    }//GEN-LAST:event_btnSensorSettingsActionPerformed

    private void btnSensorStatusActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSensorStatusActionPerformed
        // TODO add your handling code here:
        if(cbSensorList.getItemCount() > 0)
        {
            int sensorNr = cbSensorList.getSelectedIndex();
            showSensorProperities(sensorNr);
        }
    }//GEN-LAST:event_btnSensorStatusActionPerformed

    private void cbSensorListItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cbSensorListItemStateChanged
        // TODO add your handling code here:
        // TODO add your handling code here:
        //repopulate the measToDraw cb
        if(evt.getStateChange() == ItemEvent.DESELECTED)
        {
            int sensNr = cbSensorList.getSelectedIndex();
            if(smartSensors.get(sensNr).hasMeasurements)
            {
                String measurments[] = new String[smartSensors.get(sensNr).m_measurments.size()];
                for(int i = 0; i < smartSensors.get(sensNr).m_measurments.size(); i++)
                {
                    measurments[i] = smartSensors.get(sensNr).m_measurments.get(i).name;
                }
                cbMeasToDraw.setModel(new DefaultComboBoxModel(measurments));
                cbMeasToDraw.setSelectedIndex(0);
                smartSensors.get(sensNr).setMeasurmentToDraw(0);
            }
            else
            {
                cbMeasToDraw.removeAllItems();
            }
            drawMap();      
        }
    }//GEN-LAST:event_cbSensorListItemStateChanged

    private void cbMeasToDrawItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cbMeasToDrawItemStateChanged
        // TODO add your handling code here:
        if(evt.getStateChange() == ItemEvent.DESELECTED)
        {
            int sensNr = cbSensorList.getSelectedIndex();
            if(smartSensors.get(sensNr).hasMeasurements)
            {
                int measNr = cbMeasToDraw.getSelectedIndex();  
                smartSensors.get(sensNr).setMeasurmentToDraw(measNr);
                drawMap(); 
            }    
        }
    }//GEN-LAST:event_cbMeasToDrawItemStateChanged

    private void btnConnectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnConnectActionPerformed
        // TODO add your handling code here:
        String chosenPort = (String)cbPortName.getSelectedItem();
        try {
            CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(chosenPort);
            if (portIdentifier.isCurrentlyOwned()) {
                appendConnectionLog("Serial Connect error : Port" + chosenPort + " is in use!");
            }
            else
            {
                digiModule = new DigiMeshDevice(chosenPort, Integer.parseInt(txBaudRate.getText()));
                appendConnectionLog("Connected to port : " + chosenPort + ", Baud Rate : " + txBaudRate.getText());
                connectedToHubModule = true;
            }
        } catch (NoSuchPortException ex) {
            Logger.getLogger(SerialPortGui.class.getName()).log(Level.SEVERE, null, ex);
        }
    }//GEN-LAST:event_btnConnectActionPerformed

    private void btnClearLogActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnClearLogActionPerformed
        // TODO add your handling code here:
        txConnectionLog.setText("");
    }//GEN-LAST:event_btnClearLogActionPerformed

    private void btnCancelActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCancelActionPerformed
        // TODO add your handling code here:
        if(connectedToHubModule)
        {
            if(digiModule.isOpen())
            {
               appendConnectionLog("Closed HUB device");
               digiModule.close();
            }
            appendConnectionLog("Serial Port Closed");
            connectedToHubModule = false;
        }
    }//GEN-LAST:event_btnCancelActionPerformed

    private void txBaudRateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txBaudRateActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_txBaudRateActionPerformed

    private void cbPortNameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cbPortNameActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_cbPortNameActionPerformed
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(MeshGui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(MeshGui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(MeshGui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(MeshGui.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new MeshGui().setVisible(true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnCancel;
    private javax.swing.JButton btnClearLog;
    private javax.swing.JButton btnConnect;
    private javax.swing.JButton btnSensorSettings;
    private javax.swing.JButton btnSensorStatus;
    private javax.swing.JButton btnShowAvailableSensors;
    private javax.swing.JButton btnStartReadWrite;
    private javax.swing.JComboBox<String> cbMeasToDraw;
    private javax.swing.JComboBox<String> cbPortName;
    private javax.swing.JComboBox<String> cbSensorList;
    private javax.swing.JCheckBox cbxAutoScroll;
    private javax.swing.JLabel jLMapImage;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextField txBaudRate;
    private javax.swing.JTextArea txConnectionLog;
    private javax.swing.JTextArea txSensorData;
    // End of variables declaration//GEN-END:variables
}

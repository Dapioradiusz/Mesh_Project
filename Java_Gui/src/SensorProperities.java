/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package digimesh.xbee.gui;

import java.awt.event.ItemEvent;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JComboBox;
import javax.swing.JSlider;

/**
 *
 * @author Radek
 */
public class SensorProperities extends javax.swing.JFrame {

    /**
     * Creates new form SetLimitsGui
     */
    //costatnts
    private static final int SLEEP_MAX = 5;
    private static final int SLEEP_MIN = 0;
    private static final int SLEEP_INIT = 1;
    
    //member variables
    private final MeshGui mainGui;
    private boolean changedMeasParams;
    public SensorProperities(MeshGui mainGui) {
        initComponents();
        changedMeasParams = false;
        
        this.mainGui = mainGui;
        //fill comboBoxes with mainGui sensor data
        if(mainGui.smartSensors.size() > 0)
        {
            String sensorNames[] = new String[mainGui.smartSensors.size()];
            for(int i = 0; i < mainGui.smartSensors.size(); i++)
            {
                sensorNames[i] = "Sensor " + (i + 1);
            }
            cbSensorList.setModel(new DefaultComboBoxModel(sensorNames));
            cbSensorList.setSelectedIndex(0);
            fillMeasurmentsComboBox();
            slSleepTime = new JSlider(SLEEP_MIN, SLEEP_MAX, SLEEP_INIT);
        }
    }
    
    private void fillMeasurmentsComboBox()
    {
        int sensNr = cbSensorList.getSelectedIndex();
        cbxDrawOnMap.setSelected(mainGui.smartSensors.get(sensNr).setPositionGPS);
        if(cbxDrawOnMap.isSelected())
        {
            Position pos = mainGui.smartSensors.get(sensNr).getPositionGPS();
            etGPSLatitude.setText(Double.toString(pos.positionX));
            etGPSLongitude.setText(Double.toString(pos.positionY));
        }
        cbxEnableSleep.setSelected(mainGui.smartSensors.get(sensNr).sleepEnable);
        slSleepTime.setValue((int) mainGui.smartSensors.get(sensNr).getSleepTime());
        if(mainGui.smartSensors.get(sensNr).hasMeasurements)
        {
            String measurments[] = new String[mainGui.smartSensors.get(sensNr).m_measurments.size()];
            for(int i = 0; i < mainGui.smartSensors.get(sensNr).m_measurments.size(); i++)
            {
                measurments[i] = mainGui.smartSensors.get(sensNr).m_measurments.get(i).name;
            }
            cbMeasurmentsList.setModel(new DefaultComboBoxModel(measurments));
            cbMeasurmentsList.setSelectedIndex(0);
            setMeasurmentVauleControls();
        }
        else
        {
            cbMeasurmentsList.removeAllItems();
        }
    }
    private void setMeasurmentVauleControls()
    {
        int measurmentNr = cbMeasurmentsList.getSelectedIndex();
        int sensNr = cbSensorList.getSelectedIndex();
        if(mainGui.smartSensors.get(sensNr).hasMeasurements)
        {
            Double measurmentValue = mainGui.smartSensors.get(sensNr).m_measurments.get(measurmentNr).value;
            txMeasurmentValue.setText(measurmentValue.toString());
            txCurrentLowerLimit.setText(Double.toString(mainGui.smartSensors.get(sensNr).m_measurments.get(measurmentNr).lowerLimit));
            txCurrentUpperLimit.setText(Double.toString(mainGui.smartSensors.get(sensNr).m_measurments.get(measurmentNr).upperLimit));
            int maxLimit = 2 * measurmentValue.intValue();
            int minLimit = new Double(0.5 * measurmentValue).intValue();
            slUpperLimit.setMaximum(maxLimit);
            etUpperLimit.setText(Integer.toString(maxLimit));
            slUpperLimit.setMinimum(minLimit);
            etLowerLimit.setText(Integer.toString(minLimit));
        }
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        slUpperLimit = new javax.swing.JSlider();
        cbSensorList = new javax.swing.JComboBox<>();
        slLowerLimit = new javax.swing.JSlider();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        btnAccept = new javax.swing.JButton();
        btnClose = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        cbMeasurmentsList = new javax.swing.JComboBox<>();
        jLabel2 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        jLabel6 = new javax.swing.JLabel();
        etUpperLimit = new javax.swing.JTextField();
        etLowerLimit = new javax.swing.JTextField();
        cbxEnableSleep = new javax.swing.JCheckBox();
        slSleepTime = new javax.swing.JSlider();
        jLabel7 = new javax.swing.JLabel();
        txSleepTime = new javax.swing.JTextField();
        jLabel8 = new javax.swing.JLabel();
        etGPSLatitude = new javax.swing.JTextField();
        etGPSLongitude = new javax.swing.JTextField();
        jLabel9 = new javax.swing.JLabel();
        jLabel10 = new javax.swing.JLabel();
        cbxDrawOnMap = new javax.swing.JCheckBox();
        txCurrentLowerLimit = new javax.swing.JTextField();
        txMeasurmentValue = new javax.swing.JTextField();
        txCurrentUpperLimit = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        slUpperLimit.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                slUpperLimitStateChanged(evt);
            }
        });

        cbSensorList.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cbSensorListItemStateChanged(evt);
            }
        });

        slLowerLimit.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                slLowerLimitStateChanged(evt);
            }
        });

        jLabel3.setText("Upper Limit");

        jLabel4.setText("Lower Limit");

        btnAccept.setText("Accept");
        btnAccept.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnAcceptActionPerformed(evt);
            }
        });

        btnClose.setText("Close");
        btnClose.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnCloseActionPerformed(evt);
            }
        });

        jLabel1.setText("Sensor List");

        cbMeasurmentsList.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cbMeasurmentsListItemStateChanged(evt);
            }
        });

        jLabel2.setText("Measurments");

        jLabel5.setText("Current Upper Limit");

        jLabel6.setText("Current Lower Limit");

        etUpperLimit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                etUpperLimitActionPerformed(evt);
            }
        });

        etLowerLimit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                etLowerLimitActionPerformed(evt);
            }
        });

        cbxEnableSleep.setText("Enable Sleep");

        jLabel7.setText("Sleep Time");

        jLabel8.setText("Current Value");

        jLabel9.setText("GPS Latitude");

        jLabel10.setText("GPS Longitude");

        cbxDrawOnMap.setText("Draw On Map");

        txCurrentLowerLimit.setEditable(false);

        txMeasurmentValue.setEditable(false);

        txCurrentUpperLimit.setEditable(false);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(50, 50, 50)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(btnAccept, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE)
                    .addComponent(slLowerLimit, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
                    .addComponent(jLabel3)
                    .addComponent(jLabel4)
                    .addComponent(jLabel1)
                    .addComponent(etUpperLimit)
                    .addComponent(etLowerLimit)
                    .addComponent(cbSensorList, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(slUpperLimit, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE))
                .addGap(48, 48, 48)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(cbMeasurmentsList, javax.swing.GroupLayout.PREFERRED_SIZE, 123, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(jLabel2)
                        .addComponent(jLabel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jLabel6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                        .addComponent(txMeasurmentValue, javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(jLabel8, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(txCurrentLowerLimit, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE)
                            .addComponent(txCurrentUpperLimit))
                        .addGap(66, 66, 66)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(cbxEnableSleep, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jLabel7, javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(slSleepTime, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE)
                    .addComponent(txSleepTime, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE))
                .addGap(62, 62, 62)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(etGPSLatitude, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE)
                        .addComponent(etGPSLongitude, javax.swing.GroupLayout.DEFAULT_SIZE, 123, Short.MAX_VALUE)
                        .addComponent(jLabel9)
                        .addComponent(jLabel10)
                        .addComponent(cbxDrawOnMap, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(btnClose, javax.swing.GroupLayout.PREFERRED_SIZE, 123, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(40, 40, 40))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(33, 33, 33)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(cbSensorList, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(cbMeasurmentsList, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(cbxEnableSleep)
                            .addComponent(cbxDrawOnMap))))
                .addGap(23, 23, 23)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel3)
                        .addGap(4, 4, 4)
                        .addComponent(slUpperLimit, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(etUpperLimit, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel4)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(slLowerLimit, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(18, 18, 18)
                                .addComponent(etLowerLimit, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel5)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(txCurrentUpperLimit, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(28, 28, 28)
                                .addComponent(jLabel6)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(txCurrentLowerLimit, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel9)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(etGPSLatitude, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(jLabel10)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(etGPSLongitude, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel7)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(txSleepTime, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel8)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(txMeasurmentValue, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(18, 18, 18)
                        .addComponent(slSleepTime, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 35, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnAccept, javax.swing.GroupLayout.PREFERRED_SIZE, 35, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(btnClose, javax.swing.GroupLayout.PREFERRED_SIZE, 35, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(35, 35, 35))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnCloseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnCloseActionPerformed
        // TODO add your handling code here:
        mainGui.sensorSettingsOpen = false;
        this.setVisible(false);
    }//GEN-LAST:event_btnCloseActionPerformed

    private void etLowerLimitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_etLowerLimitActionPerformed
        // TODO add your handling code here:
        changedMeasParams = true;
    }//GEN-LAST:event_etLowerLimitActionPerformed

    private void etUpperLimitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_etUpperLimitActionPerformed
        // TODO add your handling code here:
        changedMeasParams = true;
    }//GEN-LAST:event_etUpperLimitActionPerformed

    private void btnAcceptActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnAcceptActionPerformed
        // TODO add your handling code here:
        //check if sleep options changed
        int sensNr = cbSensorList.getSelectedIndex(); 
        int measurmentNr = cbMeasurmentsList.getSelectedIndex();
        
        if(cbxEnableSleep.isSelected())
        {
            if(slSleepTime.getValue() > 0)   
            {
                mainGui.smartSensors.get(sensNr).sleepEnable = true;
                mainGui.smartSensors.get(sensNr).setSensorSleepTime(slSleepTime.getValue());
            }
        }
        //check measurment options
        if(cbMeasurmentsList.getItemCount() > 0 && changedMeasParams)
        {
            double currentEtLower = Double.parseDouble(etLowerLimit.getText());
            double currentEtUpper = Double.parseDouble(etUpperLimit.getText());

            mainGui.smartSensors.get(sensNr).m_measurments.get(measurmentNr).lowerLimit = currentEtLower;
            mainGui.smartSensors.get(sensNr).m_measurments.get(measurmentNr).upperLimit = currentEtUpper; 
        }
         
        //check GPS position options
        if(cbxDrawOnMap.isSelected())
        {
            Position pos = new Position(Double.parseDouble(etGPSLatitude.getText()), 
                                        Double.parseDouble(etGPSLongitude.getText()));
            mainGui.smartSensors.get(sensNr).setSensorPositionGPS(pos);
            mainGui.drawMap();
        }
    }//GEN-LAST:event_btnAcceptActionPerformed

    private void cbSensorListItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cbSensorListItemStateChanged
        // TODO add your handling code here:
        if(evt.getStateChange() == ItemEvent.DESELECTED)
        {
            fillMeasurmentsComboBox();
        }       
    }//GEN-LAST:event_cbSensorListItemStateChanged

    private void cbMeasurmentsListItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cbMeasurmentsListItemStateChanged
        // TODO add your handling code here:
        if(evt.getStateChange() == ItemEvent.DESELECTED)
        {
            setMeasurmentVauleControls();
        }
    }//GEN-LAST:event_cbMeasurmentsListItemStateChanged

    private void slUpperLimitStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_slUpperLimitStateChanged
        // TODO add your handling code here:
        if (!slUpperLimit.getValueIsAdjusting()) 
        {
            etUpperLimit.setText(Double.toString(slUpperLimit.getValue()));
            changedMeasParams = true;
        }
    }//GEN-LAST:event_slUpperLimitStateChanged

    private void slLowerLimitStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_slLowerLimitStateChanged
        // TODO add your handling code here:
        if (!slLowerLimit.getValueIsAdjusting()) 
        {
            etLowerLimit.setText(Double.toString(slLowerLimit.getValue()));
            changedMeasParams = true;
        }
    }//GEN-LAST:event_slLowerLimitStateChanged

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
            java.util.logging.Logger.getLogger(SensorProperities.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(SensorProperities.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(SensorProperities.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(SensorProperities.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
               // new SensorProperities().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnAccept;
    private javax.swing.JButton btnClose;
    private javax.swing.JComboBox<String> cbMeasurmentsList;
    private javax.swing.JComboBox<String> cbSensorList;
    private javax.swing.JCheckBox cbxDrawOnMap;
    private javax.swing.JCheckBox cbxEnableSleep;
    private javax.swing.JTextField etGPSLatitude;
    private javax.swing.JTextField etGPSLongitude;
    private javax.swing.JTextField etLowerLimit;
    private javax.swing.JTextField etUpperLimit;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JSlider slLowerLimit;
    private javax.swing.JSlider slSleepTime;
    private javax.swing.JSlider slUpperLimit;
    private javax.swing.JTextField txCurrentLowerLimit;
    private javax.swing.JTextField txCurrentUpperLimit;
    private javax.swing.JTextField txMeasurmentValue;
    private javax.swing.JTextField txSleepTime;
    // End of variables declaration//GEN-END:variables
}
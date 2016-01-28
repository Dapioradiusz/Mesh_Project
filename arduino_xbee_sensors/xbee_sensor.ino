
//#include <dht11.h> - library for temp and humid sensor
#include <Printers.h>
#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
//Create response object
ZBRxResponse rx64 = ZBRxResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
//Declare variables
unsigned long start = millis();
bool firstSend = true;

uint8_t const maxPayloadLength = 50;
uint8_t payload[maxPayloadLength];
const uint32_t HUB_MAC_MSB = 0x0013a200;
const uint32_t HUB_MAC_LSB = 0x40b49a62;
XBeeAddress64 addr64 = XBeeAddress64(HUB_MAC_MSB, HUB_MAC_LSB); // HUB adress
ZBTxRequest tx = ZBTxRequest(addr64, payload, sizeof(payload));

//signal xbee status pins
const uint8_t STATUS_LED = 10;
const uint8_t ERROR_LED = 9;

//analog devices pins
const uint8_t DHT11_SENSOR = 2;


//xbee payload tags
const String MEASURMENTS_START_TAG = "Measurments";
const String MEASURMENTS_END_TAG = "Ms_end";
const String MEASURMENT_START_TAG = "Measurment";
const String MEASURMENT_END_TAG = "M_end";
const String NAME_TAG = "name";
const String UNIT_TAG = "unit";
const String VALUE_TAG = "value";

bool finishedSendingPayload = true;
bool gotTxStatusResponse = false;

uint8_t numOfDataToSend = 0;
uint16_t data_size = 0;

String data = String();

void flashLed(int pin, int times, int wait) 
{
    for (int i = 0; i < times; i++) 
    {
        digitalWrite(pin, HIGH);
        delay(wait);
        digitalWrite(pin, LOW);
        
        if (i + 1 < times) 
        {
          delay(wait);
        }
    }
}

void addMeasurment(String &measurments,
                   const String &name, 
                   float value, 
                   const String &unit)
{
    measurments += MEASURMENT_START_TAG + "\n"
                 + NAME_TAG + " = '" + name + "'\n"
                 + VALUE_TAG + " = '" + value + "'\n"
                 + UNIT_TAG + " = '" + unit + "'\n"
                 + MEASURMENT_END_TAG + "\n";               
}

String getMeasurments()
{
    // int chk = DHT11.read(DHT11PIN);         
    // Serial.print("Stan sensora: ");
    // switch (chk)
    // {
    // case DHTLIB_OK: 
      // Serial.print("OKt"); 
      // break;
    // case DHTLIB_ERROR_CHECKSUM: 
      // Serial.println("Błąd sumy kontrolnej"); 
      // break;
    // case DHTLIB_ERROR_TIMEOUT: 
      // Serial.println("Koniec czasu oczekiwania - brak odpowiedzi"); 
      // break;
    // default: 
      // Serial.println("Nieznany błąd"); 
      // break;
    // }
    // Serial.print("Humidity(%): ");              
    // Serial.print((float)DHT11.humidity, 2);
    // Serial.print("Temperature (Celsius): ");           
    // Serial.println((float)DHT11.temperature, 2);
    
    //float tempValue = (float)DHT11.temperature;
    //float humiValue = (float)DHT11.humidity;
    
    //test
    float tempValue = 9.5;
    float humiValue = 70.0;
      
    String measurments = MEASURMENTS_START_TAG + "\n";
    addMeasurment(measurments, "Temperature", tempValue, "Celsius");
    addMeasurment(measurments, "Humidity", humiValue, "%");
    measurments += MEASURMENTS_END_TAG + "\n";
    Serial.println(measurments);
    return measurments;
}

void setup() 
{
    //Start Serial communication
    Serial.begin(9600);
    Serial.println("Xbee Sensor Start");
    xbee.setSerial(Serial);
}

void loop() 
{
  //RX
  //check is got response
  xbee.readPacket();
    
  if (xbee.getResponse().isAvailable()) 
  {
      //Got something
      Serial.println("Got response!");
      Serial.println("API ID: ");
      Serial.print(xbee.getResponse().getApiId(), HEX);
      
      if(xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
      {
          xbee.getResponse().getZBRxResponse(rx64);
      
          uint8_t *datarx = rx64.getData();
          uint8_t dataL = rx64.getDataLength();
        
          Serial.println("Data: \n");
          for (int i = 0; i < dataL; i++)
          {
            Serial.print(datarx[i]);
          }
      }
      
      //TX Status           
      if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) 
      {
        xbee.getResponse().getZBTxStatusResponse(txStatus);    
        // get the delivery status, the fifth byte
        if (txStatus.getDeliveryStatus() == SUCCESS) 
        {
            // success.  time to celebrate
            Serial.println("Got tx status response");
            gotTxStatusResponse = true;
            flashLed(STATUS_LED, 5, 50);
        } 
        else 
        {
            // the remote XBee did not receive our packet. is it powered on?
            flashLed(ERROR_LED, 4, 500);
        }
      }      
      
      else if(xbee.getResponse().isError())
      {
          Serial.println("Get Response Error!"); 
          flashLed(ERROR_LED, 2, 500);  
      }
  }
  
  
  //TX
  if(firstSend &&  ((millis() - start) >= 10000) )
  {
      data = getMeasurments();
      data_size = data.length() + 1; // + null terminator
      char data_Ch[data_size];
      data.toCharArray(data_Ch, data_size);
      if(data_size > maxPayloadLength)
      {
          finishedSendingPayload = false;
          numOfDataToSend = data_size;
          for(uint8_t i = 0; i < maxPayloadLength; i++)
          {
              payload[i] = data_Ch[i];
          }
          numOfDataToSend -= maxPayloadLength;
      }
      else
      {
          for(uint8_t i = 0; i < data_size; i++)
          {
              payload[i] = data_Ch[i];
          }  
      }
      Serial.println("Sending data ... ");     
      xbee.send(tx);   
      // flash TX indicator
      Serial.println("Success!"); 
      flashLed(STATUS_LED, 1, 2000);
      firstSend = false;
      start = millis();
  }
  else if(gotTxStatusResponse && ((millis() - start) >= 5000))
  {
      if(finishedSendingPayload)
      {
          data = getMeasurments();
          data_size = data.length() + 1; // + null terminator
          char data_Ch[data_size];
          data.toCharArray(data_Ch, data_size);
          if(data_size > maxPayloadLength)
          {
              finishedSendingPayload = false;
              numOfDataToSend = data_size;
              for(uint8_t i = 0; i < maxPayloadLength; i++)
              {
               payload[i] = data_Ch[i];
              }
              numOfDataToSend -= maxPayloadLength;
          }
          else
          {
              for(uint8_t i = 0; i < data_size; i++)
              {
                payload[i] = data_Ch[i];
              }
          }
      }
      else
      { 
          uint16_t data_size = data.length() + 1;
        
          char data_Ch[data_size];
          data.toCharArray(data_Ch, data_size);
          uint16_t data_index = data_size - numOfDataToSend;
          if(numOfDataToSend > maxPayloadLength)
          {
              for(uint16_t i = 0; i < maxPayloadLength; i++)
              {
                  payload[i] = data_Ch[i + data_index];
              }
              numOfDataToSend -= maxPayloadLength;
          }
          else
          {
              if(numOfDataToSend < MEASURMENTS_END_TAG.length()+1)
              {
                  uint16_t tag_size = MEASURMENTS_END_TAG.length()+1;
        
                  char data_Ch[tag_size];
                  MEASURMENTS_END_TAG.toCharArray(data_Ch, tag_size);
                  
                  for(uint8_t i = 0; i < tag_size; i++)
                  {
                      payload[i] = data_Ch[i];
                  }
              }
              else
              {
                  for(uint8_t i = 0; i < numOfDataToSend; i++)
                  {
                      payload[i] = data_Ch[i + data_index];
                  }
                
              }
              finishedSendingPayload = true;
          }   
      }
      Serial.println("Sending data ... ");     
      xbee.send(tx);
      start = millis();
      // flash TX indicator
      Serial.println("Success!"); 
      flashLed(STATUS_LED, 1, 2000);
      gotTxStatusResponse = false;
  }   
}

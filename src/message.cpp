/*
   message.cpp

   Code to handle sensor messages (gathering information, building and sending the message).
   A message is an 11 bytes structure containing 
    - Id of sensor
    - Battery Status (0:OK,1:Low)
    - Temperature (as a float)
    - Humidity (as a float)
*/

#include <Arduino.h>
#include "TempSensor.h"

//------------------------------------------------------------------------
// updateDHT
//   Read Temp sensor values and update temp and humi values in message
//   struct. 
//   Returns false if unable to read DHT
bool updateDHT(message *M) {
  float temp;
  float humi;

  Serial.println("-> updateDHT");
  delay(2200);
  temp = aht20.getTemperature();
  humi = aht20.getHumidity();

  if (isnan(temp) || isnan(humi)) 
    return false;
  else {
    M->t = temp;
    M->h = humi;
    Serial.print("Temp: "); Serial.println(M->t);
    Serial.print("Humi: "); Serial.println(M->h);
    return true;
  }
   
}

//------------------------------------------------------------------------
// updateMessage
//   Gather information from Battery and DHT sensor into a message struct
void updateMessage(message *M) {
  Serial.println("updateMessage");
  // checkBattery(M)
  if (!updateDHT(M)) {
    // Did not manage to read values from sensor, try one more time after delay
    delay(1000);
    updateDHT(M);
  }
}

//------------------------------------------------------------------------
// sendMessage
//   Sends message 'repeats' times.
void sendMessage(message *M,uint8_t repeats) {
  byte cpt;
  
  // noInterrupts();

  for (cpt=0; cpt<repeats; cpt++) {
    Serial.print("Sending attempt #"); Serial.println(cpt+1);
    rf_driver.send((uint8_t *)&M, sizeof(message));
    rf_driver.waitPacketSent();
  }
  delay(200);
  // interrupts();
}
#include <Arduino.h>
#include "TempSensor.h"

float humidity;
float temperature;
uint8_t batStatus;

message Msg;

// Variables for sleep mode
volatile bool watchdogActive = false;
byte sleepCycles = 0;

// Objects to manipulate FR Module (ASK algorithm) and Temp Sensor
RH_ASK rf_driver(BAUDRATE,-1,TXPIN,-1,0);
AHT20 aht20;

//------------------------------------------------------------------------
// Define watchdog timer interrupt action
ISR(WDT_vect) {
  watchdogActive = true; // set flag
}

void setup()
{
  watchdogActive = true;
  pinMode(LEDPIN,OUTPUT);
  Serial.begin(115200);	  // Debugging only
  Serial.println("Dans setup");

  if (!rf_driver.init())
    Serial.println("init failed");

  while (aht20.begin() != true) {
    Serial.println(F("AHT20 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    delay(5000);
  }
  // calibration();    // Calibrate battery readings ?
  Serial.println("Lance updataMessage");
  updateMessage(&Msg);  // Sefresh message data (bat,temp,humi)

  Serial.println("Lance sendMessage");
  sendMessage(&Msg,REP); // Send message on RF for 'REP' times

  setup_watchdog();    // Set 8s watchdog timer interrupt
}

void loop()
{
   
  if (watchdogActive) {  // if there was a watchdog wakeup
    // watchdogActive = false;  // reset the flag
    sleepCycles += 1;

    if (sleepCycles >= INTVAL/8) {
      updateMessage(&Msg);
      digitalWrite(LEDPIN,HIGH);
      sendMessage(&Msg,REP);
      digitalWrite(LEDPIN,LOW);

      sleepCycles = 0;  // reset counter
    }
  }

  sleep();  // go to sleep!

}

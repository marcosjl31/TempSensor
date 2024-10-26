#ifndef _TEMPSENSOR_H
  #define _TEMPSENSOR_H

  #include <Arduino.h>

  #include <avr/sleep.h>
  #include <avr/power.h>
  #include <avr/wdt.h>

  #include <RH_ASK.h>
  #ifdef RH_HAVE_HARDWARE_SPI
  #include <SPI.h> // Not actually used but needed to compile
  #endif

  // #include <Adafruit_Sensor.h>
  // #include <DHT.h>
  #include <AHT20.h>

  // Settings
  #define INTVAL 8                    // Data sent every N sec.
  #define REP 7                       // Message will be sent N times
  #define IDSENSOR 0x1965

  #define DHTPIN 2
  #define DHTTYPE DHT22
  #define TXPIN 12
  #define BAUDRATE 2000
  #define LEDPIN LED_BUILTIN

  // This is a 11 bytes message to be sent.
  struct message {
    uint16_t id=IDSENSOR;             // Id of temp sensor
    uint8_t bs=0;                     // Battery Status 1=Low
    float t=0.0;                      // Temperature
    float h=0.0;                      // Humidity
  };

  extern RH_ASK rf_driver;
  extern AHT20 aht20;  

  //------------------------------------------------------------------------
  // Forward declarations
  void sleep();
  void setup_watchdog();
  void updateMessage(message *M);
  void sendMessage(message *M,uint8_t xtimes);
  void checkBattery(message *M);
  bool updateDHT(message *M);
   
#endif
#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <AHT20.h>


#define CALIB_FACTOR 5.25       // change this value to calibrate the battery voltage
#define OP_VOLT 3.6             // (V) Minimum operational voltage allowed for the battery
#define WAKE_INTERVAL 20        // (mn) Send temp/humi/batt data 3 times per hour

WiFiManager wifiManager;
// [Please CHANGE THIS] - WiFi settings and local IP configuration
String staHostname = "TempSensor";
char static_ip[16] = "192.168.3.11";
char static_gw[16] = "192.168.3.1";
char static_mask[16] = "255.255.255.0";
char static_dns[16] = "192.168.3.1";
// [Please CHANGE This] - API ReST server to post data from TempSensor
String weatherStationServer = "http://192.168.3.10/post-data";

// Forward declarations
void goSleep(unsigned int sleepMinutes);
float round2(float value);


#endif

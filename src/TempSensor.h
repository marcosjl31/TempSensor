#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#define CALIB_FACTOR 5.18       // change this value to calibrate the battery voltage
#define OP_VOLT 3.6             // (V) Minimum operational voltage allowed for the battery
#define WAKE_INTERVAL 20        // (mn) Send temp/humi/batt data 3 times per hour


// [Please CHANGE THIS] - WiFi settings and local IP configuration
String staHostname = "TempSensor";

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

// [Please CHANGE This] - API ReST server to post data from TempSensor
String weatherStationServer = "http://192.168.3.10/post-data";

// Forward declarations
void goSleep(unsigned int sleepMinutes);
float round2(float value);


#endif

#include <Arduino.h>
#include "TempSensor.h"

//--- Vars related to http client request to post data to main weather station.
WiFiClient client;
HTTPClient http;
int httpResponseCode;

// Objects to manipulate AHT20 sensor
AHT20 aht20;
float temperature = 0.0;
float humidity =0.0;

//--- Battery related vars
float volt;
int batterypercentage;
unsigned long raw;

//--- JSON vars
JsonDocument jsonToSend;
String jsonToSendStr;

//--- Other vars
int i=0;

void setup()
{
  Serial.begin(74880);
  Serial.println("TempSensor starting");

  //--- Check Battery voltage first
  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k

  raw = analogRead(A0);
  volt = raw * CALIB_FACTOR / 1024;

  batterypercentage = (volt - OP_VOLT) * 100 / 0.6;   // OP_VOLT V is the lower limit set to 0%, bandwith 0.6 V
  if (batterypercentage > 100) batterypercentage = 100;
  
  //--- Connecting to Wifi
  wifiManager.setConfigPortalTimeout(5000);
  IPAddress _ip,_gw,_mask,_dns;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _mask.fromString(static_mask);
  _dns.fromString(static_dns);

  wifiManager.setHostname(staHostname);
  wifiManager.setSTAStaticIPConfig(_ip,_gw,_mask,_dns);

  if(!wifiManager.autoConnect("AP_WifiConfig","password")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  } 

  Serial.println("INFO: connected to WiFi");

  Wire.begin(); //Join I2C bus

  //Check if the AHT20 will acknowledge
  if (aht20.begin() == false)
  {
    Serial.println("ERROR: AHT20 not detected. Please check wiring. Hibernating...");
    goSleep(0);
  }
  Serial.println("INFO: AHT20 acknowledged.");
  delay(2000);
  //If a new measurement is available
  while (!aht20.available()){
    Serial.println("Waiting for temp sensor...");
    delay(2000);
  } 
  
  //Get the new temperature and humidity value
  temperature = aht20.getTemperature();
  humidity = aht20.getHumidity();

  //--- We've got temperature/humidity/batt.voltage data, let's send it to 
  //    Weather Station ReST API server.
  http.begin(client,weatherStationServer);
  http.addHeader("Content-Type","application/json");

  jsonToSend["temperature"] = round2(temperature);
  jsonToSend["humidity"] = round2(humidity);
  jsonToSend["batt_per"] = round2(batterypercentage);
  
  serializeJson(jsonToSend,jsonToSendStr);
  httpResponseCode = http.POST(jsonToSendStr);
  Serial.println("DEBUG: HTTP response code: " + String(httpResponseCode));
  http.end();

  // delay(20000);

  //--- go to sleep or hibernate depending on battery level
  if (volt > OP_VOLT)
    goSleep(WAKE_INTERVAL);
  else
    goSleep(0);
}

void loop()
{
}

//--- goSleep function
//    Go to deep sleep for sleepMinutes minutes, if 0, sleep forever ! (till reset button is pressed)
void goSleep(unsigned int sleepMinutes) {
  
  if (sleepMinutes > 0) {
    Serial.print("INFO: going to sleep now for ");
    Serial.print(sleepMinutes);
    Serial.println(" mn");
  } 
  else
    Serial.print("INFO: going to sleep forever");

  ESP.deepSleep(sleepMinutes * 60 * 1000000);

}

//--- Round a float to 2 dec.
float round2(float value){
  return ((value * 100 + 0.5) / 100.0);
}

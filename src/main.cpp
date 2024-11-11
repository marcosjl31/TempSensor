#include <Arduino.h>
#include "TempSensor.h"

float volt;
float calib_factor = 5.18; // change this value to calibrate the battery voltage
int batterypercentage;

void setup()
{
  Serial.begin(115200);
  Serial.println("Humidity AHT20 examples");

  Wire.begin(); //Join I2C bus

  //Check if the AHT20 will acknowledge
  if (aht20.begin() == false)
  {
    Serial.println("AHT20 not detected. Please check wiring. Freezing.");
    while (1);
  }
  Serial.println("AHT20 acknowledged.");
}

void loop()
{
  //If a new measurement is available
  if (aht20.available() == true)
  {
    //Get the new temperature and humidity value
    float temperature = aht20.getTemperature();
    float humidity = aht20.getHumidity();

    //Print the results
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" C\t");
    Serial.print("Humidity: ");
    Serial.print(humidity, 2);
    Serial.print("% RH");

    Serial.println();
  }

  //The AHT20 can respond with a reading every ~50ms. However, increased read time can cause the IC to heat around 1.0C above ambient.
  //The datasheet recommends reading every 2 seconds.
  delay(2000);

  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k

  unsigned long raw = analogRead(A0);
  volt = raw * calib_factor / 1024;

  Serial.print("Voltage = ");
  Serial.print(volt, 2); // print with 2 decimal places
  Serial.println (" V");

  batterypercentage = (volt - 3.6) * 100 / 0.6;   // 3.6 V is the lower limint set to 0%, bandwith 0.6 V
  if (batterypercentage > 100) batterypercentage = 100;
  Serial.print("Battery charge: ");
  Serial.print(batterypercentage);
  Serial.println("%");

}
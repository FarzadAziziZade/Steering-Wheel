#include <Wire.h>
#include <LSM303.h>
#include "GY521.h"

GY521 sensor(0x68);
uint32_t counter = 0;

LSM303 compass;
char report[80];

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  delay(100);
  while (sensor.wakeup() == false)
  {
    Serial.print(millis());
    Serial.println("\tCould not connect to GY521");
    delay(1000);
  }
  sensor.setAccelSensitivity(0);  // 2g
  sensor.setGyroSensitivity(0);   // 250 degrees/s
  sensor.setThrottle();
  Serial.println("start...");
  // set callibration values from calibration sketch.
  sensor.axe = 0;
  sensor.aye = 0;
  sensor.aze = 0;
  sensor.gxe = 0;
  sensor.gye = 0;
  sensor.gze = 0;
}

void loop()
{
  compass.read();
  
  sensor.read();
  int ax = sensor.getAccelX();
  int ay = sensor.getAccelY();
  int az = sensor.getAccelZ();
  int gx = sensor.getGyroX();
  int gy = sensor.getGyroY();
  int gz = sensor.getGyroZ();
  int t = sensor.getTemperature();

  if (counter % 10 == 0)
  {
    Serial.println("\n\tACCELEROMETER\t\tGYROSCOPE\t\tTEMPERATURE\t\tACCELEROMETER 2\t\tMAG");
    Serial.println("\tax\tay\taz\tgx\tgy\tgz\tT\tax2\tay2\taz2\tmaggx\tmaggy\tmaggz");
  }


  Serial.print(counter);
  Serial.print('\t');
  Serial.print(ax);
  Serial.print('\t');
  Serial.print(ay);
  Serial.print('\t');
  Serial.print(az);
  Serial.print('\t');
  Serial.print(gx);
  Serial.print('\t');
  Serial.print(gy);
  Serial.print('\t');
  Serial.print(gz);
  Serial.print('\t');
  Serial.print(t);
  Serial.print('\t');
  Serial.print(compass.a.x);
  Serial.print('\t');
  Serial.print(compass.a.y);
  Serial.print('\t');
  Serial.print(compass.a.z);
  Serial.print('\t');
  Serial.print(compass.m.x);
  Serial.print('\t');
  Serial.print(compass.m.y);
  Serial.print('\t');
  Serial.print(compass.m.z);
  Serial.println();
  counter++;
  
  delay(100);
}

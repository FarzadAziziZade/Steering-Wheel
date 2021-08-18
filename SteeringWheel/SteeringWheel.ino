#include <Wire.h>
#include <LSM303.h>
LSM303 compass;
char report[80];

uint32_t counter = 0;
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

float ay_percent;
float ax2_percent;

int p5 = 5;
int p6 = 6;

void setup()
{
  pinMode(p5, OUTPUT);
  pinMode(p6, OUTPUT);

  Serial.begin(115200);
  Wire.begin();
  compass.init();
  compass.enableDefault();

  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

}

void loop()
{
  compass.read();
delay(150);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  //temperature = Wire.read() << 8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  //gyro_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  //gyro_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  //gyro_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)


  //  if (counter % 10 == 0)
  //  {
  //    Serial.println("\n\tACCELEROMETER\t\tGYROSCOPE\t\tTEMPERATURE\t\tACCELEROMETER 2\t\tMAGNETOMETER");
  //    Serial.println("\tax\tay\taz\tgx\tgy\tgz\tT\t\t\tax2\tay2\taz2\tmagx\tmagy\tmagz");
  //  }
  //
  //
  //  Serial.print(counter);
  //  Serial.print('\t');
  //  Serial.print(convert_int16_to_str(accelerometer_x));
  //  Serial.print('\t');
  //  Serial.print(convert_int16_to_str(accelerometer_y));
  //  Serial.print('\t');
  //  Serial.print(convert_int16_to_str(accelerometer_z));
  //  Serial.print('\t');
  //  Serial.print(convert_int16_to_str(gyro_x));
  //  Serial.print('\t');
  //  Serial.print(convert_int16_to_str(gyro_y));
  //  Serial.print('\t');
  //  Serial.print(convert_int16_to_str(gyro_z));
  //  Serial.print('\t');
  //  Serial.print(temperature / 340.00 + 36.53);
  //  Serial.print('\t');
  //  Serial.print('\t');
  //  Serial.print('\t');
  //  Serial.print(compass.a.x);
  //  Serial.print('\t');
  //  Serial.print(compass.a.y);
  //  Serial.print('\t');
  //  Serial.print(compass.a.z);
  //  Serial.print('\t');
  //  Serial.print(compass.m.x);
  //  Serial.print('\t');
  //  Serial.print(compass.m.y);
  //  Serial.print('\t');
  //  Serial.print(compass.m.z);
  //  Serial.println();
  //  counter++;
 
  ay_percent = 100*((abs(float(accelerometer_y) - 13500)) / (27000));
  ax2_percent = 100*((float(compass.a.x) + 15600) / (31200));

  Serial.print(ay_percent);
  Serial.print('\t');
  Serial.print(ax2_percent);
  Serial.print('\t');
  Serial.print(abs(ay_percent - ax2_percent));
  Serial.println();


  if (ay_percent > ax2_percent) {
    //left

    if (abs(ay_percent - ax2_percent) > 5) {
      if (ay_percent > 1) {
        digitalWrite(p5, HIGH);
        digitalWrite(p6, LOW);
        delay(50);
        digitalWrite(p5, LOW);
        digitalWrite(p6, LOW);
      }
    }
  }

  if (ay_percent < ax2_percent) {
    //right
    if (ay_percent < 99) {
      if (abs(ay_percent - ax2_percent) > 5) {
        digitalWrite(p6, HIGH);
        digitalWrite(p5, LOW);
        delay(50);
        digitalWrite(p5, LOW);
        digitalWrite(p6, LOW);
      }
    }
  }
}

/*
 * MPU6050_Read_Write_Registers.ino
 * Copyright 2022 by David G. Sparks
 * All Rights Reserved
 * 
 * Read data from the hardware registers of a MPU6050 Gyroscope/Accelerometer, 
 * and write data to it. Use only the "twi" software library of the Arduino IDE, 
 * but not any device-specific library.
 *
 * Display a register's value as individual bits,
 * represented as a string of eight characters, '0' or '1'.
 *
 * LICENSE
 *
 * DS3231_Read_Write_Registers.ino ("this program") is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see <https://www.gnu.org/licenses/>.
 * 
 * Thanks to "Dejan" for an example of starting-up the MPU6050, see
 * https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
 */

// #include "twi.h"

#include "regByteTools.h"

const int MPU6050_address = 0x69;

// MPU6050 startup registers
const int GYRO_CONFIG = 0x1b;
const int ACCEL_CONFIG = 0x1c;
const int PWR_MGMT_1 = 0x6b;


// test variable
int theTemperature;
 
void setup() {
  // put your setup code here, to run once:

  Serial.begin (115200);
  twi_init();

  // initialize MPU6050

  // reset the power management register
  setRegister(MPU6050_address, PWR_MGMT_1, 0); // Make reset - place a 0 into the 6B register
  // configure accelerometer sensitivity
  setRegister(MPU6050_address, ACCEL_CONFIG, 0x10); // Full Scale Range (default +/- 2g)
  // configure gyroscope sensitivity
  setRegister(MPU6050_address, GYRO_CONFIG, 0x10); // Full Scale Range (default +/- 250deg/s)
  
  Serial.println("\n\n  MPU6050 Registers\n");
  
  // put the MSB of the temperature into the MSB of the variable
  // and display the bits in the register
  theTemperature = displayRegister (MPU6050_address, "Temp_out_H (0x41): ", 0x41) << 8;

  // incorporate the LSB of the temperature
  // and display the bits in the register
  theTemperature |= displayRegister (MPU6050_address, "Temp_out_L (0x42) ", 0x42);

  // display the variable adjusted to correct temperature
  // see "MPU-6000 and MPU-6050 Register Map and Descriptions" page 30
  double adjustedTemperature = 36.53 + ( (double) theTemperature) / 340.0;

  // comment: this came out to 26.53 degrees Celsius in my kitchen,
  // = about 80 degrees Fahrenheit, while at the time, 
  // the kitchen was actually around 69 F.

  Serial.println();
  Serial.println(adjustedTemperature);
  

}

void loop() {
  // nothing happens in the loop
}

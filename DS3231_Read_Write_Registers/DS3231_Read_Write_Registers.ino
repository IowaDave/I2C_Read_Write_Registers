/*
 * DS3231_Read_Write_Registers.ino
 * Copyright 2022 by David G. Sparks
 * All Rights Reserved
 * 
 * Read data from the hardware registers of a DS3231 Real Time Clock, 
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
 */

// #include "twi.h"

#include "regByteTools.h"

const int DS3231_address = 0x68;

void setup() {
  // put your setup code here, to run once:

  Serial.begin (115200);
  twi_init();
  
  Serial.println("\n\n  DS3231 Registers\n");
  
  displayRegister (DS3231_address, "Time seconds: ", 0x00);
//  Serial.println();

  displayRegister (DS3231_address, "Time minutes: ", 0x01);
//  Serial.println();

  displayRegister (DS3231_address, "Time hours: ", 0x02);
//  Serial.println();

  displayRegister (DS3231_address, "Time day: ", 0x03);
//  Serial.println();

  displayRegister (DS3231_address, "Time date: ", 0x04);
//  Serial.println();

  displayRegister (DS3231_address, "Time month: ", 0x05);
//  Serial.println();

  displayRegister (DS3231_address, "Time Year:", 0x06);
//  Serial.println();
  
  displayRegister (DS3231_address, "Alarm 1 seconds: ", 0x07);
//  Serial.println();

  displayRegister (DS3231_address, "Alarm 1 minutes: ", 0x08);
//  Serial.println();

  displayRegister (DS3231_address, "Alarm 1 hour: ", 0x09);
//  Serial.println();

  displayRegister (DS3231_address, "Alarm 1 day/date: ", 0x0a);
//  Serial.println();

  uint8_t originalA2Minutes 
  = displayRegister (DS3231_address, "Alarm 2 minutes: ", 0x0b);
//  Serial.println();

  displayRegister (DS3231_address, "Alarm 2 hour: ", 0x0c);
//  Serial.println();

  displayRegister (DS3231_address, "Alarm 2 day/date: ", 0x0d);
//  Serial.println();

  displayRegister (DS3231_address, "Control 0x0e: ", 0x0e);
//  Serial.println();

  displayRegister (DS3231_address, "Control status 0x0f: ", 0x0f);
//  Serial.println();

  displayRegister (DS3231_address, "Temperature MSB: ", 0x11);
//  Serial.println();

  displayRegister (DS3231_address, "Temperature LSB: ",0x12);
//  Serial.println();


  // randomly set new value for Alarm 2 minutes
  uint8_t newMin; 
  
  /* Tested with AVR-based Arduino Nano */

  // seed the rng
  randomSeed(analogRead(A0));
  // get a new, different value, 0..11 inclusive
  while ((newMin = (uint8_t) random(0,12)) 
        == originalA2Minutes) { } // try again

  setRegister (DS3231_address, 0x0b, newMin);

  displayRegister (DS3231_address, "New Alarm 2 minutes: ",0x0b);
//  Serial.println();
  
}

void loop() {
  // nothing happens in the loop
}

/*
 * DS3231_Read_Write_Registers.ino
 * Copyright 2022 by David G. Sparks
 * All Rights Reserved
 * 
 * Read data from the hardware registers of a DS3231 Real Time Clock, 
 * and write data to it. Use only the "twi" software library of the Arduino IDE, 
 * but not any device-specific library.
 *
 * Display a register's value as a string of eight characters, '0' or '1'.
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

#include "twi.h"

/*
 * Early work: Buffer and function for converting an 8-bit byte
 * into a string of eight characters, '0' or '1'.
 * Because sprintf() does not perform bitwise formatting.
 * 
 * An adaptation of this code segment is used
 * in the displayRegister() function, below.
 * 
 *
 * char bitString[9]; // one more than string length
 *
 * void byteToBinaryString ( char * buf, byte b ) {
 *   for (int i = 7; i >= 0; i--) {
 *     buf[i] = (b & 1) == 0  ? '0' : '1';
 *     b >>= 1;
 *   }
 *   buf[8] = '\0'; // ensure null terminator
 * }
 */

uint8_t getRegister ( uint8_t regAddress ) {
  uint8_t regBuffer[1]; // yes, one byte only
  regBuffer[0] = regAddress;
  twi_writeTo(0x68, regBuffer, 1, true, false); // blocking, don't send stop
  twi_readFrom(0x68, regBuffer, 1, true); // always blocking, send stop
  return regBuffer[0];
}

void setRegister (uint8_t regAddress, uint8_t valueToSet) {
  uint8_t regBuffer[2]; // yes, two bytes
  regBuffer[0] = regAddress;
  regBuffer[1] = valueToSet;
  twi_writeTo(0x68, regBuffer, 2, true, true); // blocking, send stop
}

/*
 * Display the value of a given register,
 * preceded by a label string,
 * and return that value to the caller
 */
uint8_t displayRegister (char * label, uint8_t registerAddress) {
  // get the value from the register
  uint8_t regValue = getRegister( registerAddress );
  // save it to be returned
  uint8_t returnValue = regValue;
  // initialize a string for display
  char regString[] = "  :                          ";
  // insert the register address 0 .. 99 into the string
  // ones digit
  regString[1] = '0' + (registerAddress % 10);
  // tens digit
  regString[0] = '0' + (registerAddress / 10);
  // insert characters representing the bits
  for (int i = 25; i > 3; i-=3) {
    regString[i] = (regValue & 1) == 0 ? '0' : '1';
    regValue >>= 1;
  }
  // display the label on a new line and then the string
  Serial.println(); // blank line
  Serial.print("  "); // offset two spaces from left edge of window
  Serial.println(label);
  Serial.print("  "); // offset two spaces from left edge of window
  Serial.println(regString);

  // return the original register value
  return returnValue;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin (115200);
  twi_init();
  
  Serial.println("\n\n  DS3231 Registers\n");
  
  displayRegister ("Time seconds: ", 0x00);
//  Serial.println();

  displayRegister ("Time minutes: ", 0x01);
//  Serial.println();

  displayRegister ("Time hours: ", 0x02);
//  Serial.println();

  displayRegister ("Time day: ", 0x03);
//  Serial.println();

  displayRegister ("Time date: ", 0x04);
//  Serial.println();

  displayRegister ("Time month: ", 0x05);
//  Serial.println();

  displayRegister ("Time Year:", 0x06);
//  Serial.println();
  
  displayRegister ("Alarm 1 seconds: ", 0x07);
//  Serial.println();

  displayRegister ("Alarm 1 minutes: ", 0x08);
//  Serial.println();

  displayRegister ("Alarm 1 hour: ", 0x09);
//  Serial.println();

  displayRegister ("Alarm 1 day/date: ", 0x0a);
//  Serial.println();

  uint8_t originalA2Minutes 
  = displayRegister ("Alarm 2 minutes: ", 0x0b);
//  Serial.println();

  displayRegister ("Alarm 2 hour: ", 0x0c);
//  Serial.println();

  displayRegister ("Alarm 2 day/date: ", 0x0d);
//  Serial.println();

  displayRegister ("Control 0x0e: ", 0x0e);
//  Serial.println();

  displayRegister ("Control status 0x0f: ", 0x0f);
//  Serial.println();

  displayRegister ("Temperature MSB: ", 0x11);
//  Serial.println();

  displayRegister ("Temperature LSB: ",0x12);
//  Serial.println();


  // randomly set new value for Alarm 2 minutes
  uint8_t newMin;
  
  /* Arduino Method*/
  // seed the rng
  randomSeed(analogRead(A0));
  // get a new, different value
  while ((newMin = (uint8_t) random(0,12)) 
        == originalA2Minutes) { } // try again
  /*
   * ESP 8266 method
   * newMin = ESP.random() % 12; // 0 .. 11
   */

  setRegister (0x0b, newMin);

  displayRegister ("New Alarm 2 minutes: ",0x0b);
//  Serial.println();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

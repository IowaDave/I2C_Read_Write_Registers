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


/*
 * Fetch a one-byte value from a register of an I2C peripheral
 * Parameters: i2c address, register number
 * Returns: the value obtained from the peripheral
 * Asserts: i2c address and register number are valid (not checked)
 * Side effects: none
 */
uint8_t getRegister ( uint8_t i2cAddress, uint8_t registerAddress ) {
  uint8_t regBuffer[1]; // one byte only, used both to send and to receive
  regBuffer[0] = registerAddress;
  // tell device which register to read from
  twi_writeTo(i2cAddress, regBuffer, 1, true, false); // true=blocking, false=don't send stop
  // tell device to send the value in that register
  twi_readFrom(i2cAddress, regBuffer, 1, true); // always blocking, true=send stop
  return regBuffer[0];
}

/*
 * Send a one-byte value to be be stored in a register of an I2C peripheral.
 * Parameters: i2c address, register number, the byte to be stored
 * Returns: nothing
 * Asserts: the i2c and register addresses are valid (not checked)
 * Side effects: none
 */
void setRegister (uint8_t i2cAddress, uint8_t registerAddress, uint8_t valueToSet) {
  uint8_t regBuffer[2]; // two bytes needed for send
  regBuffer[0] = registerAddress;
  regBuffer[1] = valueToSet;
  twi_writeTo(i2cAddress, regBuffer, 2, true, true); // true=blocking, true=send stop
}

/*
 * Display the value of a given register,
 * preceded by a label string,
 * and return that value to the caller.
 * Parameters: i2c address, label string, register number.
 * Returns: the value obtrained from the register.
 * Asserts: i2c and register addresses are valid (not checked)
 * Side effects: none
 */
uint8_t displayRegister (uint8_t i2cAddress, char * label, uint8_t registerAddress) {
  // get the value from the register
  uint8_t regValue = getRegister( i2cAddress, registerAddress );
  // save it to be returned
  uint8_t returnValue = regValue;
  // initialize a string for display
  // Note, it uses a C-type string, which means
  // the code writer must ensure sufficient size.
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

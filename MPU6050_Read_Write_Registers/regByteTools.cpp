#include "Arduino.h" // for uint8_t
#include "regByteTools.h"


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
 * Side effects: Serial output
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


 /* Early work: Buffer and function for converting an 8-bit byte
  * into a string of eight characters, '0' or '1'.
  * Because sprintf() does not perform bitwise formatting.
  * 
  * An adaptation of this code segment is used
  * in the displayRegister() function, below.
  * 
  * The parameter, buf, expects to point to a 9-char array, e.g.,
  * char bitString[9]; // one more than string length
  */

/*
 * Fill a char array with digits '0' and '1'
 * representing the bits in a byte
 * Parameters: pointer to a 9-char string, byte
 * Asserts: string pointed to by buf is at least 9 chars long (not checked)
 * Returns: nothing
 * Side effects: modifies contents of the string pointed to by buf
 */
void byteToBinaryString ( char * buf, byte b ) {
  for (int i = 7; i >= 0; i--) {
    buf[i] = (b & 1) == 0  ? '0' : '1';
    b >>= 1;
  }
  buf[8] = '\0'; // ensure null terminator
}

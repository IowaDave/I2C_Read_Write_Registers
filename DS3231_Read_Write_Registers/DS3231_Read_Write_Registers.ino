#include <Wire.h>

/*
 * Buffer and function for converting an 8-bit byte
 * into a string of bit characters.
 * 
 * Later I adapted this code segment into the
 * displayRegister() function, below.
 * 
char bitString[9]; // one more than string length


void byteToBinaryString ( char * buf, byte b ) {
  for (int i = 7; i >= 0; i--) {
    buf[i] = (b & 1) == 0  ? '0' : '1';
    b >>= 1;
  }
  buf[8] = '\0'; // ensure null terminator
}
*/

uint8_t getRegister ( uint8_t regAddress ) {
  uint8_t regValue;
  Wire.beginTransmission(0x68); // DS3231 I2C address
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1); // ask for one byte
  while (Wire.available()) {
    regValue = Wire.read();
  }
  return regValue;
}

/*
 * Display the value o given register
 * and return the value to the caller
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
  // display the label and then the string
  Serial.println(); // blank line
  Serial.print("  "); // two spaces from left edge
  Serial.println(label);
  Serial.print("  "); // two spaces from left edge
  Serial.println(regString);

  // return the original register value
  return returnValue;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin (115200);
  Wire.begin();
  
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
  
  /* Arduino Method
  // seed the rng
  // randomSeed(analogRead(A0));
  // get a new, different value
  //while ((newMin = (uint8_t) random(0,12)) 
  //      == originalA2Minutes) { } // try again
  */
  
  /*
   * ESP 8266 method
   */
  newMin = ESP.random() % 12; // 0 .. 11

 
  // set the new value
  Wire.beginTransmission(0x68);
  Wire.write(0x0b);
  Wire.write(newMin);
  Wire.endTransmission();

  displayRegister ("New Alarm 2 minutes: ",0x0b);
//  Serial.println();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

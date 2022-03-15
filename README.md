# I2C_Read_Write_Registers
Control I2C devices directly through hardware registers.

Note to self: I2C is a lot simpler than I thought. If you think of an I2C device as memory, then everything about it makes sense. For communicating with an external device, your code need only to take one or the other of two actions:

1. Store bytes into memory located on the device.
2. Retrieve bytes from that memory.

The code in the example Arduino project, DS3231_Read_Write_Registers, includes short functions that perform the two actions for a single byte. They are designed to be universal, for use with any I2C peripheral. 

Simple? Really? OK, what about those bytes? Hmm... yes, details matter. More on that a little farther down.

The memory locations of an I2C peripheral are called "registers." Registers are identified by numbers. One register can store one byte = 8 bits of information. The example code demonstrates a function designed to display a register's bits as a string of 1s and 0s.

For example, the bits representing the "Minutes" value of the current time can be read from Register 1 of the DS3231.

![Bits from Register 1](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/Reg_1_Time_Minutes.png)<br>**Figure 1: Bits in Register 1**

The "Minutes" bits are arranged in a special way, called Binary Coded Decimal. Each "nybble" of 4 bits forms one digit of a two-digit value. In the example shown above, the "high nybble", 0010, represents 2 and the "low nybble", 1001, represents 9. Combined, they encode the value 29. The image in Figure 1 was captured at 29 minutes past the hour. 

The [Maxim Datasheet for DS3231](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf) details the arrangement of the bits in all 18 of the accessible registers on pages 11-15. Having the datasheet in hand, plus a way to reveal the individual bits of a register, you can see exactly what's going on in there.

Frequently, the bits in a register provide more than one piece of information. Continuing with the DS3231, the bits representing the "Hour" value of the current time can be read from Register 2. These bits include not only the number but also information about whether it is in 12- or 24-hour format, and if 12-hour, then whether it is AM or PM.

![Bits from Register 2](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/Reg_2_Time_Hour.png)<br>**Figure 2: Bits in Register 2**

Referring to the datasheet, we examine bits 6, 5, and 4 in Figure 2. Bit 6 is the 12/24-hour format flag. It is 0, which means the DS3231 is operating in 24-hour mode. Bit 5 is the "20" bit when in 24-hour mode. It is 0, which means the hour is between 0 and 19. Bit 4 is the "10" bit. It is 1, which means we add 10 to the number in the low nybble. That value is 0010, representing 2. Thus the hour is 10 + 2 = 12. 

Combining Register 2 with Register 1, the DS3231 tells us that the time is 12:29, in 24-hour format, meaning 12:29 p.m.

Bits in a register might control how the peripheral operates. For example, the DS3231 provides two Alarms. Enabling or disabling the alarms is done by writing 1 or 0, respectively, to certain bits in Register 14, the "Control" register:

![Bits from Register 14](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/Reg_14_Control.png)<br>**Figure 3: Bits in Register 14**

Bits 1 and 0, the least-significant bits, at the right-hand side of Figure 3, are the alarm-enable bits for alarms 2 and 1, respectively. Both of them have the value, 1, meaning the alarms are "set", or enabled. Writing a 0 to those bits would disable the respective alarms.

The DS3231 can generate interrupts to trigger actions on your Arduino. Other bits that your code can manipulate in Registers 14 and 15 control how the DS3231 performs interrupts.

![Bits from Register 15](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/Reg_15_Control:Flags.png)<br>**Figure 4: Bits in Register 15**

Bits 1 and 0 in Register 15 are the alarm flags. They indicate the occurrence of Alarm 2 and Alarm 1, respectively. They are both 0 in Figure 4, which means no alarm event has happened. 

Registers 7 through 15 all participate in regulating the alarm features of the DS3231. Your code can set times and manage the alarms by writing properly-formatted bytes to the registers. The example code demonstrates changing the "Minutes" register for Alarm 2. 

As I write this in March, 2022, I have just begun to communicate with I2C devices by reading and writing the actual bits in registers directly. In the past, I felt limited to using third-party libraries. Too often, I'd feel disappointed with some aspect of a library. 

Also I'd feel frustrated that the Arduino libraries somewhat "hide" the hardware from me. The DS3231 is an example of this. The popular "NorthernWidget" library for the device provides no direct way to examine the actual bits in a register. Certain of the alarm bits are managed only as a side effect of a function designed to do something else. This code design decision can make certain kinds of troubleshooting difficult.

By contrast, when I go directly to the registers, I learn to see and to decide what they actually contain.

Many thanks to Tom Almy and Norman Dunbar for their Arduino books. They showed me the way.

The example project, DS3231_Read_Write_Registers, works with a DS3231 Real Time Clock having the I2C address 0x68. It reads the time and temperature registers, then changes one of them and reads it a second time.

### The Project Board

The DS3231 module used in this project came in one of those 37-Sensors kits. Another I2C module in there was a GY-571 board mounting an MPU-6050 accelerometer and gyroscope.  I installed both modules onto a breadboard, with an I2C connection to a Nano knock-off. 

![Project board showing components](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/populated.jpg)<br>**Figure 5: Project board showing components**

The yellow and white wires in Figure 5 carry the I2C SDA and SCL lines from pins A4 and A5 of the Nano. The blue wires are positioned to bring interrupt signals from the devices back to pins 2 and 3, when I choose to enable them. 

The DS3231 module is on the left in Figure 5; the GY-521 is in the center; and the Nano is on the right.

![Project board showing only the wiring](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/bareboard.jpg)<br>**Figure 6: Project board wiring**

Figure 6 removes the components to reveal the wiring underneath. There is a pull-up resistor at position 36 on the breadboard. It connects to the AD0 terminal of the GY-521 module, changing its I2C address to 0x69 from the default 0x68. This was necessary because the DS3231 has an I2C address of 0x68 which cannot be changed.

![I2C Bus scan](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/I2C_Bus_Scan.png)<br>**Figure 7: I2C Bus scan**

A scan of the I2C bus identifies three attached devices:

 * 0x50 the 24LC32 EEPROM on the DS3231 module
 * 0x68 the DS3231 Real Time Clock
 * 0x69 the MPU-6050 gyroscope/accelerometer on the GY-521 module

I look forward to learning my way around the registers on these three devices, one byteful of bits at a time.

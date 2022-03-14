# I2C_Read_Write_Registers
Control I2C devices directly through hardware registers.

Note to self: I2C is a lot simpler than I thought. If you think of an I2C device as memory, then everything about it makes sense. For communicating with an external device, your code need only to take one or the other of two actions:

1. Store bytes into memory located on the device.
2. Retrieve bytes from that memory.

The code in the example Arduino project, DS3231_Read_Write_Registers, includes short functions that perform the two actions for a single byte. They are designed to be universal, for use with any I2C peripheral. 

Simple? Really? OK, what about those bytes? Hmm... yes, details matter. More on that a little farther down.

The memory locations of an I2C peripheral are called "registers." Registers are identified by numbers. One register can store one byte = 8 bits of information. The example code demonstrates a function designed to display a register's bits as a string of 1s and 0s.

For example, the bits representing the "Minutes" value of the current time can be read from Register 1 of the DS3231.

![Bits from Register 1](https://github.com/IowaDave/I2C_Read_Write_Registers/blob/main/Images/Reg_1_Time_Minutes.png)

The "Minutes" bits are arranged in a special way, called Binary Coded Decimal. Each "nybble" of 4 bits forms one digit of a two-digit value. In the example shown above, the "high nybble", 0010, represents 2 and the "low nybble", 1001, represents 9. Combined, they encode the value 29. This image was captured at 29 minutes past the hour. 

You can see the exact arrangement of Register 1 in the [Maxim Datasheet for DS3231](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf), page 11. 

The datasheet details all 18 of the accessible registers on pages 11-15. Having the datasheet in hand, plus a way to reveal the individual bits of a register, you can see exactly what's going on in there.

Frequently, the bits in a register provide more than one piece of information. Continuing with the DS3231, the bits representing the "Hour" value of the current time can be read from Register 2. These bits include not only the number but also information about whether it is in 12- or 24-hour format, and if 12-hour, then whether it is AM or PM.

Bits in a register might control how the peripheral operates. For example, the DS3231 provides two Alarms. Enabling or disabling the alarms is done by writing 1 or 0, respectively, to certain bits in Register 14. The DS3231 can generate interrupts to trigger actions on your Arduino. Other bits that your code can manipulate in Registers 14 and 15 control how the DS3231 performs interrupts.

As I write this in March, 2022, I have just begun to communicate with I2C devices by reading and writing the actual bits in registers directly. In the past, I felt limited to using third-party libraries. Too often, I'd feel disappointed with some aspect of a library. 

Also I'd feel frustrated that the Arduino libraries somewhat "hide" the hardware from me. The DS3231 is an example of this. The popular "NorthernWidget" library for the device provides no direct way to examine the actual bits in a register. Certain of the alarm bits are managed only as a side effect of a function designed to do something else. This code design decision can make certain kinds of troubleshooting difficult.

By contrast, when I go directly to the registers, I learn to see and to decide what they actually contain.

Many thanks to Tom Almy and Norman Dunbar for their Arduino books. They showed me the way.

The example project, DS3231_Read_Write_Registers, works with a DS3231 Real Time Clock having the I2C address 0x68. It reads the time and temperature registers, then changes one of them and reads it a second time.

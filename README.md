# I2C_Read_Write_Registers
Control I2C devices directly through hardware registers.

Note to self: I2C is a lot simpler than I thought. It is an easy way to store data into, and to read data from, memory addresses located on an external device.

Really! If you think of an I2C device as memory, then everything about it makes sense.

The memory locations of an I2C peripheral are called "registers." Registers are identified by numbers. One register can store one byte = 8 bits of information. Frequently, the bits in a register provide more than one piece of information. 

Bits in one register might control how the peripheral operates. For example, a single bit in one register might specify reporting temperature in Fahrenheit or Celsius. Bits in another register might provide a value that the peripheral has obtained, e.g., a temperature. The datasheet for the peripheral details the meaning of each bit or group of bits in each register.

As I write this in March, 2022, I have just begun to communicate with I2C devices by reading and writing the actual bits in registers directly. In the past, I would find and learn to use a third-party library. Too often, I'd feel disappointed with some aspect of the library. Also I'd feel frustrated that the Arduino libraries somewhat "hide" the hardware from me.

By contrast, when I direct to the registers, I learn to see what is actually going on in there.

Many thanks to Tom Almy and Norman Dunbar for their books. They showed me the way.

The example project, DS3231_Read_Write_Registers, works with a DS3231 Real Time Clock having the I2C address, 0x68. It reads the time and temperature registers, then changes one of them and reads it a second time.

Refer to the [Maxim Datasheet for DS3231](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf), pages 11 to 15, for details about the registers.

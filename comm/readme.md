# GMP module of Communication

The GMP's communication module, known as "comm", offers a collection of streamlined communication routines. 
This module is structured into multiple classes. 
These communication classes are tailored to different chips through the BSP.

## command device
At the core of the library is the command class, that is `cmd_dev`. 
All the classes are derived from the `cmd_dev` class. 
The `cmd_dev` class provides a virtual function called `cmd()` which serves as a means of reacting commands. 
Users can utilize the `cmd_dev::cmd()` function to send messages to an object. 
The `cmd_dev::cmd()` function has two different prototypes, as shown below:

``` C++
virtual gmp_stat_t cmd(uint32_t cmd);
virtual gmp_stat_t cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam);

```

>  parameter description
> `cmd` the command index, this command index is a unique 32 bits unsigned int number,
> which was defined in several header files. Because all the classes has it's unique command.
> `wparam` (word parameter) is a unsigned int 32 number, as a parameter of the command.
> `lparam` (long pointer parameter) is a pointer of parameters of command.
> generally, this parameter point to a predefined structure which contains all the information 
> that the command need.
>
> Majority of command was react by two prototype. When a command doesn't specify a param, usually 
> the parameter will set as default.
>
> return value description
> 







This module provide plenty of libraries of chip communication. This module may divide into three parts.

+ Basement: the implement of communication setup and data transmitting or receiving.

This part of code may completed in the folder of `GMP/bsp/\<your main chip\>/\<Your board description\>`.

This part of code should be provided by board creator. By deriving the class in `GMP/comm/comm_basic.h`.

Such as The chip support you to implement a IIC communication line, then you should derive the class named `i2c_device`, which was defined in `comm_basic.h`. The class named `i2c_device` provided you a list of common function or common usage of i2c, for instance change baud-rate, and these function wouldn't be implemented in `i2c_device` these functions would be implemented in BSP file.  Because peripherals  are usually  strongly correlated with the chip you choose. Meanwhile, you should implement not only standard functions, but also some special and unique function that the chip has.

Note: GPIO is a special communication device. You may only transmit 1 or 0. Just like what Linux done, everything is a file. In the library, every peripheral is treated as a device. 

+ Application middleware: the application layer of the communication process.

This part of code may implement some sensor chip function such as temperature sensor, lightness sensor, humidity sensor; storage chip for instance AT24C series EEPROM, W25Q series SPI  FLASH .

In this part, basement would be involved into the sensor class or storage class. Every time the communication happened the basement communication class would be invoked.

+ User Application: User may use application middleware directly, and implement your whole goal.

As a user, you should know one thing, that is, clarifying which physical device is connected to which bus. And what you need to do is to attach the device and the bus. the device class stores a pointer to a general bus. What you've done indeed is to let the bus pointer point to the derived bus class object.





# Device Manager

This is a key part of the GMP library. This module provided a easy magagement way for users.
Just like what the device manager do in Windows OS, this module collection all the pheripheral information 
and all the outer hardware information, and manage them automatically, meanwhile provided a easiest way to 
implement RPC(remote process call).

We strongly recommend that user may use the tool, `dev_mgr`. And if you need to use the RPC functionality provided in this library,
you have no choice to use this class.
 

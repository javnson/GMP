# GMP IAP submodule

In the field of Application Programming (IAP), it is often necessary to implement an IAP submodule. 
Thankfully, the GMP has developed a sophisticated communication module that makes it easy to do so.

The IAP module consists of two main processes. 

The first process involves loading the program into either RAM or FLASH memory. 
Typically, the program is loaded into FLASH memory. 
In this process, the user needs to handle received messages and write the pure program text into FLASH memory. 
After that, the user needs to check the validation code, such as the CRC, to ensure that the program has been loaded correctly.

At a certain point, the resolve routine becomes a protocol. 
This protocol involves calling the `filter` object to write the content to FLASH. 
To accomplish this, the user must call the `attach()` function to connect a FLASH hardware device with the IAP class. 
Additionally, the protocol provides a toolkit that the user can utilize to check the validation code and resolve messages from the stream.

The second process has only one thing to do. 
User should transfer execution permissions to the loaded program.
This library will provide user a function prototype of transfer permissions,
and meanwhile provide a series of examples.


# controller interface submodule

This submodule provided a interface classes for controller.
These classes would be invoked in `gmp\ctrl` controller module.

The functional description of submodule is as follows:

+ ADC submodule

All the settings for the ADC are stored in the `adc_header.h` file. 
This file contains a function to configure all the channels and another function to read the results from the ADC. 
Each sample result corresponds to a specific physical measurement, by an mapping relationship. 
These results are used to calculate state variables such as control plant current, speed, voltage, position, safety range, etc. 
Alternatively, these values can be used as feedback for implementing a closed-loop control system. 
Overall, this submodule provides input values for a control or interactive system.

+ Encoder submodule

GMP is directly involved in the field of motor control. 
A crucial component of motor control is the angle measurement module, which is known as an encoder. 
Encoders can be used as feedback for various controllers such as speed controllers, 
position controllers, and coordinate transformation. 
The `encoder_header.h` provides an overview of encoders. 
Encoders encompass different types of physical sensors including discrete position sensors (QEP, HALL, etc.), 
continuous position sensors (Resolver, Magnetic Encoder, Absolute position encoder, etc.), 
and observers (PLL, Flux observer, sliding mode observer, slip observer), 
as well as the ramp generator. 
The encoder module abstracts the acquisition of angle, turns, velocity, interpolation, and other related aspects. 
In summary, this submodule provides a standardized input for different types of encoders.

+ SVPWM submodule

Space vector pulse width modulation (SVPWM) is a crucial technique for generating a three-phase converter. 
It is commonly used in motor drivers, grid-connected inverters, and voltage vector generators, among other applications. 
SVPWM serves as an important output strategy. 
Instead of manually calculating the values, users only need to provide the normalized values of $U_\alpha$ and $U_\beta$,
and this submodule will automatically calculate the values of Ta, Tb, Tc. 
These values are then sent to the high-performance timer, resulting in the voltage vector amplitude and phase. 
The `svpwm_header.h` toolkit provides a collection of tools for implementing SVPWM. 
The class within this toolkit offers an interface where users only need to provide the target vector $\vec U$. 
The class will handle the automatic calculation of the parameters for the high-performance timer. 
Overall, this submodule provides a standardized output for a three-phase voltage inverter.


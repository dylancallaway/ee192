# EECS192 Mechatronics Design Lab Course Code

This is the repository for the code I wrote for the 1/10th scale line following car.

The firmware, which runs on an STM32 NUCLEO microcontroller on the car is in the "fw" directory.

I created classes with relevant methods for each major component in the algorithm. i.e. the "Motor" class has a method called "Run" which turns the motor forward at a specified duty cycle. The source code and header files for these classes can be found in the "include" directory (in "fw").

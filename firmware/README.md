== Firmware ==

This firmware is to be flashed to the atmega88 of the pentabug.
The Makefile supports 2 different ways of flashing the firmware image:

=== the serial bootloader way ===

With the

	make serflash

target of the Makefile You can flash the microcontroler via the serial bootloader that is part of the initial firmware.


=== the isp way ===

With the

	make flash

target of the Makefile you can flash the microcontroler via an isp programmer.
Adjust the avrdude settings to Your programmers needs beforehand.



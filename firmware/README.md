# Firmware

## Flashing

## General

This firmware is to be flashed to the atmega88 of the pentabug.
The Makefile supports 2 different ways of flashing the firmware image:

### the serial bootloader way

With the

	make serflash

target of the Makefile You can flash the microcontroler via the serial
bootloader that is part of the initial firmware.

### the isp way 

With the

	make flash

target of the Makefile you can flash the microcontroler via an isp programmer.
Adjust the avrdude settings to Your programmers needs beforehand.

## App API

The new firmware aims to be as simple as possible to the application developer.
The applications are defined by a function which is called repeatedly in an
Arduino like main loop. Most hardware components are (read: should be in the
near future) accessible through an easy API.

There can be multiple different applications in one firmware. The next
application can be started by pressing one of the two buttons for at least 2
seconds. The application has to be cooperative as other applications can only
be started if the `test_app_stop()` function is called in your application
regularly. You most propably won't have to worry about this as most functions
in the hardware abstraction layer are already doing this for you.


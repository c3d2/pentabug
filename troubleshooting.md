# Troubleshooting

### That peculiar bug reacts sluggishly or slowly and doesn't "feel" right

Check that the fuses are correctly set to work at 8 MHz. Fuses should be `-U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0x01:m` 

### I checked the connections to the ISP pins but flashing firmware still fails

See [issue #23](https://github.com/c3d2/pentabug/issues/23) and try to omit the `-B 1` switch in the makefile.

### My bug does not respond to light

See [issue #21](https://github.com/c3d2/pentabug/issues/21) and help us fixing this in the firmware.

### My bug does not respond to sound very well

See [issue #22](https://github.com/c3d2/pentabug/issues/22) and help us fixing this in the firmware.


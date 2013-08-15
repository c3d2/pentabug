# Test firmware

Use this firmware to test all hardware components of a Pentabug. Add a
`pentatonic` extension and press all buttons. The buttons test the following
components (from left to right):

* flash LED 1
* send IR signal; receiving this signal flashes LED 2
* plays (lower) tone using BUZGND
* plays (higher) tone using BUZZR
* starts the vibration motor

This test should catch most obvious errors, but not all possible errors. Some
pinouts are not tested (e.g. JP3 and some pins of JP4 and JP5).


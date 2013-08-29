# Test firmware

Use this firmware to test all hardware components of a Pentabug. It can test
nearly all hardware features of the device.

The following things exist in all modes:

* right LED is on when IR signals are received (use a remote or first mode to
  test)
* pressing the right button activates next mode, plays a sound using BUZZR

Here is a description what happens on a right button press in which mode:

* activate left LED and send IR signal
* activates motor
* plays sound using BUZZGND
* activates all pins on the extension board for a short time (add a penetatonic
  to get feedback on all but one of those pins)


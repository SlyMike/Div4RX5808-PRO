# Div4RX5808-PRO
Arduino based 5.8ghz  Diversity Video receiver using 2x RX5808-PRO RX modules.

This project requires that you make 3 seperate PCB's. Two RX5808-PRO enabled video RX modules and a Diversity main board.
Boards ans schematics are supplied for both the receivers and mainboard.

The RX modules will need to be assembled and programmed with the RX5808_PRO software (available on Github).
The diversity code is available within this repository. After downloading, replace the .C file extendsion with .ino.

The code has been tested to a degree, it seems to work for me but there is always room for improvement.
I consider this project a work in progress, you may find bugs, spelling mistakes, missing component values. 
I may address this in future but for most folk it should not be hard to build.

FEATURES:

  RX5808-PRO SUPPORT.
  LIVE VIDEO / SPECTRUM ANALYSER SWITCHING VIA PUSH BUTTON.
  SINGLE RX OR DIVERSITY MODES VIA PUSH BUTTON.
  SERIAL DEBUG MODE VIA PUSH BUTTON.
  MODE LED INDICATORS.
  SOFTWARE SWITCH DE-BOUNCING.
  VARIABLE SOFTWARE RSSI SMOOTHING.
  RSSI VALUES VIA 4 LEDS.
  VARIABLE RX SWITCHING RATE.
  AUTO RSSI CALIBRATION.

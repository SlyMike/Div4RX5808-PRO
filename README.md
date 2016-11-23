# Div4RX5808-PRO
Arduino based 5.8ghz  Diversity Video receiver using 2x RX5808-PRO RX modules.

This project requires that you make 3 seperate PCB's. Two RX5808-PRO enabled video RX modules and a Diversity main board.
Boards and schematics are supplied for both the receivers and mainboard. 
Boards and schematics are in Eagle PCB format.

The RX modules will need to be assembled and programmed with the RX5808-PRO software (available on Github).
The diversity code for the Main board is available within this repository. After downloading, replace the .C file extension with .ino and upload.

The Main board and RX modules will require an Arduino Nano with an Atmega328.

The software portion of this project is not an adaptation of RX5808-PRO, it's function is to monitor 2x RSSI inputs from 2x RX5808-PRO enabled receivers and display the video feed from the receiver with the better signal. 
Video, audio and controls are switched simultaniously. LEDs indicate RSSI percentage (4 LEDs), active receiver (2 LEDs) and diversity mode (1 LED).

The code has been tested to a degree, it seems to work for me but there is always room for improvement.
I consider this project a work in progress, you may find bugs, spelling mistakes, missing component values, most of the source files were made in a rush or whilst I was doing something else.
I may address this in future but for most folk it should not be hard to build. I recommend Forking and Fixing, if you find a problem. ;-)

The point of this project was to create a decent micro controlled diversity receiver with better features than some of the crude homebrew solutions that I found on the net. I'm pretty sure I met my goal!

      FEATURES:
      
      *** AUTO RSSI CALIBRATION ***
      
      RX5808-PRO SUPPORT.
      
      LIVE VIDEO / SPECTRUM ANALYSER SWITCHING VIA PUSH BUTTON.
      SINGLE RX OR DIVERSITY MODES VIA PUSH BUTTON.
      SERIAL DEBUG MODE VIA PUSH BUTTON.
      TOTAL 6 PUSH BUTTONS FOR EASY OPERATION.
      SOFTWARE SWITCH DE-BOUNCING.
      
      MODE LED INDICATORS.
      RSSI LED INDICATORS.
      RX LED INDICATORS.
      
      VARIABLE RSSI SMOOTHING.
      VARIABLE RX SWITCHING RATE.
      VARIABLE RSSI HYSTERESIS.
      
      CHOICE OF VOLTAGE REFERENCE METHODS - CODE WILL WORK ON 3V3 AND 5V TARGETS.
      WELL COMMENTED CODE.
      

      

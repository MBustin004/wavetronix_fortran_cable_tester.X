# wavetronix_fortran_cable_tester.X
Created_Git

This is an MPLAB X project using the dsPIC33Fj64GP802. 
It is a 6 conductor cable tester that determines if the order of the connected conductors is correct.
This project is a visual confirmation testing device for electricians to verify that their wiring is correct. Even though the conductors in a standard Wavetronix homerun cable are colour coded, mistakes are frequently being made and not rectified until the day of turn-on because the electrician in the field has no way of testing his work.

There are two possible types of errors:
  - No connection (not connected or broken conductor)
  - Swapped conductors (blue in orange and orange in blue)

This device is capable of detecting both types of errors. A push button is used in initiate a test. 6 LEDs are used to indicate the state of each of the 6 conductors. These LEDs communicate to the electrician what error has been made, if any. A full solid LED indicates no issue. If an LED is blinking then that means there is a connection; however, the conductor not in the correct location. If an LED is off then there is no connection.


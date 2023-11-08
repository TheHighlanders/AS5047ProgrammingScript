# Programming Script for AS5047p Encoder Chips
This script is intended for an Arduino Nano, connected to an AS5047p chip, and programs it to enable PWM output on the W pin.

## Use
The values to program on the chip are set in the script, and could be modified to program many use cases. NOTE: The `factorySetting = 0` bit must be set in order for programming to succeed.

The encoder chip must be connected to the Arduino using SPI

The programming process implemented here is described in the datasheet: [AS5047p Datasheet](https://ams.com/documents/20143/36005/AS5047P_DS000324_3-00.pdf), this script is not polished, and is only intended for setting a few bits, so replaces the register comparisons with specific bit checks.

This script is made using the AS5X47 library from Adrian-Legrand, which is available in the Arduino Library Manager. It is based off of the ConfigureSensor example.
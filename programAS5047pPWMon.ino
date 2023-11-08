/*
  ConfirgureSensor

  Configure the sensor with advanced parameters.
  This example shows how to activate the UVW output for a 7 pole pairs motor.
  For an advanced list and description of all available parameters, 
  see the manufacturer documentation. All parameters are implemented in this library.

  This example also demostrates how to print debug information.

  Start the terminal with a speed of 9600 Bauds to see the debug information.

  The encoder is connected as follows:
  AS5X47         Arduino Board
    5V <-------------> 5V
   GND <-------------> GND
  MOSI <-------------> MOSI (Pin 11 for Arduino Uno)
  MISO <-------------> MISO (Pin 12 for Arduino Uno)
   SCK <-------------> SCK  (Pin 13 for Arduino Uno)
   CSN <-------------> SS   Arbitrary, Pin 10 in this example.
  See Arduino SPI Reference to see how to connect with other boards.
  https://www.arduino.cc/en/reference/SPI

  Created 18 april 2020
  By Adrien Legrand

  https://github.com/Adrien-Legrand/AS5X47

*/

// Include the library
#include <AS5X47.h>

// Define where the CSN Pin in connected. 
int slaveSelectPin = 10;

// Start connection to the sensor.
AS5X47 as5047p(slaveSelectPin);

void setup() {
  // Initialize a Serial Communication in order to
  // print the measured angle.
  Serial.begin(9600);

  // Create a structure corresponding to the SETTINGS1 Register
  // See manufacturer documentation for more details.
  Settings1 settings1;
  as5047p.printDebugString();
  delay(50);
  // Parameters from SETTINGS1 can be set like this:
  // According to manufacturer documentation, UVW_ABI enables UVW output.
  settings1.values.factorySetting = 0;
  
  //VALUES TO PROGRAM SHOULD BE SET HERE
  settings1.values.uvw_abi = 0;
  settings1.values.pwmon = 1; 

  // Write settings to the encoder:
  as5047p.writeSettings1(settings1);

  // This command print the debug information to the Serial port.
  // All registers of the encoder will be read and printed.
  // Note: A Serial communication must be opened with Serial.begin() before.
  as5047p.printDebugString();

  auto rdf = as5047p.readRegister(SETTINGS1_REG);
  settings1.raw = rdf.values.data;

  if(settings1.values.pwmon == 1){   
    Prog prog;

    as5047p.writeSettings1(settings1);  
    prog.values.progen = 1;
    as5047p.writeRegister(PROG_REG, prog.raw);
    Serial.println("Programmed PROGEN");
    
    prog.values.progen = 0;
    prog.values.progotp = 1;
    as5047p.writeRegister(PROG_REG, prog.raw);
    Serial.println("Programmed PROGOTP");

    ReadDataFrame progRDF = as5047p.readRegister(PROG_REG);
    prog.raw = progRDF.values.data;
    while(prog.raw != 0x0001){
      progRDF = as5047p.readRegister(PROG_REG);
      prog.raw = progRDF.values.data;

      Serial.println(prog.raw);
      delay(100);
    }
    Serial.println("PROG_REG equals 0x0001");

    for(int i = 0x16; i <= 0x19; i++){
      as5047p.writeRegister(i, 0x00);
    }
    Serial.println("Cleared Non Vol");
    prog.raw = 0x0;

    prog.values.progver = 1;
    as5047p.writeRegister(PROG_REG, prog.raw);
    Serial.println("Programmed PROGVER");
    prog.raw = 0x0;
    prog.values.otpref = 1;
    as5047p.writeRegister(PROG_REG, prog.raw);
    Serial.println("Programmed OPTREF");

    ReadDataFrame settingsRDF = as5047p.readRegister(SETTINGS1_REG);
    settings1.raw = settingsRDF.values.data;
  
    if(settings1.values.pwmon == 1){
      Serial.println("PWMON = 1");
    }

  }else{
    Serial.println("Did not Program");
  }


}

void loop() {}

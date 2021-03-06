//test main branch while changing stepper

// Reagenzglasläufer

// Christian Schormair christian.schormair@tum.de
// Hans ... Nachname...?

//libraries
  //ESP-FlexyStepper
  
//variables and constants

void setup() {
}

void loop() {
}

/////////////////////////
//Tasks to be implemented:
/////////////////////////

// "at the same time, all the time":
  //drive stepper motor https://github.com/pkerspe/ESP-FlexyStepper
  //read opt101
  //read hall sensor (maybe only when its likely that the magnet is near, between opt101)
  //read flip switch or button for ENA, to make the stepper motor loose

// other tasks:
  //accelerate stepper motor
  //processing the data
    //find maximums
    //manage the values for one rotation
  //send data to PC
  //correct position if neccessary
  //send ENA (to make the stepper motor loose), stop data processing meanwhile
  //correct position if neccessary
  //maybe turn on/off a LED or use a display that shows, if the maximum of the the range of the opt101 is reached 
                                    //(adjust sensitivity of the sensor through potentiometer if neccessary)

// Hardcoded "variables". Change here in the code and load up the new program to the ESP32. 
// Avoids the need for a display, buttons and additional programming for that. We will see if that is enough.
// Hardcoded "variables":
  //speed of the stepper motor

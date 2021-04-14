// Tissue-Culture-Rotator/Reagenzglasläufer

// Christian Schormair christian.schormair@tum.de
// Hans Ewigmann

//Tasks to be implemented are listed below the code

//libraries
#include "FastAccelStepper.h"

//definitions for FastAccelStepper.h
  //Pins
#define dirPinStepper    32
#define enablePinStepper 25
#define stepPinStepper   33
  //Set microstep mode at the stepper driver. 1, 2 , 4, 8, 16, 32
  //Set respective value here:
#define pulsesPerRevMotor  1600 // = 200 * microstepmode. See table at stepper driver TB6600.
  //define ratio for timing belt pulleys here:
#define gearRatio 50/25 //teeth on pulley on wheel divided through teeth pulley for motor
#define pulsesPerRevWheel = pulsesPerRevMotor * gearRatio
  //Set speed here:
#define revPerSecondWheel  2
  //The following are own ("default") constants, the other speeds and accelerations are part of FastAccelStepper.h
#define defaultSpeedInHz pulsesPerRevWheel * revPerSecondWheel // in (micro-)steps/s
  //Set acceleration here:
#define defaultAcceleration  pulsesPerRevMotor // in steps/s²

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;
  

//variables and constants
  //basics
const unsigned short tubecount = 30;
unsigned short tubes[tubecount]={0};
  //hall
bool hall = false;
  //for FastAccelStepper.h see above



//ESP-FlexyStepper library already has built in xTaskCreate, without vTaskDelay()

void setup() {
  Serial.begin(115200);
  
  //for FastAccelStepper.h
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  stepper->setDirectionPin(dirPinStepper);
  stepper->setEnablePin(enablePinStepper);
  stepper->setAutoEnable(false);
  
}

void loop() {
  //turn around
  
  for(;;){
    //read hall repeatedly
    if hall true{
      measureRev();
    }
  }
}

void measureRev(){
  
}

//replace by an interrupt
void readStopButton(void){
  for(;;){ // infinite loop
  }
}





/////////////////////////
//Tasks to be implemented:
/////////////////////////

//Information about multitasking on ESP32:
  //https://savjee.be/2020/01/multitasking-esp32-arduino-freertos/
  //https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/
  //https://www.instructables.com/ESP32-With-Arduino-IDE-Multi-Core-Programming/
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#_CPPv412TaskHandle_t

// "at the same time, all the time":
  //drive stepper motor via PWM FastAccelStepper.h  https://github.com/gin66/FastAccelStepper (https://github.com/pkerspe/ESP-FlexyStepper)
  //read opt101
  //read hall sensor (only when its likely that the magnet is near)
  //read stopSwitch via interrupt



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
                                    //(to adjust sensitivity of the sensor through potentiometer if neccessary)

// Hard coded "variables". Change here in the code and load up the new program to the ESP32. 
// Avoids the need for a display, buttons and additional programming for that. We will see if that is enough.
// Hardcoded "variables":
  //speed of the stepper motor

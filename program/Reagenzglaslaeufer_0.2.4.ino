// Tissue-Culture-Rotator/Reagenzglasläufer

// Christian Schormair christian.schormair@tum.de
// Hans Ewigmann

//Tasks to be implemented are listed below the code

//libraries
#include "FastAccelStepper.h"

//definitions for FastAccelStepper.h
  //Pins
const short dirPinStepper    =32;
const short enablePinStepper =25;
const short stepPinStepper   =33;
  //Set microstep mode at the stepper driver. 1, 2, 4, 8, 16, 32
  //Set respective value here:
const int stepsPerRevMotor = 1600; // = 200 * microstepmode. See table at stepper driver TB6600.
  //define ratio for timing belt pulleys here:
const float gearRatio = 50/25; //teeth on pulley on wheel divided through teeth pulley for motor
const float stepsPerRevWheel = stepsPerRevMotor * gearRatio;
  //Set speed here (in revolutions per second):
const float rpsWheel  = 2;
  //The following are own ("default") constants, the other speeds and accelerations are part of FastAccelStepper.h
const float defaultSpeedInHz = stepsPerRevWheel * rpsWheel; // in (micro-)steps/s
  //Set acceleration here:
const int defaultAcceleration = stepsPerRevMotor; // in steps/s²

  //Set speed for measure revolution here (in revolutions per MINUTE)
const float rpmMeasureWheel = 1;
const float measureSpeedInHz = stepsPerRevWheel * rpmMeasureWheel / 60;
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void waitSpeedWaitHall (int speedMode);


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
  waitSpeedWaitHall(2);
}

void waitSpeedWaitHall (int speedMode=1){ //Speed Modes: 0...stop, 1...keep speed, 2...measureSpeed, 3...defaultSpeed
  float speedbeforeMilliHz = stepper->getSpeedInMilliHz();
  float timeToAccelerate=0;//in ms, remains 0 if speedMode=1
  if(speedMode==3){
        stepper->setSpeedInHz(defaultSpeedInHz);
        //Time to Accelerate in ms. Theoretically. Add some ms here. Squareroot of...
        timeToAccelerate = sqrt(2 * abs(speedbeforeMilliHz-defaultSpeedInHz)/ defaultAcceleration)*1000+200;
  }
  if(speedMode==2){
        stepper->setSpeedInHz(measureSpeedInHz);
        timeToAccelerate = sqrt(2 * abs(speedbeforeMilliHz-measureSpeedInHz)/ defaultAcceleration)*1000+200;
  }
  stepper->runForward();
  delay(timeToAccelerate);
  
}


/*
void measureRev(){
  
}

//replace by an interrupt
//void readStopButton(void){


void processData(){
  Serial.flush()
}


*/

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

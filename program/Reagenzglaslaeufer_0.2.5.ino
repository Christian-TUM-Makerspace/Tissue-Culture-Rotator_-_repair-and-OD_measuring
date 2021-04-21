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
const float defaultRpsWheel  = 2;
  //The following are own ("default") constants, the other speeds and accelerations are part of FastAccelStepper.h
const float defaultSpeedInHz = stepsPerRevWheel * defaultRpsWheel; // in (micro-)steps/s
  //Set acceleration here:
const int defaultAcceleration = stepsPerRevMotor; // in steps/s²
  //Set speed for measure revolution here (in revolutions per MINUTE)
const float rpmMeasureWheel = 1;
const float measureSpeedInHz = stepsPerRevWheel * rpmMeasureWheel / 60;
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

//functions must be declared before they can be called
void waitSpeedWaitHall (int speedMode);


//variables and constants
  //basics
const unsigned short tubecount = 30;
  //array for measured values. first column [0] for raw data, second [1] for calculated OD
unsigned short tubes[tubecount][2]={0};
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
  stepper->setAutoEnable(true);
  stepper->setAcceleration(defaultAcceleration);
  stepper->setSpeedInHz(0);  
  Serial.print("Default revolutions per second, wheel: ");
  Serial.println(defaultRpsWheel);
  Serial.print("defaultSpeedInHz: ");
  Serial.println(defaultSpeedInHz);
  Serial.print("Measurement, revolutions per MINUTE, wheel: ");
  Serial.println(rpmMeasureWheel);  
  Serial.print("defaultAcceleration: ");
  Serial.println(defaultAcceleration);  

  
}

void loop() {
  //stepper->stopMove();
  /*stepper->setSpeedInHz(500);
  stepper->runForward();  
  delay(1000);
  stepper->setSpeedInHz(300);

  delay(1000);*/
  Serial.print("position before waitSpeedWaitHall(3) ");
  Serial.println(stepper->getCurrentPosition());
  Serial.flush();
  waitSpeedWaitHall(3);

  Serial.print("position after waitSpeedWaitHall(3) ");
  Serial.println(stepper->getCurrentPosition());
  Serial.flush();
  delay(10000);

  Serial.print("position before waitSpeedWaitHall(0) ");
  Serial.println(stepper->getCurrentPosition());
  Serial.flush();
  waitSpeedWaitHall(0);
  Serial.print("Speed in Hz ");  
  Serial.println(stepper->getSpeedInMilliHz()/1000);
  //stepper->runForward();
  Serial.print("position after waitSpeedWaitHall(0) ");
  Serial.println(stepper->getCurrentPosition());
  Serial.println(" ");
  Serial.flush();
  delay(10000);
}

void waitSpeedWaitHall (int speedMode=1){ //Speed Modes: 0...stop, 1...keep speed, 2...measureSpeed, 3...defaultSpeed, 4...for testing
  float speedbeforeHz = stepper->getCurrentSpeedInMilliHz()/1000;
  float timeToAccelerateInMillisec=0;//in ms
  if(speedMode==3){
    stepper->setSpeedInHz(defaultSpeedInHz);
    //Time to Accelerate in ms. Theoretically. Add some ms here.
    Serial.print(" sqrt(2 * abs(speedbeforeHz-defaultSpeedInHz)) = ");
    Serial.println(sqrt(2 * abs(speedbeforeHz-defaultSpeedInHz)));    
    timeToAccelerateInMillisec = sqrt(2 * abs(speedbeforeHz-defaultSpeedInHz) / defaultAcceleration)*1000 +200;
    stepper->runForward();
  }
  else if(speedMode==2){  
    stepper->setSpeedInHz(measureSpeedInHz);
    timeToAccelerateInMillisec = sqrt(2 * abs(speedbeforeHz-measureSpeedInHz) / defaultAcceleration)*1000  +200;
    stepper->runForward();
  }
  else if(speedMode==0){
    stepper->setSpeedInHz(0);
    stepper->stopMove();
    timeToAccelerateInMillisec = sqrt(2 * abs(speedbeforeHz-0) / defaultAcceleration)*1000  +200;
    //stepper->runForward();
  }
  else if(speedMode==4){
    Serial.println("Speedmode 4");
    stepper->setAcceleration(100);
    stepper->setSpeedInHz(500);
    stepper->move(100);
    delay(5000);
  }
  else{
    Serial.println("Error: Wrong number for speedMode");
    Serial.println("Speed Modes: 0...stop, 1...keep speed, 2...measureSpeed, 3...defaultSpeed, 4...for testing");
  }

  delay(timeToAccelerateInMillisec);
  Serial.print("timeToAccelerateInMillisec ");
  Serial.println(timeToAccelerateInMillisec);
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

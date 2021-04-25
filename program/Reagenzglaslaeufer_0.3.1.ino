// Tissue-Culture-Rotator/Reagenzglasläufer

// Christian Schormair christian.schormair@tum.de
// Hans Ewigmann

//Tasks to be implemented are listed below the code

//functions must be declared before they can be called
void measurementRev();
void waitSpeedWaitHall (int speedMode);

//variables, constants, pins
  //basics
const unsigned short tubecount = 40;
  //array for measured values. first column [0] for raw data, second [1] for calculated OD
int tubeValues[tubecount][2]={0};


  //for FastAccelStepper.h see above

  //Pins for sensors: opt101 and hall effekt sensor KY-024
const int optPin= 34;
const int hallPin= 35; //supply with 3.3 V from the board!

//libraries
#include "FastAccelStepper.h"

//definitions for FastAccelStepper.h
  //Pins
const short dirPinStepper    =32;
const short enablePinStepper =25;
const short stepPinStepper   =33;
  //more for FastAccelStepper.h
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

//For speed and position control
  //Set microstep mode at the stepper driver. 1, 2, 4, 8, 16, 32
  //Set respective value here:
const int stepsPerRevMotor = 6400; // = 200 * microstepmode. See table at stepper driver TB6600.
  //define ratio for timing belt pulleys here:
const float gearRatio = 50/25; //teeth on pulley on wheel divided through teeth pulley for motor
const float stepsPerRevWheel = stepsPerRevMotor * gearRatio;
  //Set speed here (in revolutions per second):
const float defaultRpsWheel  = 1;
  //The following are own ("default") constants, the other speeds and accelerations are part of FastAccelStepper.h
const float defaultSpeedInHz = stepsPerRevWheel * defaultRpsWheel; // in (micro-)steps/s
  //Set acceleration here:
const int defaultAcceleration = stepsPerRevMotor; // in steps/s²
  //Set speed for measure revolution here (in revolutions per MINUTE)
const float rpmMeasureWheel = 0.5;
const float measurementSpeedInHz = stepsPerRevWheel * rpmMeasureWheel / 60;
  //positions on the wheel
    //steps per tube segment
const float stepsTubeSegm = stepsPerRevWheel/tubecount;
const int roundedStepsTubeSegm = round(stepsTubeSegm);
/*  //Save the values of the optical measurement around one tube temporarily (find maximum afterwards)
  //first the right size for the array must be estimated. Lets try one measurement per microsecond ms
  //Revolutions per minute / (microseconds per minute * tubecount *2)
int sizeArray = round(rpmMeasureWheel*stepsPerRevMotor/(60000*tubecount*2));
int optValTemp [sizeArray] = {0};*/


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
  Serial.print("measurementSpeedInHz: ");
  Serial.println(measurementSpeedInHz);  
  Serial.print("roundedStepsTubeSegm: ");
  Serial.println(roundedStepsTubeSegm);  
  Serial.print(": ");
  Serial.println();  

  pinMode(optPin, INPUT);
  pinMode(hallPin, INPUT);
}

void measurementRev(){
  //Bring wheel to measure speed
  if (stepper->getCurrentSpeedInMilliHz()/1000 != measurementSpeedInHz){
    waitSpeedWaitHall(2);
  }

  int optValTemp = 0;
  //wait until tube is near opt101
  while(stepper->getCurrentPosition() < 0.25 * stepsTubeSegm){
    //delay(1);
  }
  //measure one tube after another
  for (short i=0; i < tubecount; i++){
    int endPos = (i+0.5) * stepsTubeSegm;
    //measure repeatedly while opt101 is near tube, keep the maximum value
    int optValTemp = 0;
    while (stepper->getCurrentPosition()< endPos){
      optValTemp = analogRead(optPin);
      if (optValTemp > tubeValues[i][0]){
        tubeValues[i][0] = optValTemp;    
      }
    }
    Serial.println(tubeValues[i][0]);
  }
  //
  //Check for half segment position if hallPin is HIGH
  //continue if otherwise try another round
  //if then it does not work... do what?
  //maybe "return" something that is not 0
  //But what?

  //
  // function: send data
  //
}

void waitSpeedWaitHall (int speedMode = 1){ //Speed Modes: 0...stop, 1...keep speed, 2...measureSpeed, 3...defaultSpeed, 4...for testing
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
    stepper->setSpeedInHz(measurementSpeedInHz);
    timeToAccelerateInMillisec = sqrt(2 * abs(speedbeforeHz-measurementSpeedInHz) / defaultAcceleration)*1000  +200;
    stepper->runForward();
  }
  else if(speedMode==0){
    stepper->setSpeedInHz(0);
    stepper->stopMove();
    timeToAccelerateInMillisec = sqrt(2 * abs(speedbeforeHz-0) / defaultAcceleration)*1000  +200;
    //stepper->runForward();
  }
  else if(speedMode==4){
    Serial.println("Speedmode 4: test");
    stepper->setAcceleration(100);
    stepper->setSpeedInHz(500);
    stepper->move(100);
    delay(5000);
  }
  else{
    Serial.println("Error: Wrong number for speedMode");
    Serial.println("Speed Modes: 0...stop, 1...keep speed, 2...measurementSpeed, 3...defaultSpeed, 4...for testing");
  }

  delay(timeToAccelerateInMillisec);
  //Serial.print("timeToAccelerateInMillisec ");
  //Serial.println(timeToAccelerateInMillisec);

  while (digitalRead(hallPin) == HIGH){
    delay(10);
  }
  Serial.println("hallPin near");

  //a workaround: With the ESP32, FastAccelStepper.h can not set the current position when moving.
    //see: https://github.com/gin66/FastAccelStepper/blob/258ffb42d34a971ff1f193b82b1a1abefbc27229/src/FastAccelStepper.h#L103
  stepper->keepRunning();
  stepper->stopMove();
  delay(1000);
  stepper->setCurrentPosition(0);  
  int pos = stepper->getCurrentPosition();  
  stepper->runForward();
  Serial.print("CurrentPosition:  ");
  Serial.println(pos);  
}

void loop(){
  measurementRev();
}

/*
//replace by an interrupt
//void readStopButton(void){
void processData(){
  Serial.flush()
}
*/

/////////////////////////
//Tasks to be implemented:
/////////////////////////

//Multitasking was planned, but then an almost pure serial approach was used
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

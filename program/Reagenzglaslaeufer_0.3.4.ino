// Tissue-Culture-Rotator/Reagenzglasläufer

// Christian Schormair christian.schormair@tum.de
// Hans Ewigmann

//Tasks to be implemented are listed below the code

//functions must be declared before they can be called
void measurementRev();
void waitSpeedWaitHall (int speedMode, bool hallPosition);
void stopMode ();
void checkStopSwitch(int delayInMillis);

//how many seconds should the wheel turn between measurement turns? (through delay(), maybe takes some ms more)
//set here
int timeNormalTurnSec = 60;

//variables, constants, pins
  //basics
const unsigned short tubecount = 40;
  //array for measured values. first column [0] for raw data, second [1] for testing purposes (position for example)
int tubeValues[tubecount][2]={0};

  //Pins for sensors: opt101 and hall effekt sensor KY-024, flip switch
const int optPin= 34;
const int hallPin= 35;
const int switchPin = 27;

//Calibration in stopMode(): The measured values of the opt101 should not reach the maximum (4095 for ESP32, see below)
//When the clear solution at the beginning is in the right position and the LED shines through, the value should be near 4095.
//Maybe print the lines above and tape them to the device.
//High resistance -> high amplification -> high value
  //Pins for the calibration LED (3 colors)
const int ledBlue = 17; //to low or not in the right position
const int ledGreen = 18; //Potentiometer is set correctly, if led never turns red when moving the wheel slowly around the right position.
const int ledRed = 19; //Potentiometer is set to high.
  //Lower limit for the calibraion
const int lowerLimitCalibration = 3900;
  //lower Limit for calibration. below the calibration LED will turn blue.
  //ESP32 analogRead ranges between 0 and 4095 (12 bit). Change if you use a board with other values.
const int analogMax = 4095;

  //if the magnet is not found, set true. Try once again, then stop measurement.
bool magnetNotFound = false;
bool stopMeasurement = false;

//libraries
#include "FastAccelStepper.h"

//definitions for FastAccelStepper.h
  //Pins
const short dirPinStepper    =32; //not used, but maybe someone else wants to change the direction
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
const float defaultRpsWheel  = 0.5; //not tested above 2. 
  //The following are own ("default") constants, the other speeds and accelerations are part of FastAccelStepper.h
  // Default speed in (micro-)steps/s   FastAccelStepper.h "allows up 200000 generated steps per second" on ESP32. Enough.
const float defaultSpeedInHz = stepsPerRevWheel * defaultRpsWheel; 
  //Set acceleration here:
const int defaultAcceleration = stepsPerRevMotor; // in steps/s²
  //Set speed for measure revolution here (in revolutions per MINUTE)
const float rpmMeasureWheel = 2; //0.5 seems to be around some kind of eigen frequency on our device
const float measurementSpeedInHz = stepsPerRevWheel * rpmMeasureWheel / 60;
  //positions on the wheel
    //steps per tube segment
const float stepsTubeSegm = stepsPerRevWheel/tubecount;
const int roundedStepsTubeSegm = round(stepsTubeSegm);//stepper can only make full (micro)steps, no comma steps


//settings for hall sensor. change here, if the measurements are not made in the right positions.
  //offset for hall sensor. "how much to early the hall sensor reacts?" 
  //In percentage of a segment for one test tube.
const short offsetHallPercent = 15;
const int offsetHallSteps = stepsTubeSegm * offsetHallPercent / 100;
  //measured analog value of the hall sensor BELOW which the magnet is considered near
    //a hall sensor has a lower voltage when its near a magnet (in the right direction)
    //Use following test code if neccessary and choose a value some 100 below the maximum
    //https://github.com/Christian-TUM-Makerspace/Tissue-Culture-Rotator_-_repair-and-OD_measuring/blob/86767f8fad0253bf3ae0899edf4722ef4e440fe7/testcode/Hall_Sensor_KY-024.ino
const int valueMagnNear = 1300;

void setup() {
  Serial.begin(115200);

  //for flip switch for stopMode()

  pinMode(switchPin, INPUT_PULLDOWN);

  //for FastAccelStepper.h
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  stepper->setDirectionPin(dirPinStepper);
  stepper->setAutoEnable(false);
  stepper->setEnablePin(enablePinStepper,false);
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
  pinMode(ledBlue, OUTPUT);
  digitalWrite(ledBlue, LOW);//somehow it shines a little  
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
}

//No continious check of the flip switch. Insert function between all steps that take more than a second or so.
void checkStopSwitch(int delayInMillis = 0){
  if (digitalRead(switchPin)== HIGH){
    stopMode ();    
  } 
  delay(delayInMillis);
}

//Set the motor free to insert the test tubes and calibrate
//Documentation for the calibration in the header
//Can not be called if Speed is set 0 otherwise
void stopMode (){
  waitSpeedWaitHall(0,false);
  stepper->setEnablePin(enablePinStepper,true);
  while (digitalRead(switchPin) == HIGH) {
    Serial.println("In loop for LED and switch pin");    
    int stopModeOpt = analogRead(optPin);
    if (stopModeOpt == analogMax){
      digitalWrite(ledBlue, LOW);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledRed, HIGH);
      delay(200); // to make it visible
    }
    else if (lowerLimitCalibration < stopModeOpt  && stopModeOpt < analogMax){
      digitalWrite(ledBlue, LOW);
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledRed, LOW);
    }
    else if (lowerLimitCalibration > stopModeOpt){
      digitalWrite(ledBlue, HIGH);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledRed, LOW);
    }
  }
  stepper->setEnablePin(enablePinStepper,false);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  Serial.println("End of stopMode()");  
}

void measurementRev(){
  //for testing:
  //int countValues = 0;
  
  //Bring wheel to measure speed
  if (stepper->getCurrentSpeedInMilliHz()/1000 - measurementSpeedInHz > 10){
    waitSpeedWaitHall(2,true);
  }
  //zero the array
  memset(tubeValues, 0, sizeof tubeValues);

  int optValTemp = 0;
  
  //measure one tube after another
  for (short i=0; i < tubecount; i++){
    checkStopSwitch();
    int startPos = (i+0.25) * stepsTubeSegm;
    int endPos = (i+0.75) * stepsTubeSegm;
    //measure repeatedly while opt101 is near tube, keep the maximum value
    int optValTemp = 0;
    //wait until tube is near opt101
    while(stepper->getCurrentPosition() < startPos){
    delay(1);

    }
    while (stepper->getCurrentPosition()< endPos){
      //countValues = countValues + 1;
      optValTemp = analogRead(optPin);
      if (optValTemp > tubeValues[i][0]){
        tubeValues[i][0] = optValTemp;
        //for testing, if the postions are right:
        //tubeValues[i][1] = stepper->getCurrentPosition();
      }
    }
    Serial.println(tubeValues[i][0]);

  }
  //check if the revolution ran normal
  int posTemp = stepper->getCurrentPosition();
  while (posTemp + 0.25*stepsTubeSegm > stepper->getCurrentPosition()){
    if (analogRead(hallPin) > valueMagnNear){
      Serial.println("measurement revolution successfull ");
      magnetNotFound = false;
      return;
    }
  }
  //stop the masurement if the magnet is not found two times in sequence
  if (magnetNotFound == false){
    magnetNotFound = true;
  }
  else {
    stopMeasurement = true;
    Serial.println("Magnet was not found two times in sequence. Stopping measurement.");
  }  

  //
  // function: send data
  //
}

//Set new speed, wait until it is reached, if hallPosition = true: wait for magnet and set position to 0 there (or near zero, with offset)
void waitSpeedWaitHall (int speedMode = 1, bool hallPosition = true){ //Speed Modes: 0...stop, 1...keep speed, 2...measurementSpeed, 3...defaultSpeed, 4...for testing
  stepper->setEnablePin(enablePinStepper,false);
  
  if(speedMode==3){
    stepper->setSpeedInHz(defaultSpeedInHz);
    stepper->runForward();
    Serial.println("Speed mode 3: defaultSpeed");
  }
  else if(speedMode==2){  
    stepper->setSpeedInHz(measurementSpeedInHz);
    stepper->runForward();
    Serial.println("Speed mode 2: measurementSpeed");
  }
  else if(speedMode==1){
    stepper->setSpeedInHz(stepper->getSpeedInMilliHz()*1000);
    stepper->runForward();
  }
  else if(speedMode==0){
    stepper->keepRunning();
    stepper->stopMove();
    stepper->setSpeedInHz(0);
    Serial.println("Speed mode 0: stop");
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
  while (abs(stepper->getSpeedInMilliHz() - stepper->getCurrentSpeedInMilliHz())> 10){//somehow it never gets the same for higher speeds
    Serial.print("CurrentSpeedInMilliHz = ");    
    Serial.println(stepper->getCurrentSpeedInMilliHz());
    Serial.print("Set SpeedInMilliHz = ");    
    Serial.println(stepper->getSpeedInMilliHz());
    delay(100);
  }
  //Set the position to zero or a corrected value (= minus offset) around zero
    //Ignores the time for acceleration. Should be ok for measuring. Keep in mind for other uses.
  if (hallPosition == true){
    while (analogRead(hallPin) > valueMagnNear){
    }
    Serial.println("hallPin near");
    //a workaround: With the ESP32, FastAccelStepper.h can not set the current position when moving.
      //see: https://github.com/gin66/FastAccelStepper/blob/258ffb42d34a971ff1f193b82b1a1abefbc27229/src/FastAccelStepper.h#L103
    stepper->keepRunning();
    stepper->stopMove();
    delay(100);
    stepper->setCurrentPosition(-offsetHallSteps);  
    int pos = stepper->getCurrentPosition();  
    stepper->runForward();
  }
}

void loop(){
/*  int buttonPressed = digitalRead(switchPin);
  if (digitalRead(switchPin) == HIGH){
    Serial.println("switch is on");
  } 
  else {
    Serial.println("switch is off");
  }
  delay(100);
*/
  waitSpeedWaitHall(3,false);
  timeNormalTurnSec = 1;//>=1
  for (int i = 0; i < timeNormalTurnSec;i++){
    checkStopSwitch(1000);
  }
  if (stopMeasurement == false){
    measurementRev();
  }
}



/////////////////////////
//Tasks to be implemented:
/////////////////////////

//Multitasking was planned, but then an almost pure serial approach was used. Even for the stopSwitch.
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
  //maybe turn on/off a LED or use a display that shows, if the maximum of the the range of the opt101 is reached 
                                    //(to adjust sensitivity of the sensor through potentiometer if neccessary)

// Hard coded "variables". Change here in the code and load up the new program to the ESP32. 
// Avoids the need for a display, buttons and additional programming for that. We will see if that is enough.
// Hardcoded "variables":
  //speed of the stepper motor

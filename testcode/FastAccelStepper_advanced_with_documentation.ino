//https://github.com/gin66/FastAccelStepper

//modified example with version 0.23.0

//documentation for usage:
//https://github.com/gin66/FastAccelStepper/blob/master/src/FastAccelStepper.h
//permalink to the version used here:
//https://github.com/gin66/FastAccelStepper/blob/2e0bba84d3dead4e6b8de964defb1c3f78daa21f/src/FastAccelStepper.h
//There is much text and it is hard to find the basics there.
//Some annotations from there were copied to this sketch with own annotations added.

//This library was chosen because it uses PWM on the ESP32, which should be much much less CPU-intensive.

/*  

  // about enable and disable
  // FastAccelStepper.h   line 51 ff
  https://github.com/gin66/FastAccelStepper/blob/2e0bba84d3dead4e6b8de964defb1c3f78daa21f/src/FastAccelStepper.h#L51

  
  // maybe it is better to turn auto_enable off
  // maybe that avoids problems with many operations "at the same time"
  // FastAccelStepper.h   line 69 - 90
  bool auto_enable = false;
  stepper->setAutoEnable(false);

  
  // Retrieve the current position of the stepper
  int32_t getCurrentPosition();

  // Set the current position of the stepper - either in standstill or while moving
      //    for esp32: the implementation uses getCurrentPosition(), which does not
      //               consider the steps of the current command
      //               => recommend to use only in standstill
  // FastAccelStepper.h   line 101 ff
      void setCurrentPosition(int32_t new_pos);
        

  // For stepper movement control by FastAccelStepper's ramp generator
  //
  // setSpeedInUs expects as parameter the minimum time between two steps.
  // If for example 5 steps/s shall be the maximum speed of the stepper,
  // then t = 0.2 s/steps = 200000 us/step, so call
  //      setSpeedInUs(200000);
  // FastAccelStepper.h   line 114 ff
  // more details in FastAccelStepper.h

  // setSpeedInHz() allows to set the stepper speed as step frequency in Hertz.
  // This means steps/s.
  //Line 147 ff

  
  // set Acceleration expects as parameter the change of speed
  // as step/s².
  // If for example the speed should ramp up from 0 to 10000 steps/s within
  // 10s, then the acceleration is 10000 steps/s / 10s = 1000 steps/s²
     setAcceleration()
  // Line 159 ff
  // more details in FastAccelStepper.h

  
  // This function applies new values for speed/acceleration.
  // This is convenient especially, if the stepper is set to continuous running.
  void applySpeedAcceleration();
  // Line 181 ff

  
  // start/move the stepper for (move) steps or to an absolute position.
  //
  // If the stepper is already running, then the current running move will be
  // updated together with any updated values of acceleration/speed. The move is
  // relative to the target position of any ongoing move ! If the new
  // move/moveTo for an ongoing command would reverse the direction, then the
  // command is silently ignored.
  int8_t move(int32_t move);
  int8_t moveTo(int32_t position);
  // more about feedback in FastAccelStepper.h
  // Line: https://github.com/gin66/FastAccelStepper/blob/2e0bba84d3dead4e6b8de964defb1c3f78daa21f/src/FastAccelStepper.h#L185


  // There are 2 ways to run the motor continiously:
  https://github.com/gin66/FastAccelStepper/blob/2e0bba84d3dead4e6b8de964defb1c3f78daa21f/src/FastAccelStepper.h#L200-L212
  
  // This command flags the stepper to keep run continuously into current
  // direction. It can be stopped by stopMove.
  // Be aware, if the motor is currently decelerating towards reversed
  // direction, then keepRunning() will speed up again and not finish direction
  // reversal first.
  void keepRunning();
  bool isRunningContinuously() { return _rg.isRunningContinuously(); }

  // This command just let the motor run continuously in one direction.
  // If the motor is running in the opposite direction, it will reverse
  // return value as with move/moveTo
  int8_t runForward() { return _rg.startRun(true); }
  int8_t runBackward() { return _rg.startRun(false); }


  // There are 2 ways to stop movement
  https://github.com/gin66/FastAccelStepper/blob/2e0bba84d3dead4e6b8de964defb1c3f78daa21f/src/FastAccelStepper.h#L234-L241

  // stop the running stepper as fast as possible with deceleration
  // This only sets a flag and can be called from an interrupt !
  void stopMove();
  bool isStopping() { return _rg.isStopping(); }

  // stop the running stepper immediately and set new_pos as new position
  // This can be called from an interrupt !
  void forceStopAndNewPosition(uint32_t new_pos);
*/

//FastAccelStepper
#include "FastAccelStepper.h"

#define dirPinStepper    32
#define enablePinStepper 25
#define stepPinStepper   33


//Set microstep mode at the stepper driver. 1, 2 , 4, 8, 16, 32
//Set respective value here:
#define pulsesPerRevMotor  1600 // = 200 * microstepmode. See table at stepper driver TB6600.
//define timing belt pulleys here:
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

void setup() {
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);

  stepper->setDirectionPin(dirPinStepper);
  stepper->setEnablePin(enablePinStepper);
  stepper->setAutoEnable(false);

}

void loop() {
  stepper->enableOutputs();


      stepper->setSpeedInHz(defaultSpeedInHz);       // 500 steps/s
      stepper->setAcceleration(defaultAcceleration);    // 100 steps/s²
      stepper->move(100); //lower value -> longer movement, why?   
      /*stepper->setSpeedInHz(1600);  
      stepper->setAcceleration(500);    
      stepper->move(5); //lower value -> longer movement, why?
      delay(2000);*/
}

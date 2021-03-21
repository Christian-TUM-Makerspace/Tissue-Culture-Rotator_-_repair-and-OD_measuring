//https://github.com/gin66/FastAccelStepper

//modified example

#include "FastAccelStepper.h"


#define dirPinStepper    34
#define enablePinStepper 35
#define stepPinStepper   33

// maybe it is better to turn auto_enable off
// FastAccelStepper.h   line 69 - 90
bool auto_enable = false;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup() {
   engine.init();
   stepper = engine.stepperConnectToPin(stepPinStepper);

}

void loop() {
     if (stepper) {
      stepper->setDirectionPin(dirPinStepper);
      stepper->setEnablePin(enablePinStepper);
      stepper->setAutoEnable(true);

        
      stepper->setSpeedInHz(500);       // 500 steps/s
      stepper->setAcceleration(100);    // 100 steps/s²
      stepper->move(1000); //lower value -> longer movement, why? the value is a position, but what kind of position?

      /*stepper->setSpeedInHz(1600);  
      stepper->setAcceleration(500);    
      stepper->move(5); //lower value -> longer movement, why?
      delay(2000);*/
      
     }
}

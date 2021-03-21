//https://github.com/gin66/FastAccelStepper

//modified example

#include "FastAccelStepper.h"


#define dirPinStepper    34
#define enablePinStepper 35
#define stepPinStepper   33


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
      stepper->move(1000); //lower value -> longer movement, why?


     }
}

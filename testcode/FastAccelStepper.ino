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

      //stepper->setSpeedInHz(6400);       // 500 steps/s
      stepper->setAcceleration(500);    // 100 steps/s²
      //stepper->move(100); //lower value -> longer movement, why?
      stepper->setSpeedInHz(1600);  
      stepper->move(5);
      //stepper->setSpeedInHz(0);
      //delay(2000);
      
     }
}

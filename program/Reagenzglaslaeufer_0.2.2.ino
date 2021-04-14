// Reagenzglasläufer

//Tasks to be implemented are listed below the code

// Christian Schormair christian.schormair@tum.de
// Hans Ewigmann

//libraries
#include <ESP_FlexyStepper.h>
  
//variables and constants
const int tubecount = 30;
int tubes[tubecount]={0};

//ESP-FlexyStepper library already has built in xTaskCreate, without vTaskDelay()

void setup() {
  Serial.begin(115200);
}


void readHall(void){
  }
  
}

//replace by an interrupt
void readStopButton(void){
  for(;;){ // infinite loop
  }
}

void readOpt(void){
  
  for(;;){ // infinite loop

    Serial.println("readOpt");

    // Pause the task for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS); //portTICK_PERIOD_MS for milliseconds instead of "ticks
  }
  
}

void loop() {
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

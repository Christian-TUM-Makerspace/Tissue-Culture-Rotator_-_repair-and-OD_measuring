// Reagenzglasläufer

//Tasks to be implemented are listed below the code

// Christian Schormair christian.schormair@tum.de
// Hans ... Nachname...?

//libraries
#include <ESP_FlexyStepper.h>
  
//variables and constants
const int tubecount = 30;
int tubes[tubecount]={0};

//Task handles for Multitasking through FreeRTOS (see "Information about multitasking on ESP32" below)
TaskHandle_t readOptHandle=NULL;
TaskHandle_t readHallHandle=NULL;
TaskHandle_t readStopButtonHandle=NULL;
//ESP-FlexyStepper library already has built in xTaskCreate, without vTaskDelay()

void setup() {
  Serial.begin(115200);
  
  
  //create readOpt task through FreeRTOS, executed in the readOpt() function, priority 3, core 0
  xTaskCreatePinnedToCore(
                    readOpt,     // Function that implements the task.
                    "readOpt",   // name of task.
  /*reduce when I understand it better */10000,       // Stack size in bytes, not words.
                    NULL,        // Parameter passed into the task.   or: ( void * ) 1,    
                    3,           // priority of the task
                    &readOptHandle,// Task handle to keep track of created task
                    0);          // pin task to core 0
  //The same for hall effect sensor and stop button, each: priority lower priority than readOpt, core 1
  xTaskCreatePinnedToCore(readHall,"readHall", 10000, NULL, 1, &readHallHandle, 1);
  xTaskCreatePinnedToCore(readStopButton,"readStopButton", 10000, NULL, 2, &readStopButtonHandle, 1);
  
}

void readHall(void * pvParameters){
//  delay(1020);
  for(;;){ // infinite loop
    Serial.println("readHall");

    // Pause the task for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  
}
void readStopButton(void * pvParameters){
//  delay(1030);
  for(;;){ // infinite loop
    Serial.println("readStopButton");
    
    // Pause the task for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void readOpt(void * pvParameters){
//  delay(1010);
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
  //drive stepper motor ESP_FlexyStepper.h  https://github.com/gin66/FastAccelStepper (https://github.com/pkerspe/ESP-FlexyStepper)
  //read opt101
  //read hall sensor (maybe only when its likely that the magnet is near, between opt101)



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

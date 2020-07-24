#include "Arduino.h"
#include <GFXDisplay.h>

GFXDisplay *gfx;

TaskHandle_t Task1;

void setup() {

  Serial.begin(115200);
  
  gfx = new GFXDisplay(16);
  
  Serial.println("Spectrum Analyzer");


  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    1000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 


  


}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
      gfx->drawScreen();
    vTaskDelay(1);
  } 
}



void loop() {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());


  for(;;) {
    
      gfx->scrollWord("Hello World", GREEN, 50);
  }
  
}

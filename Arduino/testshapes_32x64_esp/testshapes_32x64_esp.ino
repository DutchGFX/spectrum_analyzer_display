#include <spectrumDisplay.h>

#define BARS 16

TaskHandle_t Task1;

spectrumDisplay *s;
uint8_t mags[BARS];


void setup() {

  Serial.begin(115200);
  Serial.println("Spectrum Analyzer");

  s = new spectrumDisplay(BARS);
  s->checkDisplay();

  for (int i = 0; i < BARS; i++)
  {
    mags[i] = 0;
  }

  mags[0] = 255;
  mags[1] = 127;
  mags[2] = 63;

  s->updateDisplay(mags);
  s->setDrop(6);

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
    s->drawScreen();
    vTaskDelay(1);
  } 
}



void loop() {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());


/*
  for(;;) {
    for (int i = 0; i < BARS; i++)
    {
      for (int j = 0; j < 252; j += 4)
      {
        mags[i] += 4;
        s->updateDisplay(mags);
        delay(10);
      }
      mags[i] = 0;
      delay(10);
    }
  }
  */
}

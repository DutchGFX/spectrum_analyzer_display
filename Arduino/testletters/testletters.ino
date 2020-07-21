#include "Arduino.h"
#include <GFXDisplay.h>

GFXDisplay *gfx;

void setup() {

  Serial.begin(115200);
  
  gfx = new GFXDisplay(16);

  
  gfx->printWord("tyler ", BLUE);
  gfx->printWord("is ", RED);
  gfx->printWord("gay ", WHITE);
  gfx->printLetter('\n', WHITE);
  
  gfx->printWord("and peter is a    god", PURPLE);

}

void loop() {

      gfx->drawScreen();

}

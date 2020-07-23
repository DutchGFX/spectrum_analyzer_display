#include "Arduino.h"
#include <GFXDisplay.h>

GFXDisplay *gfx;

void setup() {

  Serial.begin(115200);
  
  gfx = new GFXDisplay(16);

  gfx->printWord("ABCDEFGHIJKL", RED);
  gfx->printLetter('\n', WHITE);
  gfx->printWord("MNOPQRSTUVW", GREEN);
  gfx->printLetter('\n', WHITE);
  gfx->printWord("XYZ", BLUE);

}

void loop() {

      gfx->drawScreen();

}

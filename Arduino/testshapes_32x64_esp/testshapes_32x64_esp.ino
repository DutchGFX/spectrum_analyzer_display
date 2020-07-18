#include "spectrumDisplay.h"

#define BARS 16

spectrumDisplay *s;
uint8_t mags[BARS];


uint8_t row = 0;
uint8_t col = 0;

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

}

int t = millis();

void loop() {

  s->drawScreen();

  if (millis() > (t + 10))
  {
    row += 4;
    if (row < 252)
    {
      mags[col] = row;
      s->updateDisplay(mags);
      
    }
    else
    {
      row = 0;
      mags[col] = row;
      s->updateDisplay(mags);
      col++;
      if (col == BARS)
      {
        col = 0;
      }
    }
    t = millis();
    
  }
  
}

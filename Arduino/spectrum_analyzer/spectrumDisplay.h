#include "Arduino.h"
#include "GFXDisplay.cpp"

#ifndef SPECTRUM_DISPLAY_H
#define SPECTRUM_DISPLAY_H

class spectrumDisplay
{
  public:
    spectrumDisplay(int bars);
    void updateDisplay(uint8_t *mags);
    void checkDisplay();
    void setDrop(uint8_t s);
    void setColorMode(uint8_t s);
    void cycleColorMode();
    void drawScreen();
    
  private:
    uint8_t _bars;
    uint8_t _bar_width;
    uint8_t _mode;
    uint16_t *_tops;
    uint16_t *_mags;
    uint8_t _drop;
    void dropBars(uint8_t index, uint8_t mag, uint16_t top);
    void updateBar(uint8_t index, uint8_t mag, uint16_t top);
    GFXDisplay *_gfx;
};

#endif

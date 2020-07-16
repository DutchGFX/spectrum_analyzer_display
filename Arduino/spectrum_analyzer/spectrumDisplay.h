#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

#include "Arduino.h"

#ifndef SPECTRUM_DISPLAY_H
#define SPECTRUM_DISPLAY_H

class spectrumDisplay {
   public:
    spectrumDisplay(int bars);
    void updateDisplay(uint8_t *mags);
    void checkDisplay();
    void setDrop(uint8_t s);

   private:
    uint8_t _bars;
    uint8_t _bar_width;
    uint8_t _mode;
    uint16_t *_tops;
    uint16_t *_mags;
    uint8_t _drop;
    void dropBars(uint8_t index, uint8_t mag, uint16_t top);
    void updateBar(uint8_t index, uint8_t mag, uint16_t top);
    RGB64x32MatrixPanel_I2S_DMA _dma_display;
};

#endif

#include "Arduino.h"
#include "RGBDisplay.cpp"

class GFXDisplay {
  private:
    RGBDisplay *rgb;

  public:
    GFXDisplay(uint8_t color_depth) {
      rgb = new RGBDisplay(color_depth);
      Serial.println("GFX Setup");  
      for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
          rgb->setPixel(j, i, 0, 0, 0);
        }
      }
    }
  
    void drawScreen() {
      rgb->drawScreen();
    }

    void setPixel(uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b) {
      rgb->setPixel(row, col, r, g, b);
    }

    void fillRect(uint8_t col, uint8_t row, uint8_t width, uint8_t height, uint8_t r, uint8_t g, uint8_t b) {
      for (int i = row; i < (row + height); i++) {
        for (int j = col; j < (col + width); j++) {
           rgb->setPixel(i, j, r, g, b); 
        }
      }
    }

    void clearScreen() {
      fillRect(0, 0, 64, 32, 0, 0, 0);
    }
};

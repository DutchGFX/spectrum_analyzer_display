#include "Arduino.h"
#include "RGBDisplay.cpp"

#define RAINBOW_SPACE 5

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

    void fillRect(uint8_t col, uint8_t row, uint8_t width, uint8_t height, uint16_t c) {
      uint8_t r = c >> 8;
      uint8_t g = (c >> 4) & 15;
      uint8_t b = c & 15;
      
      for (int i = row; i < (row + height); i++) {
        for (int j = col; j < (col + width); j++) {
           rgb->setPixel(i, j, r, g, b); 
        }
      }
    }

    uint16_t getColor(uint8_t r, uint8_t g, uint8_t b) {


      uint16_t temp = r;
      temp = temp << 4;
      temp |= g;
      temp = temp << 4;
      temp |= b;
      return temp;

    }

    uint16_t getRainbow(uint8_t c) {


      uint8_t r = 0;
      uint8_t g = 0;
      uint8_t b = 0;

      if (c < RAINBOW_SPACE * 1)
      {
        r = 15;
        g = (c - (RAINBOW_SPACE * 0)) * 15 / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 2)
      {
        g = 15;
        r = ((RAINBOW_SPACE * 2) - c) * 15 / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 3)
      {
        g = 15;
        b = (c - (RAINBOW_SPACE * 2)) * 15 / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 4)
      {
        b = 15;
        g = ((RAINBOW_SPACE * 4) - c) * 15 / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 5)
      {
        b = 15;
        r = (c - (RAINBOW_SPACE * 4)) * 15 / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 6)
      {
        r = 15;
        b = ((RAINBOW_SPACE * 6) - c) * 15 / RAINBOW_SPACE;
      }
      else
      {
        r = 15;
      }
      
      uint16_t temp = r;
      temp = temp << 4;
      temp |= g;
      temp = temp << 4;
      temp |= b;
      return temp;
    }


    void clearScreen() {
      fillRect(0, 0, 64, 32, 0);
    }
};

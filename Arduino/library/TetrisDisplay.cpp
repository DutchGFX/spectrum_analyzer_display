#include "Arduino.h"
#include "RGBDisplay.cpp"

#define RAINBOW_MAX     32
#define RAINBOW_SPACE   (RAINBOW_MAX / 6)
#define MAX_BRIGHTNESS  15
#define HALF_BRIGHTNESS 7

#define WHITE         gfx->getColor( MAX_BRIGHTNESS,  MAX_BRIGHTNESS,  MAX_BRIGHTNESS)
#define RED           gfx->getColor( MAX_BRIGHTNESS,               0,               0)
#define ORANGE        gfx->getColor( MAX_BRIGHTNESS, HALF_BRIGHTNESS,  MAX_BRIGHTNESS)
#define YELLOW        gfx->getColor( MAX_BRIGHTNESS,  MAX_BRIGHTNESS,               0)
#define LIGHT_GREEN   gfx->getColor(HALF_BRIGHTNESS,  MAX_BRIGHTNESS,               0)
#define GREEN         gfx->getColor(              0,  MAX_BRIGHTNESS,               0)
#define TEAL          gfx->getColor(              0,  MAX_BRIGHTNESS, HALF_BRIGHTNESS)
#define LIGHT_BLUE    gfx->getColor(              0,  MAX_BRIGHTNESS,  MAX_BRIGHTNESS)
#define MEDIUM_BLUE   gfx->getColor(              0, HALF_BRIGHTNESS,  MAX_BRIGHTNESS)
#define BLUE          gfx->getColor(              0,               0,  MAX_BRIGHTNESS)
#define PURPLE        gfx->getColor(HALF_BRIGHTNESS,               0,  MAX_BRIGHTNESS)
#define PINK          gfx->getColor( MAX_BRIGHTNESS,               0,  MAX_BRIGHTNESS)
#define MAJENTA       gfx->getColor( MAX_BRIGHTNESS,               0, HALF_BRIGHTNESS)

class TetrixDisplay {
  private:
    RGBDisplay *rgb;
    uint8_t _row;
    uint8_t _col;

  public:
    TetrixDisplay() {
      rgb = new RGBDisplay(16);

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

    void play() {

    }


    void clearScreen() {
    }

  
};

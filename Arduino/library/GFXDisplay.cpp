#include "Arduino.h"
#include "RGBDisplay.cpp"
#include "letters.h"

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

class GFXDisplay {
  private:
    RGBDisplay *rgb;
    uint8_t _row;
    uint8_t _col;
    unsigned _t;
    int8_t _r_d;
    int8_t _c_d;
    uint16_t _colors[12] = {3840, 3967, 4080, 2032, 240, 247, 255, 127, 15, 1807, 3855, 3847};
    uint8_t _color_index;

  public:
    GFXDisplay(uint8_t color_depth) {
      rgb = new RGBDisplay(color_depth);
      _row = 0;
      _col = 0;
      _t = millis();
      _r_d = 1;
      _c_d = -1;
      _color_index = 0;
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
        r = MAX_BRIGHTNESS;
        g = (c - (RAINBOW_SPACE * 0)) * MAX_BRIGHTNESS / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 2)
      {
        g = MAX_BRIGHTNESS;
        r = ((RAINBOW_SPACE * 2) - c) * MAX_BRIGHTNESS / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 3)
      {
        g = MAX_BRIGHTNESS;
        b = (c - (RAINBOW_SPACE * 2)) * MAX_BRIGHTNESS / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 4)
      {
        b = MAX_BRIGHTNESS;
        g = ((RAINBOW_SPACE * 4) - c) * MAX_BRIGHTNESS / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 5)
      {
        b = MAX_BRIGHTNESS;
        r = (c - (RAINBOW_SPACE * 4)) * MAX_BRIGHTNESS / RAINBOW_SPACE;
      }
      else if (c < RAINBOW_SPACE * 6)
      {
        r = MAX_BRIGHTNESS;
        b = ((RAINBOW_SPACE * 6) - c) * MAX_BRIGHTNESS / RAINBOW_SPACE;
      }
      else
      {
        r = MAX_BRIGHTNESS;
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

    void printLetter(char letter, uint16_t c) {

      if (letter == ' ')
      {
        if (_col != 0)
        {
          _col++;
        }
        return;
      }
      if (letter == '\n')
      {
        _col = 0;
        _row += 8;
        return;
      }

      uint8_t r = c >> 8;
      uint8_t g = (c >> 4) & 15;
      uint8_t b = c & 15;


      uint8_t index = letter - 'A';

      uint8_t new_col;
      for (int i = 0; i < 8; i++)
      {
        uint8_t temp = letters[index][i];
        Serial.println(temp);
        for (int j = 6; j >= 0; j--)
        {
          if ((temp >> j) & 1)
          {
            rgb->setPixel(_row + i, _col + 6 - j, r, g, b);
          }
        }
        if ((temp >> 7) & 1)
        {
          new_col = i;
        } 
      }
      _col += new_col;



    }

    void printWord(char *s, uint16_t c) {
      unsigned i = 0;
      while (s[i] != 0)
      {
        printLetter(s[i], c);
        i++;
      }
    }

    void scrollWord(char *s, uint16_t c) {


    }

    void bounceWord(char *s, uint8_t d) {

      uint16_t c = _colors[_color_index];

      if (millis() > (_t + d))
      {
        clearScreen();
        uint8_t old_row = _row;
        uint8_t old_col = _col;

        if (_col == 0)
        {
          _c_d = -_c_d;
          _color_index += 5;
        }

        printWord(s, c);
        _row = old_row + _r_d;

        if (_row == 25 || _row == 0)
        {
          _r_d = -_r_d;
          _color_index += 5;
        }

        if (_col == 64)
        {
          _c_d = -_c_d;
          _color_index += 5;
        }
        _col = old_col + _c_d;

        if (_color_index > 12)
        {
          _color_index -= 12;
        }
        _t = millis();
      }
    }

    void setCursor(int r, int c) {
      _row = r;
      _col = c;
    }
};

#include "Arduino.h"
#include "RGBDisplay.cpp"

#define RAINBOW_SPACE 5
#define WHITE         gfx->getColor(15, 15, 15)
#define RED           gfx->getColor(15,  0,  0)
#define ORANGE        gfx->getColor(15,  7, 15)
#define YELLOW        gfx->getColor(15, 15,  0)
#define LIGHT_GREEN   gfx->getColor( 7, 15,  0)
#define GREEN         gfx->getColor( 0, 15,  0)
#define TEAL          gfx->getColor( 0, 15,  7)
#define LIGHT_BLUE    gfx->getColor( 0, 15, 15)
#define MEDIUM_BLUE   gfx->getColor( 0, 7 , 15)
#define BLUE          gfx->getColor( 0, 0 , 15)
#define PURPLE        gfx->getColor( 7, 0 , 15)
#define PINK          gfx->getColor( 15, 0, 15)
#define MAJENTA       gfx->getColor( 15, 0,  7)

class GFXDisplay {
  private:
    RGBDisplay *rgb;
    uint8_t _row;
    uint8_t _col;

  public:
    GFXDisplay(uint8_t color_depth) {
      rgb = new RGBDisplay(color_depth);
      _row = 0;
      _col = 0;
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

    void printLetter(char letter, uint16_t c) {
      if (letter == ' ')
      {
        if (_col != 0)
        {
          _col++; 
          if (_col > 58)
          {
            _col = 0;
            _row += 8;
          }
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

      uint8_t letters[26][8] = {
        // a
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00011100,
        0b00100010,
        0b00100110,
        0b10011010,
        0b00000000},

        // b
       {0b00100000,
        0b00100000,
        0b00100000,
        0b00111100,
        0b00100010,
        0b00100010,
        0b10011100,
        0b00000000},

        // c
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00011100,
        0b00100000,
        0b10100000,
        0b00011100,
        0b00000000},

        // d
       {0b00000010,
        0b00000010,
        0b00000010,
        0b00011110,
        0b00100010,
        0b00100010,
        0b10011100,
        0b00000000},

        // e
       {0b00000000,
        0b00000000,
        0b00011000,
        0b00100100,
        0b00111000,
        0b10100000,
        0b00011100,
        0b00000000},

        // f
       {0b00000100,
        0b00001010,
        0b00001000,
        0b00001000,
        0b00011100,
        0b10001000,
        0b00001000,
        0b00000000},

        // g
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00011000,
        0b00100100,
        0b10011100,
        0b00000100,
        0b00011000},

        // h
       {0b00000000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00011100,
        0b10010010,
        0b00010010,
        0b00000000},

        // i
       {0b00000000,
        0b00000000,
        0b00010000,
        0b10000000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b00000000},

        // j
       {0b00000000,
        0b00000000,
        0b00001000,
        0b00000000,
        0b10001000,
        0b00001000,
        0b00101000,
        0b00010000},

        // k
       {0b00000000,
        0b00100000,
        0b00100000,
        0b00101000,
        0b00110000,
        0b10101000,
        0b00100100,
        0b00000000},

        // l
       {0b00000000,
        0b00100000,
        0b10100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00100000,
        0b00000000},

        // m
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00010100,
        0b00101010,
        0b10101010,
        0b00000000},

        // n
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10010000,
        0b00101000,
        0b00101000,
        0b00000000},

        // o
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00011000,
        0b00100100,
        0b10100100,
        0b00011000,
        0b00000000},

        // p
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00111000,
        0b00100100,
        0b10111000,
        0b00100000,
        0b00100000},

        // q
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00111000,
        0b00101000,
        0b10111000,
        0b00001000,
        0b00000100},

        // r
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00011000,
        0b10100000,
        0b00100000,
        0b00100000,
        0b00000000},

        // s
       {0b00000000,
        0b00000000,
        0b00011000,
        0b00100000,
        0b10010000,
        0b00001000,
        0b00110000,
        0b00000000},

        // t
       {0b00000000,
        0b00010000,
        0b00010000,
        0b00010000,
        0b10111000,
        0b00010000,
        0b00010000,
        0b00000000},

        // u
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00100100,
        0b00100100,
        0b10100100,
        0b00011100,
        0b00000000},

        // v
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00101000,
        0b10101000,
        0b00101000,
        0b00010000,
        0b00000000},

        // w
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00100010,
        0b00101010,
        0b00101010,
        0b10010100,
        0b00000000},

        // x
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10101000,
        0b00010000,
        0b00101000,
        0b00000000},

        // y
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10101000,
        0b00101000,
        0b00010000,
        0b00100000},

        // z
       {0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b10111000,
        0b00010000,
        0b00111000,
        0b00000000}
                    };

      uint8_t index = letter - 'a';
      uint8_t new_col;
      for (int i = 0; i < 8; i++)
      {
        uint8_t temp = letters[index][i];
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

      if (_col > 58)
      {
        _col = 0;
        _row += 8;
      }


    }

    void printWord(char *s, uint16_t c) {
      unsigned i = 0;
      while (s[i] != 0)
      {
        printLetter(s[i], c);
        i++;
      }
    }

    void scrollWord() {

    }
};

#include "Arduino.h"

#define R1_PIN  23
#define G1_PIN  22
#define B1_PIN  15
#define R2_PIN  16
#define G2_PIN  21
#define B2_PIN  25

#define A_PIN   26
#define B_PIN   19 
#define C_PIN   27
#define D_PIN   18
          
#define LAT_PIN 17
#define OE_PIN  13

#define CLK_PIN 14

#define HEIGHT  32
#define WIDTH   64

inline void disableLEDs() __attribute__((always_inline));
inline void enableLEDs() __attribute__((always_inline));
inline void loadShiftRegister() __attribute__((always_inline));



class RGBDisplay {
  private:
    uint8_t _red[HEIGHT][WIDTH];
    uint8_t _green[HEIGHT][WIDTH];
    uint8_t _blue[HEIGHT][WIDTH];
    uint8_t _color_depth;

    void init_pins() {
    
      pinMode(R1_PIN, OUTPUT);
      pinMode(G1_PIN, OUTPUT);
      pinMode(B1_PIN, OUTPUT);
      pinMode(R2_PIN, OUTPUT);
      pinMode(G2_PIN, OUTPUT);
      pinMode(B2_PIN, OUTPUT);
      
      pinMode(A_PIN, OUTPUT);
      pinMode(B_PIN, OUTPUT);
      pinMode(C_PIN, OUTPUT);
      pinMode(D_PIN, OUTPUT);
      
      pinMode(LAT_PIN, OUTPUT);
      pinMode(OE_PIN, OUTPUT);
      pinMode(CLK_PIN, OUTPUT);
    
      digitalWrite(LAT_PIN, LOW);
      digitalWrite(OE_PIN, LOW);
      digitalWrite(CLK_PIN, LOW);
    }
    

  public:
    RGBDisplay(uint8_t color_depth) {
      _color_depth = color_depth;
      init_pins();
    }
  
    void drawScreen() {
      for (int k = 0; k < _color_depth; k++)
      {
        for (int j = 0; j < (HEIGHT / 2); j++)
        {
          for (int i = 0; i < WIDTH; i++)
          {
            uint32_t s = 0, c = 0;

            if (_red[j][i] > k)       s |= (1 << R1_PIN);
            else                      c |= (1 << R1_PIN);
            if (_green[j][i] > k)     s |= (1 << G1_PIN);
            else                      c |= (1 << G1_PIN);
            if (_blue[j][i] > k)      s |= (1 << B1_PIN);
            else                      c |= (1 << B1_PIN);
            if (_red[j+16][i] > k)    s |= (1 << R2_PIN);
            else                      c |= (1 << R2_PIN);
            if (_green[j+16][i] > k)  s |= (1 << G2_PIN);
            else                      c |= (1 << G2_PIN);
            if (_blue[j+16][i] > k)   s |= (1 << B2_PIN);
            else                      c |= (1 << B2_PIN);
      
            GPIO.out_w1ts = s;
            GPIO.out_w1tc = c;

            GPIO.out_w1ts = 1 << CLK_PIN;
            GPIO.out_w1tc = 1 << CLK_PIN;
          }
        
          GPIO.out_w1ts = (1 << LAT_PIN) | (1 << OE_PIN);
          GPIO.out_w1tc = 1 << LAT_PIN;

          uint32_t s = 0, c = 0;

          if ((j & 1))  s |= (1 << A_PIN);
          else          c |= (1 << A_PIN);
          if ((j & 2))  s |= (1 << B_PIN);
          else          c |= (1 << B_PIN);
          if ((j & 4))  s |= (1 << C_PIN);
          else          c |= (1 << C_PIN);
          if ((j & 8))  s |= (1 << D_PIN);
          else          c |= (1 << D_PIN);

          GPIO.out_w1ts = s;
          GPIO.out_w1tc = c;

          GPIO.out_w1tc = 1 << OE_PIN;
        }
      }
    }

    void setPixel(uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b) {
      if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH)
      {
        _red[row][col] = r;
        _green[row][col] = g;
        _blue[row][col] = b;
      }
    }
};

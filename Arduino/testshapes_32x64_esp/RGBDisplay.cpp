#include "Arduino.h"

#define R1_PIN  23
#define G1_PIN  22
#define B1_PIN  32
#define R2_PIN  33
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
    
    void loadShiftRegister() {
      digitalWrite(LAT_PIN, HIGH);
      digitalWrite(LAT_PIN, LOW);
    }
    
    void enableLEDs() {
      digitalWrite(OE_PIN, LOW);
    }
    
    void disableLEDs() {
      digitalWrite(OE_PIN, HIGH);
    }

  public:
    RGBDisplay(uint8_t color_depth) {
      _color_depth = color_depth;
      init_pins();
    }
  
    void drawScreen() {
      for (int k = 0; k < (1 << _color_depth); k++)
      {
        for (int j = 0; j < (HEIGHT / 2); j++)
        {
          for (int i = 0; i < WIDTH; i++)
          {
            digitalWrite(R1_PIN, _red[j][i] > k);
            digitalWrite(G1_PIN, _green[j][i] > k);
            digitalWrite(B1_PIN, _blue[j][i] > k);
            digitalWrite(R2_PIN, _red[j + 16][i] > k);
            digitalWrite(G2_PIN, _green[j + 16][i] > k);
            digitalWrite(B2_PIN, _blue[j + 16][i] > k);
      
            digitalWrite(CLK_PIN, HIGH);
            digitalWrite(CLK_PIN, LOW);
          }
        
          disableLEDs();
          loadShiftRegister();
          
          digitalWrite(A_PIN, (j & 1));
          digitalWrite(B_PIN, (j & 2));
          digitalWrite(C_PIN, (j & 4));
          digitalWrite(D_PIN, (j & 8));
        
          enableLEDs();
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

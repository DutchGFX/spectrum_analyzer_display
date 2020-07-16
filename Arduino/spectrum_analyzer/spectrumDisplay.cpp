#include "spectrumDisplay.h"

#define R1_PIN 23
#define G1_PIN 22
#define B1_PIN 1
#define R2_PIN 21
#define G2_PIN 19
#define B2_PIN 18

#define A_PIN 5
#define B_PIN 17
#define C_PIN 16
#define D_PIN 4
#define E_PIN -1

#define LAT_PIN 2
#define OE_PIN 15

#define CLK_PIN 0

#define HEIGHT 32
#define WIDTH 64
#define ORANGE_HEIGHT 16
#define RED_HEIGHT 24

spectrumDisplay::spectrumDisplay(int bars) {
    _bars = bars;
    _tops = (uint16_t *)malloc(sizeof(uint16_t) * bars);
    _mags = (uint16_t *)malloc(sizeof(uint16_t) * bars);
    for (int i = 0; i < bars; i++) {
        _tops[i] = 0;
        _mags[i] = 0;
    }
    _dma_display.begin(R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN);
    _bar_width = WIDTH / bars;
    _mode = 1; // 1 default
    _drop = 3;
}

void spectrumDisplay::updateDisplay(uint8_t *mags) {
    _dma_display.clearScreen();
    for (int i = 0; i < _bars; i++) {
        uint8_t mag = _mags[i] >> 8;
        dropBars(i, mags[i], _tops[i]);
        updateBar(i, mag, _tops[i]);
    }
}

void spectrumDisplay::updateBar(uint8_t index, uint8_t mag, uint16_t top) {
    top = top >> 11;
    _dma_display.fillRect(index * _bar_width, HEIGHT - top - 1, _bar_width, 1, _dma_display.color444(3, 3, 3));

    if (mag == 0)
        return;

    mag = mag >> 3;

    if (_mode == 0) {
        _dma_display.fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _dma_display.color444(0, 7, 0));
        return;
    }

    if (_mode == 1) {
        if (mag <= ORANGE_HEIGHT) {
            _dma_display.fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _dma_display.color444(0, 7, 0));
            return;
        }
        _dma_display.fillRect(index * _bar_width, HEIGHT - ORANGE_HEIGHT, _bar_width, ORANGE_HEIGHT, _dma_display.color444(0, 7, 0));
        if (mag <= RED_HEIGHT) {
            _dma_display.fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag - ORANGE_HEIGHT, _dma_display.color444(7, 3, 0));
            return;
        }
        _dma_display.fillRect(index * _bar_width, RED_HEIGHT - ORANGE_HEIGHT, _bar_width, RED_HEIGHT - ORANGE_HEIGHT, _dma_display.color444(7, 3, 0));
        _dma_display.fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag - RED_HEIGHT, _dma_display.color444(7, 0, 0));
    }
}

void spectrumDisplay::checkDisplay() {
    _dma_display.setTextSize(1);      // size 1 == 8 pixels high
    _dma_display.setTextWrap(false);  // Don't wrap at end of line - will do ourselves

    _dma_display.setCursor(5, 0);  // start at top left, with 8 pixel of spacing
    uint8_t w = 0;
    char *str = "Display";
    for (w = 0; w < 7; w++) {
        _dma_display.setTextColor(_dma_display.color565(15, 15, 15));
        _dma_display.print(str[w]);
    }
    _dma_display.setCursor(5, 8);
    w = 0;
    str = "Check";
    for (w = 0; w < 5; w++) {
        _dma_display.setTextColor(_dma_display.color565(15, 15, 15));
        _dma_display.print(str[w]);
    }

    delay(2000);
    _dma_display.clearScreen();
}

void spectrumDisplay::dropBars(uint8_t index, uint8_t mag, uint16_t top) {
    if (mag > (top >> 8)) {
        _tops[index] = mag << 8;
    } else {
        if (_tops[index] > (1 << _drop)) {
            _tops[index] -= (1 << _drop);
        } else {
            _tops[index] = 0;
        }
    }

    uint16_t mag_temp = _mags[index];

    if (mag > (mag_temp >> 8)) {
        _mags[index] = mag << 8;
    } else {
        if (mag_temp > (1 << (_drop + 2))) {
            _mags[index] -= (1 << (_drop + 2));
        } else {
            _mags[index] = 0;
        }
    }
}

void spectrumDisplay::setDrop(uint8_t s) {
    _drop = s;
}


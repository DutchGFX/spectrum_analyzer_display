#include "spectrumDisplay.h"

#define HEIGHT 32
#define WIDTH 64
#define ORANGE_HEIGHT 16
#define RED_HEIGHT 24
#define MAX_MODE 10

spectrumDisplay::spectrumDisplay(int bars) {
    _bars = bars;
    _tops = (uint16_t *)malloc(sizeof(uint16_t) * bars);
    _mags = (uint16_t *)malloc(sizeof(uint16_t) * bars);
    for (int i = 0; i < bars; i++) {
        _tops[i] = 0;
        _mags[i] = 0;
    }
    _gfx = new GFXDisplay(16);
    _bar_width = WIDTH / bars;
    _mode = 2; // 1 default
    _drop = 3;
}

void spectrumDisplay::updateDisplay(uint8_t *mags) {
    _gfx->clearScreen();
    for (int i = 0; i < _bars; i++) {
        uint8_t mag = _mags[i] >> 8;
        dropBars(i, mags[i], _tops[i]);
        updateBar(i, mag, _tops[i]);
    }
}

void spectrumDisplay::updateBar(uint8_t index, uint8_t mag, uint16_t top) {
    top = top >> 11;
    _gfx->fillRect(index * _bar_width, HEIGHT - top - 1, _bar_width, 1, _gfx->getColor(15, 15, 15));

    if (mag == 0)
        return;

    mag = mag >> 3;

    if (_mode == 0) {
        _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _gfx->getColor(0, 15, 0));
        return;
    }

    
    if (_mode == 1) {
        if (mag <= ORANGE_HEIGHT) {
            _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _gfx->getColor(0, 15, 0));
            return;
        }
        _gfx->fillRect(index * _bar_width, HEIGHT - ORANGE_HEIGHT, _bar_width, ORANGE_HEIGHT, _gfx->getColor(0, 15, 0));
        if (mag <= RED_HEIGHT) {
            _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag - ORANGE_HEIGHT, _gfx->getColor(15, 8, 0));
            return;
        }
        _gfx->fillRect(index * _bar_width, RED_HEIGHT - ORANGE_HEIGHT, _bar_width, RED_HEIGHT - ORANGE_HEIGHT, _gfx->getColor(15, 7, 0));
        _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag - RED_HEIGHT, _gfx->getColor(15, 0, 0));
        return;
    }

    if (_mode == 2) {
        for (int i = 0; i < mag; i++)
        {
          _gfx->fillRect(index * _bar_width, HEIGHT - i, _bar_width, 1, _gfx->getColor(i/2, (HEIGHT - i) / 2, 0));
        }
    }
    
    if (_mode == 3) {
        _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _gfx->getColor(_bars - index/2, 0, index/2));
    }

    if (_mode == 4) {
      for (int i = 0; i < mag; i++)
      {
        _gfx->fillRect(index * _bar_width, HEIGHT - i, _bar_width, 1, _gfx->getColor(index/2, i/2, (_bars - index)/2));
      }
    }
    
    if (_mode == 5) {
      for (int i = 0; i < mag; i+=2)
      {
        if (index < (_bars / 2) && i > 15)
        {
          _gfx->fillRect(index * _bar_width, HEIGHT - i, _bar_width, 2, _gfx->getColor(0, 0, 15));
          _gfx->fillRect(index * _bar_width, HEIGHT - i + (index & 1), 1, 1, _gfx->getColor(15, 15, 15));
        }
        else
        {
          if (i & 2)
          {
            _gfx->fillRect(index * _bar_width, HEIGHT - i, _bar_width, 2, _gfx->getColor(15, 0, 0));
          }
          else
          {
            _gfx->fillRect(index * _bar_width, HEIGHT - i, _bar_width, 2, _gfx->getColor(15, 15, 15));
          }
        }
      }
    }
    
    if (_mode == 6) {
      for (int i = 0; i <= mag; i++)
      {
        _gfx->fillRect(index * _bar_width, HEIGHT - i, _bar_width, 1, _gfx->getRainbow(i));
      }
    }

    if (_mode == 7) {
        _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _gfx->getColor(mag/2, HEIGHT - mag/2, 0));
    }

    if (_mode == 8) {
        _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _gfx->getRainbow(index));
    }

    if (_mode == 9) {
        _gfx->fillRect(index * _bar_width, HEIGHT - mag, _bar_width, mag, _gfx->getRainbow(mag));
    }
    
}

void spectrumDisplay::checkDisplay() {

    Serial.println("Display check started");
    _gfx->fillRect(6, 10, 20, 20, _gfx->getColor(15, 7, 0));

    int t = millis() + 2000;

    while (t > millis())
    {
      _gfx->drawScreen();
    }

    _gfx->clearScreen();
    Serial.println("Display check done");

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


void spectrumDisplay::setColorMode(uint8_t s) {
    _mode = s;
}

void spectrumDisplay::cycleColorMode() {
    _mode++;
    if (_mode == MAX_MODE)
    {
      _mode = 0;
    }
}


void spectrumDisplay::drawScreen() {
    _gfx->drawScreen();
}

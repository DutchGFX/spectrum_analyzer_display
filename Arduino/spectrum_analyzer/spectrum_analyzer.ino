#include "config.h"
#include "spectrumDisplay.h"
#include "spectrum_analyzer.cpp"
#include "timer_interrupt.cpp"

MySpectrumAnalyzer *analyzer;
spectrumDisplay *led_display;
uint8_t mags[BANDS];
uint32_t new_sample;

/****************************************
 *            Functions                 *
 ***************************************/
void sample() {
    new_sample = analogRead(micPin);
    analyzer->insert_sample(new_sample);
}

void setup() {
    Serial.begin(115200);

    analogReadResolution(12);
    analogSetWidth(12);
    analogSetCycles(32);
    analogSetSamples(2);
    analogSetClockDiv(1);
    analogSetAttenuation(ADC_11db);
    analogSetPinAttenuation(micPin, ADC_11db);

    Serial.print("Calibration value: ");
    Serial.println(analogRead(micPin));

    /* Setup and test Spectrum Analyzer */
    analyzer = new MySpectrumAnalyzer(BANDS, BLOCKSIZE, FREQ);

//    analyzer->test_window();
    analyzer->test_sine(300.0);
    analyzer->update_mags_after_block();
    analyzer->reset_values();
    analyzer->print_mags_dB();

    /* Setup and test Display */
    led_display = new spectrumDisplay(BANDS);

    led_display->checkDisplay();
    led_display->setDrop(DROP_RATE);
    led_display->updateDisplay(analyzer->getMagnitudes());

    delay(100);

    /* Setup Microphone */
    pinMode(micPin, INPUT);
    TimerInterrupt test = TimerInterrupt(FREQ, sample);
}

int last_millis = millis();
void loop() {
    if (millis() > last_millis + 1000.0 / FPS) {
            led_display->updateDisplay(analyzer->getMagnitudes());
            last_millis = millis();
    }

    if (analyzer->buffer_full()) {
            analyzer->update_mags_after_block();
    }
}

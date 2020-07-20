#include "config.h"
#include "spectrumDisplay.h"
#include "spectrum_analyzer.cpp"
#include "timer_interrupt.cpp"

MySpectrumAnalyzer *analyzer;
spectrumDisplay *led_display;
uint8_t *mags;
uint32_t new_sample;
TaskHandle_t Task1;

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
    led_display->setColorMode(COLOR_MODE);

    delay(100);

    /* Setup Microphone */
    pinMode(micPin, INPUT);
    TimerInterrupt test = TimerInterrupt(FREQ, sample);

    xTaskCreatePinnedToCore(
        Task1code, /* Task function. */
        "Task1",   /* name of task. */
        1000,      /* Stack size of task */
        NULL,      /* parameter of the task */
        1,         /* priority of the task */
        &Task1,    /* Task handle to keep track of created task */
        0);        /* pin task to core 0 */
}

int last_millis = millis();
int color_millis = millis();
void loop() {
    Serial.print("SpectrumAnalyzer running on core ");
    Serial.println(xPortGetCoreID());

    for (;;) {
        if (millis() > (last_millis + (1000.0 / FPS))) {
            mags = analyzer->getMagnitudes();
            led_display->updateDisplay(mags);
            last_millis = millis();
        }

        
        if (millis() > color_millis + CYCLE_TIME * 1000)
        {
          led_display->cycleColorMode();
          color_millis = millis();
        }
        

        if (analyzer->buffer_full()) {
            analyzer->update_mags_after_block();
        }
    }
}

void Task1code(void *pvParameters) {
    Serial.print("SpectrumDisplay running on core ");
    Serial.println(xPortGetCoreID());

    for (;;) {
        led_display->drawScreen();
        vTaskDelay(1);
    }
}

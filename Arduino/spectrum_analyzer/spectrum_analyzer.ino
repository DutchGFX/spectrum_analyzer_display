#include "spectrum_analyzer.cpp"
#include "timer_interrupt.cpp"

#define FREQ 40000
#define BANDS 32
#define BLOCKSIZE 256

const int micPin = 32; //23 or 25 OK too

MySpectrumAnalyzer *analyzer;

void sample() {analyzer->insert_sample(5.0);}

void test_callback(uint8_t* magnitudes) {

}

void setup() {
    Serial.begin(115200);
    pinMode(micPin, INPUT);
    analyzer = new MySpectrumAnalyzer(BANDS, BLOCKSIZE, FREQ);
    analyzer->test_window();
    analyzer->print_mags_dB();
    TimerInterrupt test = TimerInterrupt(FREQ, sample);
}

void loop() {
//    analyzer->update_if_full(test_callback);

//    Delay(100);
//    Serial.println(analogRead(micPin));
}

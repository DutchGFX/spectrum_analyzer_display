#include "Arduino.h"

#define _max_input 4096
#define _post_scale_factor 1 // need to adjust
#define MIN_FREQ 0.0
#define MAX_FREQ 18000.0
#define MIN_LEVEL_DB -160.0 // must be less than 0


class MySpectrumAnalyzer {
   private:
    int _numBands;
    int _numSamps;
    int cur_count = 0;

    float *window;
    float _Fs;
    float *samples;
    float *freqs_norm;
    float *coeffs_cos;
    float scale_factor;
    float scale_factor_dB;
    
    float *snm1_vec;
    float *snm2_vec;

    void roll_samples_left() {
        /* takes [x[0], x[1]...] to [x[1], x[2]...]
         last sample remains unchanged */
        for (int n = 0; n < _numSamps - 1; n++) {
            samples[n] = samples[n + 1];
        }
    }

    void define_frequencies() {
        Serial.println();
        float delta = (MAX_FREQ - MIN_FREQ) / (_numBands - 1);
        Serial.print("Creating SpectrumAnalyzer with ");
        Serial.print(_numBands);
        Serial.print(" bands centered at [");
        for (int i = 0; i < _numBands; i++) {
            freqs[i] = MIN_FREQ + i * delta;
            freqs_norm[i] = 2.0 * PI * freqs[i] / (_Fs / 2);
            Serial.print(freqs[i], 0);
            if (i < _numBands - 1) {
                Serial.print(", ");
            }
        }
        Serial.println("]");
    }

    void define_window() {
        float a[5] = {0.21557895, 0.41663158, 0.277263158, 0.083578947, 0.006947368};  // flat top window parameters
        for (int n = 0; n < _numSamps; n++) {
            window[n] = a[0];
            for (int i = 1; i < 5; i++) {
                window[n] += pow(-1, i) * a[i] * cos(2 * i * PI * n / (_numSamps - 1));
            }
        }
    }

    void define_goertzel_coeffs() {
        for (int i = 0; i < _numBands; i++) {
            //coeffs_exp[i] = Complex(cos(freqs_norm[i]), -1*sin(freqs_norm[i]));
            coeffs_cos[i] = 2.0 * cos(freqs_norm[i]);
        }
    }

    float set_scale_factor() {
      scale_factor = 0.0;
      for (int n=0; n<_numSamps; n++) {
        scale_factor += window[n];
      }
      scale_factor *= 2.0 * _max_input / _numSamps;
      scale_factor_dB = dB(scale_factor);
    }

    /***********************************************
     *            Public facing functions          *
     ***********************************************/
   public:
    float *freqs;
    float *magnitudes;
    uint8_t *magnitudes_uint8;


    MySpectrumAnalyzer(int numBands, int numSamps, float Fs) {
        _numBands = numBands;
        _numSamps = numSamps;
        _Fs = Fs;

        magnitudes = new float[_numBands];
        magnitudes_uint8 = new uint8_t[_numBands];
        freqs = new float[_numBands];       // need sample rate
        freqs_norm = new float[_numBands];  // need sample rate
        define_frequencies();

        window = new float[numSamps];
        samples = new float[numSamps];
        define_window();

        //coeffs_exp = new Complex[_numBands];
        coeffs_cos = new float[_numBands];
        snm1_vec = new float[_numBands];
        snm2_vec = new float[_numBands];
        define_goertzel_coeffs();

        set_scale_factor();

    }

    

    float *get_magnitudes() {
        return magnitudes;
    }

    void insert_sample(float new_sample) {
        roll_samples_left();
        samples[_numSamps - 1] = new_sample;
        cur_count++;
    }

    void update_if_full(void (*callback)(uint8_t *dummy)) {
        if (cur_count >= _numSamps) {
            update_mags_block();
            callback(magnitudes_uint8);  // call the callback with the updated magnitudes
            cur_count = 0;          // reset number of samples

            //print_mags_dB();
        }
    }

    void update_mags_block() {
        float snm1, snm2, s, real, imag, mag_dB;
        for (int k = 0; k < _numBands; k++) {
            snm1 = 0.0;
            snm2 = 0.0;
            s = 0.0;
            for (int n = 0; n < _numSamps; n++) {
                s = samples[n] * window[n] + coeffs_cos[k] * snm1 - snm2;  // add window back
                snm2 = snm1;
                snm1 = s;
            }
            real = (snm1 - snm2 * cos(freqs_norm[k])) / (_numSamps / 2.0);
            imag = (snm2 * sin(freqs_norm[k])) / (_numSamps / 2.0);
            magnitudes[k] = sqrt(real * real + imag * imag);
            mag_dB = dB(magnitudes[k]) - scale_factor_dB;
            magnitudes_uint8[k] = 255 - 255*mag_dB/MIN_LEVEL_DB ;
        }
    }

    void test_window() {
        for (int n = 0; n < _numSamps; n++) {
            insert_sample(window[n]*_max_input);
        }
        update_mags_block();
    }

    void test_max(int M) {
        for (int n = 0; n < _numSamps; n++) {
            insert_sample(M);
        }
        update_mags_block();
    }

    float dB(float x) {
      return 20 * log10(abs(x));
    }

    void print_mags_dB() {
        for (int i = 0; i < _numBands-30; i++) {
            Serial.print(freqs[i], 2);
            Serial.print(": ");
            Serial.print(dB(magnitudes[i]));
            Serial.print(" dB   ");
            Serial.print(dB(magnitudes[i]/scale_factor));
            Serial.print(" dB   ");
            Serial.print(magnitudes_uint8[i]);
            Serial.println("/255");
        }
    }
};

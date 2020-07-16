#include "Arduino.h"
#include "config.h"

static char outstr[15];

class MySpectrumAnalyzer {
   private:
    int _numBands;
    int _numSamps;
    int cur_count = 0;

    double *window;
    float _Fs;
    int32_t *samples;
    double *freqs_norm;
    double *cos_terms;
    double *sin_terms;
    double *coeffs_cos;
    
    double scale_factor;
    double MAX_POSSIBLE_DB;
    float TOP_DB;
    float BOTTOM_DB;
    uint8_t *freq_correction;

    double *snm1_vec;
    double *snm2_vec;

    void *linspace(double a, double b, int N, double *fill) {
        double delta = (b - a) / (N - 1);
        for (int i = 0; i < N; i++) {
            fill[i] = a + i * delta;
        }
    }

    void *logspace(double a, double b, int N, double *fill, double base) {
        double den = log10(base);
        linspace(log10(a)/den, log10(b)/den, N, fill);
        for (int i = 0; i < N; i++) {
            fill[i] = pow(base, fill[i]);
        }
    }

    void define_frequencies() {
        logspace(MIN_FREQ, MAX_FREQ, _numBands, freqs, 10);
        Serial.println("Creating SpectrumAnalyzer");
        for (int k = 0; k < _numBands; k++) {
            freqs_norm[k] = 2.0 * PI * freqs[k] / _Fs;
            cos_terms[k] = cos(freqs_norm[k]);
            sin_terms[k] = sin(freqs_norm[k]);
            freq_correction[k] = 15/(MAX_FREQ - MIN_FREQ) * (freqs[k] - MIN_FREQ);
        }

        freqs[0] = 100;
        freqs[1] = 250;
        freqs[2] = 450;
        freqs[3] = 565;
        freqs[4] = 715;
        freqs[5] = 900;
        freqs[6] = 1125;
        freqs[7] = 1400;
        freqs[8] = 1750;
        freqs[9] = 2250;
        freqs[10] = 2800;
        freqs[11] = 3150;
        freqs[12] = 4000;
        freqs[13] = 5000;
        freqs[14] = 6400;
        freqs[15] = 12500;
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
            coeffs_cos[i] = 2.0 * cos(freqs_norm[i]);
        }
    }

    double set_scale_factor() {
        scale_factor = 0.0;
        for (int n = 0; n < _numSamps; n++) {
            scale_factor += window[n];
        }
        scale_factor *= 2.0 * _max_input / _numSamps;
        MAX_POSSIBLE_DB = dB(scale_factor);
        TOP_DB = MAX_POSSIBLE_DB - TOP_OFFSET_DB;
        BOTTOM_DB = TOP_DB - RANGE_DB;
    }

//    void update_dc_offset() {
//      // butterworth filter with wn = .1/(30e3/2)
//      // not ideal because samples reset but should work
//      if (cur_count > 0) {
//        dc_offset = 0.99997906 * dc_offset + 1.04718659e-05*(dc_offset + samples[cur_count-1]) + 1.04718659e-05*(dc_offset + samples[cur_count-2]);
//      }
//    }

    /***********************************************
     *            Public facing functions          *
     ***********************************************/
   public:
    double *freqs;
    double *magnitudes;
    uint8_t *magnitudes_uint8;
//    float dc_offset = 0;


    MySpectrumAnalyzer(int numBands, int numSamps, float Fs) {
        _numBands = numBands;
        _numSamps = numSamps;
        _Fs = Fs;

        magnitudes = new double[_numBands];
        magnitudes_uint8 = new uint8_t[_numBands];
        for (int i = 0; i < _numBands; i++) {
            magnitudes[i] = 0;
            magnitudes_uint8[i] = 0;
        }
        freqs = new double[_numBands];
        freqs_norm = new double[_numBands];
        cos_terms = new double[_numBands];
        sin_terms = new double[_numBands];
        freq_correction = new uint8_t[_numBands];


        define_frequencies();

        window = new double[numSamps];
        samples = new int32_t[numSamps];
        define_window();

        coeffs_cos = new double[_numBands];
        snm1_vec = new double[_numBands];
        snm2_vec = new double[_numBands];
        define_goertzel_coeffs();

        set_scale_factor();
        reset_values();
    }

    void insert_sample(uint32_t new_sample) {
        if (cur_count >= _numSamps) {
            return;
        }
        samples[cur_count] = new_sample - dc_offset;
        cur_count++;
        update_mags_recursive();
//        update_dc_offset();
    }

    bool buffer_full() {
        bool r = cur_count == _numSamps;
        return r;
    }

    void update_mags_block() {
        double snm1, snm2, s, real, imag, mag_dB, mag;
        for (int k = 0; k < _numBands; k++) {
            snm1 = 0.0;
            snm2 = 0.0;
            s = 0.0;
            for (int n = 0; n < _numSamps; n++) {
                s = prescale_factor * samples[n] * window[n] + coeffs_cos[k] * snm1 - snm2;  // add window back
                snm2 = snm1;
                snm1 = s;
            }
            real = (snm1 - snm2 * cos(freqs_norm[k])) / (_numSamps / 2.0);
            imag = (snm2 * sin(freqs_norm[k])) / (_numSamps / 2.0);
            mag = sqrt(real * real + imag * imag);
            magnitudes[k] = lambda * mag + (1.0 - lambda) * magnitudes[k];  // add a smoothing operator
            // mag_dB = dB(magnitudes[k]) - scale_factor_dB;

           
            magnitudes_uint8[k] = 255 - 255 * mag_dB / RANGE_DB;
            if (mag_dB <= RANGE_DB) {
                magnitudes_uint8[k] = 0;
            }
        }
    }

    void update_mags_recursive() {
        double s, real, imag, mag_dB, mag;
        for (int k = 0; k < _numBands; k++) {
            s = prescale_factor * samples[cur_count - 1] * window[cur_count - 1] + coeffs_cos[k] * snm1_vec[k] - snm2_vec[k];
            snm2_vec[k] = snm1_vec[k];
            snm1_vec[k] = s;
        }
    }

    void update_mags_after_block() {
        double s, real, imag, mag_dB, mag;
        for (int k = 0; k < _numBands; k++) {
            real = (snm1_vec[k] - snm2_vec[k] * cos_terms[k]) / (_numSamps / 2.0);
            imag = (snm2_vec[k] * sin_terms[k]) / (_numSamps / 2.0);

            mag = real * real + imag * imag;
            magnitudes[k] = lambda * mag + (1.0 - lambda) * magnitudes[k];  // add a smoothing operator
            
            mag_dB = 0.5*dB(magnitudes[k]) + freq_correction[k] ;
            magnitudes_uint8[k] = 255 + 255 * (mag_dB - TOP_DB) / RANGE_DB;

            if (mag_dB <= BOTTOM_DB) {
                magnitudes_uint8[k] = 0;
            }
        }
        reset_values();
    }

    void reset_values() {
        cur_count = 0;
        for (int k = 0; k < BANDS; k++) {
            snm1_vec[k] = 0;
            snm2_vec[k] = 0;
        }
    }

    void test_window() {
        double win_max = 0;
        double win_min = 0;
        uint32_t scaled;
        for (int n = 0; n < _numSamps; n++) {
            win_min = min(win_min, window[n]);
            win_max = max(win_max, window[n]);
        }
      
        for (int n = 0; n < _numSamps; n++) {
            scaled = round(_max_input * (window[n] - win_min) / (win_max - win_min)); 
            insert_sample(scaled);
        }
    }

    void test_sine(float f) {
        double t = 0.0;
        double Ts = 1/_Fs;
        uint32_t scaled;
        for (int n = 0; n < _numSamps; n++) {
            t = n * Ts;
            insert_sample(round(min(_max_input - dc_offset, dc_offset) * cos(2.0 * PI * f * t) + dc_offset));
        }
    }

    double dB(double x) {
        return 20 * log10(abs(x));
    }

    uint8_t *getMagnitudes() {
        return magnitudes_uint8;
    }

    void print_mags_dB() {
        Serial.println("-----------------------------");
        for (int i = 0; i < _numBands; i++) {
            dtostrf(freqs[i], 5, 0, outstr);
            Serial.print(outstr);
            Serial.print(" Hz: ");
            dtostrf(dB(magnitudes[i]), 7, 1, outstr);
            Serial.print(outstr);
            Serial.print(" dB\t");
            dtostrf(dB(magnitudes[i] / scale_factor), 7, 1, outstr);
            Serial.print(outstr);
            Serial.print(" dB\t");
            dtostrf(magnitudes_uint8[i], 3, 0, outstr);
            Serial.print(outstr);
            Serial.println("/255");
        }
    }
};

#include "Arduino.h"

class TimerInterrupt {
   private:
    volatile int interruptCounter;
    int DIVIDER = 2;      // default 80
    int FCLK = 80000000;  // 80 MHz
    float _freq = 40000;  // Hz

    hw_timer_t *timer = NULL;
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

   public:
    int totalInterruptCounter;
    void (*_callback)();

    TimerInterrupt() {}
    TimerInterrupt(float freq, void (*callback)()) {
        _freq = freq;
        _callback = callback;
        init_params();
    }

    void IRAM_ATTR onTimer() {
        portENTER_CRITICAL_ISR(&timerMux);
        interruptCounter++;
        portEXIT_CRITICAL_ISR(&timerMux);
    }

    void init_params() {
        /* Use 1st timer of 4 */
        /* 1 tick is 1/(80MHZ/Divider) */
        timer = timerBegin(0, DIVIDER, true);

        /* Attach onTimer function to our timer */
        timerAttachInterrupt(timer, _callback, true);

        // /* Set alarm to call onTimer function every second 1 tick is 1us */
        int ticks_overflow = FCLK / (DIVIDER * _freq);
        timerAlarmWrite(timer, ticks_overflow, true);

        // /* Start an alarm */
        timerAlarmEnable(timer);
    }
};

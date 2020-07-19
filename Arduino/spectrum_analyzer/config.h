/**********************
*    User Settings    *
***********************/
#define BANDS 32
#define TOP_OFFSET_DB 20.0 // must be > 0; sets the top level to this many dB below the theoretical max
#define RANGE_DB 30.0 // must be > 0;  gives the dB range of the LEDs


#define prescale_factor 1 // default 1
#define dc_offset 2332
/***********************************
*    Spectrum Analyzer Settings    *
***********************************/
#define FREQ 30000 // 40000
#define BLOCKSIZE 256// 256
#define lambda 0.85 // .75

#define _max_input 4096
#define MIN_FREQ 120.0
#define MAX_FREQ 14900.0
#define micPin 35  //23 or 25 OK too
#define FPS 40.0

#define DROP_RATE 10

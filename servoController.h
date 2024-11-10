// #include <avr/io.h>
//#include <util/delay.h>
//#include "defines.h"

/*
Fixed point values:
We operate all within a range of +- MAX_int.
All numbers are fractions of this.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INT8 127
#define MAX_INT16 32767
#define MAX_UINT8 255
#define SAMPLE_PERIOD 0.079 //Makes almost 10 when upscaled

#define UINT8TOINT8(number) (int8_t)(number-(MAX_UINT8+1)/2)
#define INT8TOUINT8(number) (uint8_t)(number+(MAX_UINT8+1)/2)

//Scales from normal domain to upscaling, where an INT8 becomes INT16
#define UPSCALE(number) (int16_t)(number*MAX_INT8)
#define DOWNSCALE(number) (number/MAX_INT8)

#define KP 0.2
#define KI 0.1
#define KD 0.2

uint32_t frame = 0;

typedef struct pidController{
    uint8_t kp;
    uint8_t ki;
    uint8_t kd;

    long int errorSum;
    int prevError;
    
    uint8_t samplePeriod;
} pidController;

pidController controller;



uint8_t updatePidController(pidController* controller, uint8_t setPointRaw, uint8_t processVariableRaw);


void testPidController(pidController* controller);

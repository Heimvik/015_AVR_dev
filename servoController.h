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

#define MAX_INT 32767
#define MAX_INT8 127
#define MAX_UINT8 255
#define SAMPLE_PERIOD_MS 10

#define KP 0.2
#define KI 0.1
#define KD 0.05

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

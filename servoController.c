#include "servoController.h"

uint8_t updatePidController(pidController* controller, uint8_t setPointRaw, uint8_t processVariableRaw){
    int16_t setPoint = UPSCALE(UINT8TOINT8(setPointRaw));
    int16_t processVariable = UPSCALE(UINT8TOINT8(processVariableRaw));
    //printf("setPoint:%d setPointRaw:%d processVariable:%d processVariableRaw:%d\n\r",setPoint,setPointRaw,processVariable,processVariableRaw);

    int16_t currentError = setPoint - processVariable;
    //printf("currentError:%d\n\r",currentError);
    int32_t pTerm = (controller->kp)*currentError;

    controller->errorSum += currentError;
    int32_t iTerm = (controller->ki)*(controller->samplePeriod)*(controller->errorSum); 

    int32_t dTerm = ((controller->kd)*(currentError - controller->prevError))/controller->samplePeriod;
    //printf("pTerm:%d iTerm:%d dTerm:%d\n\r",pTerm,iTerm,dTerm);
    
    return INT8TOUINT8((pTerm+iTerm+dTerm)/MAX_INT16);
}

void displayPidController(uint8_t setPoint, uint8_t processVariable) {
    //system("clear");

    const int max_position = 255;
    char line[max_position + 1]; // To store the line representation (0-255 chars)

    for (int i = 0; i <= max_position; i++) {
        line[i] = ' ';
    }

    line[setPoint] = 'S'; // 'S' represents the setpoint
    line[processVariable] = '|'; // '|' represents the process variable

    printf("(Frame %d) SetPoint: %d ProcessVariable: %d\n", frame, setPoint, processVariable);
    printf("----------------------------------------------------\n");
    printf("%s\n", line);
    printf("----------------------------------------------------\n");
    frame++;
}

void testPidController(pidController* controller){
    uint8_t setPoint = 0;
    uint8_t processVariable = 255;

    while (1) {
        processVariable = updatePidController(controller,setPoint, processVariable);
        displayPidController(setPoint,processVariable);
        usleep(100000);
    }
}


int main(){
    controller = (pidController){
        .kp = UPSCALE(KP),
        .ki = UPSCALE(KI),
        .kd = UPSCALE(KD),
        .errorSum = 0,
        .prevError = 0,
        .samplePeriod = UPSCALE(SAMPLE_PERIOD)
    };
    printf("kp:%d ki:%d kd:%d samplePeriod:%d\n\r",controller.kp,controller.ki,controller.kd,controller.samplePeriod);
    testPidController(&controller);
}
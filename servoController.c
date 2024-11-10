#include "servoController.h"

/*
New idea:
DO NOT use floating-point aritmetics!
Instad, use two ''levels'':
-Decimal bounds: -127 to 127 represents -1,1
-Total bounds: -2**15-1 to 2**15-1 represents total bounds
The total value has to be a long int (32 bit signed)
*/


/*
To be used on variables that one want to convert from a unsigned [0-255] to [-127-127] value
*/
int8_t uint8ToInt8(uint8_t variable){
    return (int8_t)(variable-(MAX_UINT8+1)/2);
}

uint8_t intToUint8(int16_t variable){
    return (uint8_t)(variable/MAX_INT8)+(MAX_UINT8+1)/2;
}

uint8_t updatePidController(pidController* controller, uint8_t setPointRaw, uint8_t processVariableRaw){
    int8_t setPoint = uint8ToInt8(setPointRaw);
    int8_t processVariable = uint8ToInt8(processVariableRaw);
    printf("setPoint:%d setPointRaw:%d processVariable:%d processVariableRaw:%d\n\r",setPoint,setPointRaw,processVariable,processVariableRaw);

    int16_t currentError = (int16_t)setPoint - (int16_t)processVariable;
    printf("currentError:%d\n\r",currentError);
    int16_t pTerm = (controller->kp)*currentError;

    controller->errorSum += currentError;
    int16_t iTerm = (controller->ki)*(controller->samplePeriod)*(controller->errorSum);

    int16_t dTerm = ((controller->kd)*(currentError - controller->prevError))/controller->samplePeriod;
    printf("pTerm:%d iTerm:%d dTerm:%d\n\r",pTerm,iTerm,dTerm);
    return intToUint8(pTerm+iTerm+dTerm);        
}

void displayPidController(uint8_t setPoint, uint8_t processVariable) {
    system("clear");

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
    uint8_t setPoint = 255;
    uint8_t processVariable = 0;

    while (1) {
        processVariable = updatePidController(controller,setPoint, processVariable);
        displayPidController(setPoint,processVariable);
        usleep(10000);
    }
}


int main(){
    controller = (pidController){
        .kp = KP*MAX_INT8,
        .ki = KI*MAX_INT8,
        .kd = KD*MAX_INT8,
        .errorSum = 0,
        .prevError = 0,
        .samplePeriod = (uint8_t)((SAMPLE_PERIOD_MS*MAX_INT8)/1000)
    };
    printf("kp:%d ki:%d kd:%d samplePeriod:%d\n\r",controller.kp,controller.ki,controller.kd,controller.samplePeriod);
    testPidController(&controller);
}
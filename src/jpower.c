#include "jpower.h"


int readfile(const char* filename) {
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error in opening file: %s\n", filename);
        return -1;
    }
    char reading[16];
    if (fgets(reading, 16, fptr) != NULL) {
        fclose(fptr);
        return atoi(reading);
    }
    else {
        fclose(fptr);
        printf("Error in reading file: %s\n", filename);
        return -1;
    }
}


int64_t gettime_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000 + (int64_t)ts.tv_nsec / 1000;
}

float getpower_w() {
    int curr = readfile(kCurrFile);
    int volt = readfile(kVoltFile);
    return curr * volt / 1000000.;
}
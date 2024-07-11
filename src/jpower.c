#include "jpower.h"


int readfile(const char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Fail to open file: %s.\n", filename);
        return -1;
    }
    char reading[16];
    if (fgets(reading, 16, fptr) != NULL) {
        fclose(fptr);
        return atoi(reading);
    }
    else {
        fclose(fptr);
        printf("Fail to read file: %s.\n", filename);
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

void start_monitor(jpower_t *jpower, const char *logfile, int64_t interval_us) {
    jpower->curr_fd = open(kCurrFile, O_RDONLY | O_NONBLOCK);
    if (jpower->curr_fd < 0) {
        printf("Fail to open file: %s.\n", kCurrFile);
    }
    jpower->volt_fd = open(kVoltFile, O_RDONLY | O_NONBLOCK);
    if (jpower->curr_fd < 0) {
        printf("Fail to open file: %s.\n", kVoltFile);
    }

    jpower->log_fptr = fopen(logfile, "w");
    if (jpower->log_fptr == NULL) {
        printf("Fail to open file: %s.\n", logfile);
    }
    jpower->interval_us = interval_us;
    atomic_store(&(jpower->running), true);
    int ret = pthread_create(&(jpower->monitor_thread), NULL, (void *)&monitor_thread_func, (jpower_t *)jpower);
    if(0 != ret)
        printf("Fail to create monitor thread.\n");
}

void monitor_thread_func(jpower_t *jpower) {
    char buffer[BUFFER_SIZE + 64];
    int buffer_count = 0;
    int64_t timestamp = 0;
    float power = 0;
    char volt_c[16], curr_c[16];
    int volt_i, curr_i;
    while (true == atomic_load(&(jpower->running))) {
        timestamp = gettime_us();
        // power = getpower_w();

        lseek(jpower->curr_fd, 0, 0);
        read(jpower->curr_fd, curr_c, sizeof(curr_c) - 1);
        curr_i = strtod(curr_c, NULL);
        
        lseek(jpower->volt_fd, 0, 0);
        read(jpower->volt_fd, volt_c, sizeof(volt_c) - 1);
        volt_i = strtod(volt_c, NULL);

        power = volt_i * curr_i / 1000000.;

        buffer_count += sprintf(&buffer[buffer_count], "%ld, %.2f\n", timestamp, power);
        // buffer_count += sprintf(&buffer[buffer_count], "%ld, %s, %s\n", timestamp, volt_c, curr_c);
        if (buffer_count >= BUFFER_SIZE) {
            fwrite(buffer, BUFFER_SIZE, 1, jpower->log_fptr);
            buffer_count -= BUFFER_SIZE;
            memcpy(buffer, &buffer[BUFFER_SIZE], buffer_count) ;
        }
        else
        {
            // usleep(jpower->interval_us);
        }
    }
    if(buffer_count > 0 )
    {
        fwrite(buffer, 1, buffer_count, jpower->log_fptr) ;    
    }
}

void end_monitor(jpower_t *jpower) {
    atomic_store(&(jpower->running), false);
    pthread_join(jpower->monitor_thread, NULL);
    fclose(jpower->log_fptr);
}

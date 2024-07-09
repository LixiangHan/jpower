#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef _H_JPOWER_
#define _H_JPOWER_

#define BUFFER_SIZE 4096

static const char kCurrFile[] = "/sys/bus/i2c/drivers/ina3221/7-0040/hwmon/hwmon5/curr1_input";
static const char kVoltFile[] = "/sys/bus/i2c/drivers/ina3221/7-0040/hwmon/hwmon5/in1_input";

typedef struct {
    atomic_bool running;
    int64_t interval_us;
    pthread_t monitor_thread;
    FILE *log_fptr;
} jpower_t;

int readfile(const char *filename);

int readfile_mmap(const char *filename);

int64_t gettime_us();

float getpower_w();

void start_monitor(jpower_t *jpower, const char *logfile, int64_t interval_us);

void monitor_thread_func(jpower_t *jpower);

void end_monitor(jpower_t *jpower);

#endif
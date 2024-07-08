#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifndef _H_JPOWER_
#define _H_JPOWER_

static const char kCurrFile[] = "/sys/bus/i2c/drivers/ina3221/7-0040/hwmon/hwmon5/curr1_input";
static const char kVoltFile[] = "/sys/bus/i2c/drivers/ina3221/7-0040/hwmon/hwmon5/in1_input";

int readfile(const char* filename);

int64_t gettime_us();

float getpower_w();

#endif
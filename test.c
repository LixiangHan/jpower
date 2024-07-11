#include <stdio.h>
#include "jpower.h"

int main() {
    const char* filename = "/sys/bus/i2c/drivers/ina3221/7-0040/hwmon/hwmon5/in1_input";
    printf("Timestamp = %ld\n", gettime_us());
    printf("Voltage = %d mV\n", readfile(filename));
    printf("Power = %.2f W\n", getpower_w());

    jpower_t jpower;
    start_monitor(&jpower, "test.log", 0);
    usleep(1000 * 1000);
    end_monitor(&jpower);

    return 0;
}
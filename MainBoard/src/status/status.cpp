#include "status.h"
#include <Arduino.h>

unsigned char run_car = 0x00;

unsigned char car_ready() {
    return run_car;
}

void start_car() {
    run_car = 0x01;
}

void shutdown_car(const char* message) {
    run_car = 0x00;
    // TODO: Store error message in sd card mayby along with some telemetry
    Serial.printf("CAR SHUTDOWN: ");
    Serial.printf(message);
}

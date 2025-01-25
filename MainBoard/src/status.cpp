#include "status.h"

#define SERIAL_DEBUG // Comment out this line if you don't want the shutdown message printed to the serial console

#ifdef SERIAL_DEBUG
#include <Arduino.h>  // Ignore clang error
#endif

// Status codes:
//    0x00 = Shutdown state
//    0x01 = Running state
unsigned char status = 0x00;

unsigned char get_status() { return status; }

void set_status(unsigned char value) { status = value; }

void shutdown_car(const char* msg) {
  status = 0x00;
#ifdef SERIAL_DEBUG
  Serial.printf("%s\n", msg);
#endif
}

#pragma once

#define STATUS_SHUTDOWN 0x00
#define STATUS_RUNNING 0x01

unsigned char get_status();
void set_status(unsigned char value);
void shutdown_car(const char *msg);

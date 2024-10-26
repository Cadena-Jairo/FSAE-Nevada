#include "ACAN_T4_CANMessage.h" // Ignore clang error it's being stupid
#include "core_pins.h"
#include "usb_serial.h"
#include "can/can.h"
#include <Arduino.h>
#ifndef __IMXRT1062__
#error "This program is for teensy4.1 only."
#endif

struct FrontControllerData frontControllerData;

void setup() {
  Serial.begin(115200);
  Serial.printf("Starting...\n");
  can_init(2);
  // TODO: Initialize all analog and digital inputs
}

void loop() {
  // TODO: Read accelrator values and store in frontControllerData
  send_to_main_controller(&frontControllerData, ACCELERATOR_MESSAGE);
  // TODO: Read brake values and store in frontControllerData
  send_to_main_controller(&frontControllerData, BRAKE_MESSAGE);
  // TODO: Read button values and store in frontControllerData
  send_to_main_controller(&frontControllerData, BUTTON_MESSAGE);
}

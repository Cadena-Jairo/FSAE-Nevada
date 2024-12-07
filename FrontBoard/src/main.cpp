#include "can/can.h"
#include "core_pins.h"
#include "usb_serial.h"
#include <ACAN_T4.h>
#include <ACAN_T4_CANMessage.h> // Ignore clang error it's being stupid
#include <Arduino.h>
#ifndef __IMXRT1062__
#error "This program is for teensy4.1 only."
#endif

// TODO: Add the button voltage threshold to a proper value
#define BUTTON_VOLTAGE_THRESHOLD 100

#define BRAKE_FRONT 14
#define BRAKE_REAR 15
#define ACCELERATOR_1 16
#define ACCELERATOR_2 17
#define BSPD_CURRENT 18
#define RTD_BUTTON 19
#define SENSOR_7 20
#define SENSOR_8 21
#define SENSOR_16 22
#define SENSOR_18 23
#define SENSOR_17 24
#define SENSOR_9 25
#define SENSOR_15 26
#define SENSOR_14 27
#define SENSOR_10 38
#define SENSOR_11 39
#define SENSOR_12 40
#define SENSOR_13 41

struct FrontControllerData frontControllerData;

unsigned char button_to_bool(int voltage);

void setup() {
  Serial.begin(115200);
  Serial.printf("Starting...\n");
  can_init();
  // TODO: Initialize all analog and digital inputs
  pinMode(BRAKE_FRONT,    INPUT);
  pinMode(BRAKE_REAR,     INPUT);
  pinMode(ACCELERATOR_1,  INPUT);
  pinMode(ACCELERATOR_2,  INPUT);
  pinMode(BSPD_CURRENT,   INPUT);
  pinMode(RTD_BUTTON,     INPUT);
  pinMode(SENSOR_7,       INPUT);
  pinMode(SENSOR_8,       INPUT);
  pinMode(SENSOR_16,      INPUT);
  pinMode(SENSOR_18,      INPUT);
  pinMode(SENSOR_17,      INPUT);
  pinMode(SENSOR_9,       INPUT);
  pinMode(SENSOR_15,      INPUT);
  pinMode(SENSOR_14,      INPUT);
  pinMode(SENSOR_10,      INPUT);
  pinMode(SENSOR_11,      INPUT);
  pinMode(SENSOR_12,      INPUT);
  pinMode(SENSOR_13,      INPUT);
  // TODO: Remove the test values
  frontControllerData.brakeMessage.potentiometer1 = 10.1f;
  frontControllerData.brakeMessage.potentiometer2 = 11.5f;
}

void loop() {
  // TODO: When ready to submit actual values, uncomment the transmit messages
  // We are deviding each by 1023 to get a float from 0 to 1 instead of a int from 0 to 1023
  // Possible change: Pass everything as an intager instead of a float. (might make deviding the 64 bit intager a lot easier)
  frontControllerData.brakeMessage.potentiometer1 = (float) (analogRead(BRAKE_FRONT) / 1023);
  frontControllerData.brakeMessage.potentiometer2 = (float) (analogRead(BRAKE_REAR) / 1023);
  // send_to_main_controller(&frontControllerData, BRAKE_MESSAGE);
  frontControllerData.acceleratorMessage.potentiometer1 = (float) (analogRead(ACCELERATOR_1) / 1023);
  frontControllerData.acceleratorMessage.potentiometer2 = (float) (analogRead(ACCELERATOR_2) / 1023);
  // send_to_main_controller(&frontControllerData, ACCELERATOR_MESSAGE);
  frontControllerData.buttonMessage.startButton = button_to_bool(analogRead(RTD_BUTTON));
  // send_to_main_controller(&frontControllerData, ACCELERATOR_MESSAGE);
  frontControllerData.bspdMessage.bspd_current = (float) (analogRead(BSPD_CURRENT) / 1023);
  // send_to_main_controller(&frontControllerData, BSPD_MESSAGE);
  
  // TODO: Figure out what to do with these sensors if they end up existing.
  // data = (float) (analogRead(SENSOR_7) / 1023);
  // data = (float) (analogRead(SENSOR_8) / 1023);
  // data = (float) (analogRead(SENSOR_16) / 1023);
  // data = (float) (analogRead(SENSOR_18) / 1023);
  // data = (float) (analogRead(SENSOR_17) / 1023);
  // data = (float) (analogRead(SENSOR_9) / 1023);
  // data = (float) (analogRead(SENSOR_15) / 1023);
  // data = (float) (analogRead(SENSOR_14) / 1023);
  // data = (float) (analogRead(SENSOR_10) / 1023);
  // data = (float) (analogRead(SENSOR_11) / 1023);
  // data = (float) (analogRead(SENSOR_12) / 1023);
  // data = (float) (analogRead(SENSOR_13) / 1023);

  // TODO: Remove this once working
  CANMessage temp_message;
  temp_message.id = 1;
  temp_message.dataFloat[0] = 20.5f;
  temp_message.dataFloat[1] = 50.1f;
  ACAN_T4::can2.tryToSend(temp_message);
}

// TODO: If different buttons have different voltage thresholds, then add that as an argument.
unsigned char button_to_bool(int voltage) {
  if (voltage > BUTTON_VOLTAGE_THRESHOLD) {
    return 0x01;
  }
  return 0x02;
}

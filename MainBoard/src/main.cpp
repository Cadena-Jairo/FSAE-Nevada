#include "ACAN_T4_CANMessage.h" // Ignore clang error it's being stupid
#include "core_pins.h"
#include "usb_serial.h"
#ifndef __IMXRT1062__
#error "This program is for teensy4.1 only."
#endif

// #include "../lib/can/can.h"
#ifdef __clang__
#include "../lib/interrupts/interrupts.h"
#endif
#ifndef __clang__
#include <interrupts.h>
#endif
#include "status/status.h"
#include "can/can.h"
#include "sd/sd.h"
#include <Arduino.h>

// How many seconds the start button has to be pressed to start the startup procedure
#define SECONDS_TO_START_CAR 5
#define DIRECTION_FORWARD 0x01
#define DIRECTION_BACKWARD 0x00
#define INVERTER_ENABLE 0x01
#define INVERTER_DISABLE 0x00
#define INVERTER_DISCHARGE_ENABLE 0x01
#define INVERTER_DISCHARGE_DISABLE 0x00
#define INVERTER_SPEEDMODE_ENABLE 0x01
#define INVERTER_SPEEDMODE_DISABLE 0x00
// TODO: Figure out what this value should be
#define DEFAULT_TORQUE_COMMAND_LIMIT 0x00

// Can data
struct StatusData statusData;
struct FrontControllerData frontControllerData;
struct BMSData bmsData;
struct CommandMessage commandMessage;

unsigned char button_time(unsigned char value, unsigned int min_times, unsigned int* press_time);

/*
 * Interrupt service routines:
 *   Only change global variables here and put your logic elsewhere like the
 * loop function Do not write to serial or put any delays in your ISR.
 */
void button_interrupt() {
  Serial.printf("Pin 1 interrupt triggered!\n");
  // Executes on button press
}
void setup() {
  Serial.begin(115200);
  Serial.printf("Starting...\n");
  can_init(1);
  can_init(2);
  // Place default values into command message to remove the possibility of junk values
  commandMessage.torque_command = 0x00;
  commandMessage.speed_command = 0x00;
  commandMessage.direction_command = DIRECTION_FORWARD;
  commandMessage.inverter_enable = INVERTER_DISABLE;
  commandMessage.inverter_discharge = INVERTER_DISCHARGE_DISABLE;
  commandMessage.speed_mode_enable = INVERTER_SPEEDMODE_DISABLE;
  commandMessage.commanded_torque_limit = DEFAULT_TORQUE_COMMAND_LIMIT;
}

unsigned int start_button_press_time = 0x00;

void loop() {
  can1_rx(&statusData);
  can2_rx(&frontControllerData, &bmsData);
  
  // Logic to run when car is not running
  if (car_ready() == 0x00) {
    // See if the start button has been pressed for a certian ammount of time. If so, then start the starup procedure;
    if (button_time(frontControllerData.buttonMessage.startButton, SECONDS_TO_START_CAR, &start_button_press_time) == 0x01) {
      // Run startup checks then set run_car to 1
    }
  } else if (car_ready() == 0x01) {
    send_command_message(commandMessage);
  }

  logData(&statusData, &frontControllerData);
}

// Return 1 if a value = 1 for min_times seconds
// To use, store a u_int32 global variable and pass by address to press_time
// TODO: Place in another source file
unsigned char button_time(unsigned char value, unsigned int min_times, unsigned int* press_time) {
  if (value == 0x01 && *press_time == 0x00) {
    *press_time = millis();
  } else if (value == 0x01 && *press_time != 0x00) {
    if (millis() > (*press_time + (1000 * min_times))) {
      return 0x01;
    }
  } else {
    return 0x00;
  }
}

#pragma once
#include <ACAN_T4.h> // Ignore clang error. It dosn't know what it's talking about.

#define BIT_RATE1 250
#define BIT_RATE2 500
#define BIT_RATE3 500

/*-------------START-CAN-2---------------------*/
// Don't know what we should do with this.
// We need to find a way to tell if we lost
// connection with the front controller;
#define WATCH_DOG_MESSAGE 0x00
#define ACCELERATOR_MESSAGE 0x01
#define BRAKE_MESSAGE 0x02
#define BUTTON_MESSAGE 0x03
struct WatchDogMessage {
  unsigned long long int count;
};
struct AcceleratorMessage {
  float potentiometer1;
  float potentiometer2;
};
struct BrakeMessage {
  float potentiometer1;
  float potentiometer2;
};
struct ButtonMessage {
  unsigned char startButton;
};
struct FrontControllerData {
  struct WatchDogMessage watchDogMessage;
  struct AcceleratorMessage acceleratorMessage;
  struct BrakeMessage brakeMessage;
  struct ButtonMessage buttonMessage;
};

void can_init(unsigned char can_number);
void can2_tx(CANMessage message);
void send_to_main_controller(struct FrontControllerData *data,
                             unsigned short message_id);

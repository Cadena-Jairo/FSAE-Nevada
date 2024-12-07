#pragma once
#include <ACAN_T4.h> // Ignore clang error. It dosn't know what it's talking about.

#define BIT_RATE1 500
#define BIT_RATE2 500
#define BIT_RATE3 500

/*-------------START-CAN-2---------------------*/
// Don't know what we should do with this.
// We need to find a way to tell if we lost
// connection with the front controller;
#define WATCH_DOG_MESSAGE 0x00
#define ACCELERATOR_MESSAGE 0x01
#define BRAKE_MESSAGE 0x02
#define BSPD_MESSAGE 0x03
#define BUTTON_MESSAGE 0x04
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
struct BSPDMessage {
  float bspd_current;
};
struct ButtonMessage {
  unsigned char startButton;
};
struct FrontControllerData {
  struct WatchDogMessage watchDogMessage;
  struct AcceleratorMessage acceleratorMessage;
  struct BrakeMessage brakeMessage;
  struct BSPDMessage bspdMessage;
  struct ButtonMessage buttonMessage;
};

void can_init();
void can1_tx(CANMessage message);
void send_to_main_controller(struct FrontControllerData *data,
                             unsigned short message_id);

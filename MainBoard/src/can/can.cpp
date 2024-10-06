#include "core_pins.h"
#include "usb_serial.h"
#include <Arduino.h> // Ignore clang error
#include <ACAN_T4.h>
#include "can.h"

#define BUTTON_PIN 1

void can_init() {
  ACAN_T4_Settings settings(BIT_RATE1 * 1000);
  ACAN_T4::can1.begin(settings);
  const unsigned int canError = ACAN_T4::can1.begin(settings);
  if (canError == 0x00)
    Serial.printf("Can 1 OK\n");
  else {
    Serial.printf("Can 1 initialization error: 0x%x\n", canError);
  }
  Serial.printf("Bitrate prescaler: %i\n", settings.mBitRatePrescaler);
  Serial.printf("Propagation Segment: %i\n", settings.mPropagationSegment);
  Serial.printf("Phase segment 1: %i\n", settings.mPhaseSegment1);
  Serial.printf("Phase segment 2: %i\n", settings.mPhaseSegment2);
  Serial.printf("RJW: %i\n", settings.mRJW);
  Serial.printf("Triple Sampling: ");
  Serial.printf(settings.mTripleSampling ? "yes\n" : "no\n");
  Serial.printf("Actual bitrate: %i bit/s\n", settings.actualBitRate());
  Serial.printf("Exact bitrate ? ");
  Serial.printf(settings.exactBitRate() ? "yes\n" : "no\n");
  Serial.printf("Distance from wished bitrate: %i ppm\n",
                settings.ppmFromWishedBitRate());
  Serial.printf("Sample point: %i%\n", settings.samplePointFromBitStart());
}

struct Message can_rx() {
  Message message;
  CANMessage canMessage;
  if (ACAN_T4::can1.receive(canMessage)) {
    message.data_u64 = canMessage.data64;
    message.message_id = canMessage.id;
    message.extended = canMessage.ext;
    message.length = canMessage.len;
    message.remote_frame = canMessage.rtr;
  }
  return message;
}

bool can_tx(unsigned long long message_data) {
  CANMessage message;
  message.id = 1;
  message.data64 = message_data;
  const bool ok = ACAN_T4::can1.tryToSend(message);
  if (ok) {
    Serial.printf("Message sent.\n");
  } else {
    Serial.printf("Message not sent.\n");
  }
  return ok;
}



// TODO: Implement this
// Input:  All numbers involved in a command message
// Output: An unsigned 64 bit intager that can be sent over CAN
unsigned long long int combine_command_message(struct CommandMessage message) {
  unsigned long long output;
  return output;
}












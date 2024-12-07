#include "can.h" // Ignore clang error it's being dumb
#include "ACAN_T4.h"
#include "ACAN_T4_CANMessage.h"
#include "core_pins.h"
#include "usb_serial.h"
#include "wiring.h"
#include <Arduino.h>

void can_init() {
  ACAN_T4_Settings settings(BIT_RATE1 * 1000);
  settings.mTxPinIsOpenCollector = true;
  settings.mRxPinConfiguration = ACAN_T4_Settings::PULLUP_22k;
  settings.mListenOnlyMode = false;
  settings.mLoopBackMode = false;
  ACAN_T4::can2.begin(settings);
  const unsigned int canError = ACAN_T4::can2.begin(settings);
  if (canError == 0x00)
    Serial.printf("Can 2 OK\n");
  else {
    Serial.printf("Can 2 initialization error: 0x%x\n", canError);
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

void can1_tx(CANMessage message) { ACAN_T4::can1.tryToSend(message); }

void send_to_main_controller(struct FrontControllerData *data,
                             unsigned short message_id) {
  CANMessage output;
  output.id = message_id;
  switch (message_id) {
  case ACCELERATOR_MESSAGE:
    output.dataFloat[0] = data->acceleratorMessage.potentiometer1;
    output.dataFloat[1] = data->acceleratorMessage.potentiometer2;
    // TODO: Remove this once working
    Serial.printf("Sending accelerator message: %f | %f\n",
        data->acceleratorMessage.potentiometer1,
        data->acceleratorMessage.potentiometer2);
    break;
  case BRAKE_MESSAGE:
    output.dataFloat[0] = 10.1f;
    output.dataFloat[1] = 11.2f;
    break;
  case BUTTON_MESSAGE:
    output.data[0] = data->buttonMessage.startButton;
    break;
  default:
    Serial.println("ERROR MESSAGE ID UNKNOWN");
  }
  can1_tx(output);
}

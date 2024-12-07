#include "can.h" // Ignore clang error it's being dumb
#include "../status/status.h"
#include "core_pins.h"
#include "usb_serial.h"
#include "wiring.h"
#include <Arduino.h>

// #define DEBUG_CAN to print debug messages here

#define CAN_DISCONNECT_THRESHOLD 1000 // 1 second

unsigned int last_can_message[2];

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> front_can;
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> inverter_can;

void can_init(unsigned char can_number) {
  switch (can_number) {
#ifdef DEBUG
    case 0x01:
      Serial.printf("Can 1 not used!\n");
      break;
#endif
    case 0x02:
      front_can.begin();
      front_can.setBaudRate(BIT_RATE2 * 100);
#ifdef DEBUG
      Serial.printf("CAN2 Initialized at %dkb/s\n", BIT_RATE2);
#endif
      break;
    case 0x03:
      inverter_can.begin();
      inverter_can.setBaudRate(BIT_RATE3 * 100);
#ifdef DEBUG
      Serial.printf("CAN3 Initialized at %dkb/s\n", BIT_RATE3);
#endif
      break;
  }
}

#define CAN_DATA data64
#define DATA64_INIT (unsigned long long int) canMessage.buf[0] << 56 | (unsigned long long int) canMessage.buf[1] << 48 | (unsigned long long int) canMessage.buf[2] << 40 | (unsigned long long int) canMessage.buf[3] << 32 | (unsigned long long int) canMessage.buf[4] << 24 | (unsigned long long int) canMessage.buf[5] << 16 | (unsigned long long int) canMessage.buf[6] <<  8 | (unsigned long long int) canMessage.buf[7] <<  0;

void can3_rx(StatusData *statusData) {
  CAN_message_t canMessage;
  inverter_can.read(canMessage);

  if (canMessage.len == 0x00)
    return;

  last_can_message[0] = millis();

  unsigned long long int data64 = DATA64_INIT;

  // TODO: Test to make sure this is still working and remove it.
  Serial.printf("MessageID: 0x%lx Message: 0x%llx\n", canMessage.id, data64);

  switch (canMessage.id) {
  case TEMPS_1:
    statusData->tempratureMessage1.moduleA_temp =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->tempratureMessage1.moduleB_temp =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->tempratureMessage1.moduleC_temp =
        (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->tempratureMessage1.gate_driver_board_temp =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case TEMPS_2:
    statusData->tempratureMessage2.rtd1_temp =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->tempratureMessage2.rtd1_temp =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->tempratureMessage2.rtd1_temp =
        (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->tempratureMessage2.rtd1_temp =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case TEMPS_3:
    statusData->tempratureMessage3.rtd4_coolant_temp =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->tempratureMessage3.rtd5_hot_spot_temp =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->tempratureMessage3.motor_temp =
        (0x0000ffff0000000000 & CAN_DATA) >> 32;
    statusData->tempratureMessage3.torque_shudder =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case DIGITAL_INPUT_STATUS:
    statusData->digitalInputStatus.digital_input_1 =
        (0b00000001 & CAN_DATA) >> 0;
    statusData->digitalInputStatus.digital_input_2 =
        (0b00000010 & CAN_DATA) >> 1;
    statusData->digitalInputStatus.digital_input_3 =
        (0b00000100 & CAN_DATA) >> 2;
    statusData->digitalInputStatus.digital_input_4 =
        (0b00001000 & CAN_DATA) >> 3;
    statusData->digitalInputStatus.digital_input_5 =
        (0b00010000 & CAN_DATA) >> 4;
    statusData->digitalInputStatus.digital_input_6 =
        (0b00100000 & CAN_DATA) >> 5;
    statusData->digitalInputStatus.digital_input_7 =
        (0b01000000 & CAN_DATA) >> 6;
    statusData->digitalInputStatus.digital_input_8 =
        (0b10000000 & CAN_DATA) >> 7;
    break;
  case MOTOR_POSITION_INFO:
    statusData->motorPositionInfo.motor_angle =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->motorPositionInfo.motor_speed =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->motorPositionInfo.electrical_output_req =
        (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->motorPositionInfo.delta_resolver_filtered =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case CURRENT_INFO:
    statusData->currentInfo.phasea_current =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->currentInfo.phaseb_current =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->currentInfo.phasec_current =
        (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->currentInfo.dc_bus_current =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case VOLTAGE_INFO:
    statusData->voltageInfo.dc_bus_voltage =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->voltageInfo.output_voltage =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->voltageInfo.vab_vd_voltage =
        (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->voltageInfo.vbc_vq_voltage =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case FLUX_INFO:
    statusData->fluxInfo.flux_command =  (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->fluxInfo.flux_feedback = (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->fluxInfo.id_feedback =   (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->fluxInfo.iq_feedback =   (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case INTERNAL_VOLTAGES:
    statusData->internalVoltages.reference_voltage_15v =
        (0x000000000000ffff & CAN_DATA) >> 0;
    statusData->internalVoltages.reference_voltage_25v =
        (0x00000000ffff0000 & CAN_DATA) >> 16;
    statusData->internalVoltages.reference_voltage_50v =
        (0x0000ffff00000000 & CAN_DATA) >> 32;
    statusData->internalVoltages.system_voltage_120v =
        (0xffff000000000000 & CAN_DATA) >> 48;
    break;
  case INTERNAL_STATES:
    // TODO: Store this data into the struct.
    break;
  case FAULT_CODES:
    // TODO: Store this data into the struct. (Although I don't think this would be very likely to happen)
    break;
  case TORQUE_AND_TIMER_INFO:
    // TODO: Store this data into the struct.
    break;
  }
  
  // TODO: REMOVE THIS
  /*
  float temp1, temp2, temp3;
  temp1 = (float) statusData->tempratureMessage1.moduleA_temp / 10;
  temp2 = (float) statusData->tempratureMessage1.moduleB_temp / 10;
  temp3 = (float) statusData->tempratureMessage1.moduleC_temp / 10;
  Serial.printf(
      "Module A Temp: %0.2f  Module B Temp: %0.2f  Module C Temp: %0.2f\n",
      temp1, temp2, temp3);
  */
}

void can2_rx(FrontControllerData *frontControllerData, BMSData* bmsData) {
  CAN_message_t canMessage;
  front_can.read(canMessage);

  if (canMessage.len == 0x00)
    return;

  unsigned long long int data64 = DATA64_INIT;
  
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.printf("Recieved message %ud\n", canMessage.id);

  last_can_message[1] = millis();

  switch (canMessage.id) {
  case WATCH_DOG_MESSAGE:
    // Not being used. We are using any message as a valid watch dog so we don't clog up the stream
    break;
  case ACCELERATOR_MESSAGE:
    frontControllerData->acceleratorMessage.potentiometer1 =
        (float)((0x00000000ffffffff & CAN_DATA) >> 0);
    frontControllerData->acceleratorMessage.potentiometer2 =
        (float)((0xffffffff00000000 & CAN_DATA) >> 32);
    // TODO: Remove this
    Serial.printf("Accelerator 1: %f Accelerator 2: %f\n",
      frontControllerData->acceleratorMessage.potentiometer1,
      frontControllerData->acceleratorMessage.potentiometer2);
    break;
  case BRAKE_MESSAGE:
    frontControllerData->acceleratorMessage.potentiometer1 =
        (float)((0xffffffff00000000 & CAN_DATA) >> 0);
    frontControllerData->acceleratorMessage.potentiometer2 =
        (float)((0x00000000ffffffff & CAN_DATA) >> 32);
    break;
  case BUTTON_MESSAGE:
    frontControllerData->buttonMessage.startButton = (unsigned char) CAN_DATA;
    break;
  default:
    if (canMessage.id > 3 && canMessage.id <= 73) {
      bmsData->temps[(canMessage.id - 3) / 2] = (float) *&canMessage.buf[0];
      bmsData->temps[((canMessage.id - 3) / 2) + 1] = (float) *&canMessage.buf[4];
    } else if (canMessage.id > 73 && canMessage.id < 153) {
      bmsData->voltages[(canMessage.id - 73) / 2] = (float) *&canMessage.buf[0];
      bmsData->voltages[((canMessage.id - 73) / 2) + 1] = (float) *&canMessage.buf[4];
    }
    break;
  }

  digitalWrite(LED_BUILTIN, LOW);
}

void can2_tx(CAN_message_t message) { front_can.write(message); }

void can3_tx(CAN_message_t message) { inverter_can.write(message); }

void send_command_message(struct CommandMessage message) {
  CAN_message_t canMessage;
  unsigned long long int data64 =
      (((unsigned long long int) message.commanded_torque_limit << 48) |
       ((unsigned long long int) message.speed_mode_enable << 42) |
       ((unsigned long long int) message.inverter_discharge << 41) |
       ((unsigned long long int) message.inverter_enable << 40) |
       ((unsigned long long int) message.direction_command << 32) |
       ((unsigned long long int) message.speed_command << 16) |
       ((unsigned long long int) message.torque_command));
  // Convert our unsigned long long int to an unsigned char[8]
  for (int i = 0; i < 8; i++) {
      canMessage.buf[i] = (unsigned char) (data64 >> (i * 8) & 0xFF);
  }
  can3_tx(canMessage);
}

void can_disconnect_check() {
  unsigned int curr_time = millis();
  for (int i = 0; i < 2; i ++) {
    if ((curr_time - last_can_message[i]) > CAN_DISCONNECT_THRESHOLD) {
      switch (i) {
        case 0x00:
          shutdown_car("CAN connection to inverter lost!");
          return;
        case 0x01:
          shutdown_car("CAN connection to front controller lost!");
          return;
      }
    }
  }
}

#include "can.h" // Ignore clang error it's being dumb
#include "ACAN_T4_CANMessage.h"
#include "core_pins.h"
#include "usb_serial.h"
#include "wiring.h"
#include <Arduino.h>

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

#define CAN_DATA canMessage.data64

void can_rx(StatusData* statusData) {
  CANMessage canMessage;
  ACAN_T4::can1.receive(canMessage);
  
  if (canMessage.len == 0x00) return;
  
  // TODO: Test to make sure this is still working and remove it.
  Serial.printf("MessageID: 0x%lx Message: 0x%llx\n", canMessage.id, canMessage.data64);
  
  switch (canMessage.id) {
    case TEMPS_1:
      statusData->tempratureMessage1.moduleA_temp =           (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->tempratureMessage1.moduleB_temp =           (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->tempratureMessage1.moduleC_temp =           (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->tempratureMessage1.gate_driver_board_temp = (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case TEMPS_2:
      statusData->tempratureMessage2.rtd1_temp = (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->tempratureMessage2.rtd1_temp = (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->tempratureMessage2.rtd1_temp = (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->tempratureMessage2.rtd1_temp = (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case TEMPS_3:
      statusData->tempratureMessage3.rtd4_coolant_temp =  (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->tempratureMessage3.rtd5_hot_spot_temp = (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->tempratureMessage3.motor_temp =         (0x0000ffff0000000000 & CAN_DATA) >> 32;
      statusData->tempratureMessage3.torque_shudder =     (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case DIGITAL_INPUT_STATUS:
      statusData->digitalInputStatus.digital_input_1 = (0b00000001 & CAN_DATA) >> 0;
      statusData->digitalInputStatus.digital_input_2 = (0b00000010 & CAN_DATA) >> 1;
      statusData->digitalInputStatus.digital_input_3 = (0b00000100 & CAN_DATA) >> 2;
      statusData->digitalInputStatus.digital_input_4 = (0b00001000 & CAN_DATA) >> 3;
      statusData->digitalInputStatus.digital_input_5 = (0b00010000 & CAN_DATA) >> 4;
      statusData->digitalInputStatus.digital_input_6 = (0b00100000 & CAN_DATA) >> 5;
      statusData->digitalInputStatus.digital_input_7 = (0b01000000 & CAN_DATA) >> 6;
      statusData->digitalInputStatus.digital_input_8 = (0b10000000 & CAN_DATA) >> 7;
      break;
    case MOTOR_POSITION_INFO:
      statusData->motorPositionInfo.motor_angle =             (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->motorPositionInfo.motor_speed =             (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->motorPositionInfo.electrical_output_req =   (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->motorPositionInfo.delta_resolver_filtered = (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case CURRENT_INFO:
      statusData->currentInfo.phasea_current = (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->currentInfo.phaseb_current = (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->currentInfo.phasec_current = (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->currentInfo.dc_bus_current = (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case VOLTAGE_INFO:
      statusData->voltageInfo.dc_bus_voltage = (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->voltageInfo.output_voltage = (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->voltageInfo.vab_vd_voltage = (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->voltageInfo.vbc_vq_voltage = (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case FLUX_INFO:
      statusData->fluxInfo.flux_command =  (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->fluxInfo.flux_feedback = (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->fluxInfo.id_feedback =   (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->fluxInfo.iq_feedback =   (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case INTERNAL_VOLTAGES:
      statusData->internalVoltages.reference_voltage_15v =  (0x000000000000ffff & CAN_DATA) >> 0;
      statusData->internalVoltages.reference_voltage_25v =  (0x00000000ffff0000 & CAN_DATA) >> 16;
      statusData->internalVoltages.reference_voltage_50v =  (0x0000ffff00000000 & CAN_DATA) >> 32;
      statusData->internalVoltages.system_voltage_120v =    (0xffff000000000000 & CAN_DATA) >> 48;
      break;
    case INTERNAL_STATES:
      /* FIXME: Fix bit shifts and find correct masking.
      statusData->internalStates.vsm_state =          (0x00000000000000ff & CAN_DATA) >> 0;
      statusData->internalStates.pwm_freq =           (0x000000000000ff00 & CAN_DATA) >> 8;
      statusData->internalStates.inverter_state =     (0x00000000ff000000 & CAN_DATA) >> 16;
      statusData->internalStates.relay_state =        (0x000000ff00000000 & CAN_DATA) >> 24;
      statusData->internalStates.inverter_run_mode =  (0x0000000000000000 & CAN_DATA) >> 32;
      statusData->internalStates.self_sensing_assist_enable = (0x0000000000000000 & CAN_DATA) >> 33;
      statusData->internalStates.inverter_active_discharge_state = (0x0000000000000000 & CAN_DATA) >> 37;
      statusData->internalStates.inverter_command_mode = (0x0000000000000000 & CAN_DATA) >> 37;
      statusData->internalStates.rolling_counter_value = (0x0000000000000000 & CAN_DATA) >> 40;
      statusData->internalStates.inverter_state_enable = (0x0000000000000000 & CAN_DATA) >> 44;
      statusData->internalStates.burst_model_mode = (0x0000000000000000 & CAN_DATA) >> 48;
      statusData->internalStates.start_mode_active = (0x0000000000000000 & CAN_DATA) >> 0;
      */
      break;
    case FAULT_CODES:
      break;
    case TORQUE_AND_TIMER_INFO:
      break;
  }
  float temp1, temp2, temp3;
  temp1 = (float) statusData->tempratureMessage1.moduleA_temp / 10;
  temp2 = (float) statusData->tempratureMessage1.moduleB_temp / 10;
  temp3 = (float) statusData->tempratureMessage1.moduleC_temp / 10;
  Serial.printf("Module A Temp: %0.2f  Module B Temp: %0.2f  Module C Temp: %0.2f\n",
                temp1,
                temp2,
                temp3);
}

void can_tx(CANMessage message) {
  ACAN_T4::can1.tryToSend(message);
}

void send_command_message(struct CommandMessage message) {
  CANMessage output;
  output.data64 = (
      ((unsigned long long int) message.commanded_torque_limit << 48) |
      ((unsigned long long int) message.speed_mode_enable << 42) |
      ((unsigned long long int) message.inverter_discharge << 41) |
      ((unsigned long long int) message.inverter_enable << 40) |
      ((unsigned long long int) message.direction_command << 32) |
      ((unsigned long long int) message.speed_command << 16) |
      ((unsigned long long int) message.torque_command)
  );
  can_tx(output);
}

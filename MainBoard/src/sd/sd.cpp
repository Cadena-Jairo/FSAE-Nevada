#include "sd.h"
#include "../can/can.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
// For some reason the ide knows what this is but the compiler dosn't
#ifndef BUILTIN_SDCARD
#define BUILTIN_SDCARD 254
#endif
#define PACKET_BUFFER 20 // Number of packets to store before writting to the SD card
#define FILENAME_MAX_SIZE 20

unsigned char data[sizeof(StatusData) * PACKET_BUFFER];
unsigned int data_index;
File datafile;
char filename[FILENAME_MAX_SIZE];

void generateFilename() {
  int fileIndex = 0;
  do {
    sprintf(filename, "log_%d.csv", fileIndex);
    fileIndex++;
  } while (SD.exists(filename));
}

void initSD() {
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present.");
  } else {
    Serial.println("Card initialized.");
  }
  datafile = SD.open(filename, FILE_WRITE);
  generateFilename();
  if (datafile) {
    datafile.close();
  } else {
    Serial.println("Error opening log file.");
  }
}

void sendDataSD() {
  digitalWrite(LED_BUILTIN, HIGH);
  if (SD.exists(filename)) {
    datafile = SD.open(filename, FILE_WRITE);
    if (datafile) {
      datafile.write(data, sizeof(StatusData) * PACKET_BUFFER);
      datafile.close();
    } else {
      Serial.println("ERROR - datafile did not open.");
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void logData(StatusData* inverterData, FrontControllerData* frontControllerData) {
  data[0 + data_index] = (unsigned int) millis();
  data[4 + data_index] = inverterData->tempratureMessage1.moduleA_temp;
  data[6 + data_index] = inverterData->tempratureMessage1.moduleB_temp;
  data[8 + data_index] = inverterData->tempratureMessage1.moduleC_temp;
  data[10 + data_index] = inverterData->tempratureMessage1.gate_driver_board_temp;
  data[12 + data_index] = inverterData->tempratureMessage2.control_board_temp;
  data[14 + data_index] = inverterData->tempratureMessage2.rtd1_temp;
  data[16 + data_index] = inverterData->tempratureMessage2.rtd2_temp;
  data[18 + data_index] = inverterData->tempratureMessage2.rtd3_temp;
  data[20 + data_index] = inverterData->tempratureMessage3.rtd4_coolant_temp;
  data[22 + data_index] = inverterData->tempratureMessage3.rtd5_hot_spot_temp;
  data[24 + data_index] = inverterData->tempratureMessage3.motor_temp;
  data[26 + data_index] = inverterData->tempratureMessage3.torque_shudder;
  data[28 + data_index] = inverterData->digitalInputStatus.digital_input_1;
  data[29 + data_index] = inverterData->digitalInputStatus.digital_input_2;
  data[30 + data_index] = inverterData->digitalInputStatus.digital_input_3;
  data[31 + data_index] = inverterData->digitalInputStatus.digital_input_4;
  data[32 + data_index] = inverterData->digitalInputStatus.digital_input_5;
  data[33 + data_index] = inverterData->digitalInputStatus.digital_input_6;
  data[34 + data_index] = inverterData->digitalInputStatus.digital_input_7;
  data[35 + data_index] = inverterData->digitalInputStatus.digital_input_8;
  data[36 + data_index] = inverterData->motorPositionInfo.motor_angle;
  data[38 + data_index] = inverterData->motorPositionInfo.motor_speed;
  data[40 + data_index] = inverterData->motorPositionInfo.electrical_output_req;
  data[42 + data_index] = inverterData->motorPositionInfo.delta_resolver_filtered;
  data[44 + data_index] = inverterData->currentInfo.phasea_current;
  data[46 + data_index] = inverterData->currentInfo.phaseb_current;
  data[48 + data_index] = inverterData->currentInfo.phasec_current;
  data[50 + data_index] = inverterData->currentInfo.dc_bus_current;
  data[52 + data_index] = inverterData->voltageInfo.dc_bus_voltage;
  data[54 + data_index] = inverterData->voltageInfo.output_voltage;
  data[56 + data_index] = inverterData->voltageInfo.vab_vd_voltage;
  data[58 + data_index] = inverterData->voltageInfo.vbc_vq_voltage;
  data[60 + data_index] = inverterData->fluxInfo.flux_command;
  data[62 + data_index] = inverterData->fluxInfo.flux_feedback;
  data[64 + data_index] = inverterData->fluxInfo.id_feedback;
  data[66 + data_index] = inverterData->fluxInfo.iq_feedback;
  data[68 + data_index] = inverterData->internalVoltages.reference_voltage_15v;
  data[70 + data_index] = inverterData->internalVoltages.reference_voltage_25v;
  data[72 + data_index] = inverterData->internalVoltages.reference_voltage_50v;
  data[74 + data_index] = inverterData->internalVoltages.system_voltage_120v;
  data[76 + data_index] = inverterData->internalVoltages.system_voltage_120v;
  data_index += 78;
  if (data_index >= (sizeof(StatusData) * PACKET_BUFFER) - 1) {
    sendDataSD();
    for (unsigned int i = 0; i < sizeof(StatusData) * PACKET_BUFFER; i++) {
      data[i] = 0x00;
    }
    data_index = 0x00;
  }
}

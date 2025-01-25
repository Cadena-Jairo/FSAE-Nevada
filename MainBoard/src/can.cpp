#include "can.h"

ACCUMData accumData;
FCUData fcuData;
MCUData mcuData;
IMDData imdData;

void precharge_seq() {
  if (mcuData.IR_ON_OFF == 1) {
    Serial.println("IR ON");
  } else {
    Serial.println("IR OFF");
  }
}

void printIMD() {}

void sendCANMessage(uint32_t id, uint8_t *data, uint8_t length) {
  CAN_message_t msg;
  msg.id = id;
  msg.flags.extended = 0; // Standard ID
  msg.len = length;

  memset(msg.buf, 0, sizeof(msg.buf)); // Clear buffer
  memcpy(msg.buf, data, length);       // Copy data into buffer

  if (can2.write(msg)) {
    Serial.print("Message sent: ID = 0x");
    Serial.println(id, HEX);
  } else {
    Serial.println("Failed to send CAN message.");
  }
}

void canISR(const CAN_message_t &rxMsg) {
  switch (rxMsg.id) {
  case 0x0E0:
    mcuData.LV_Batt_Voltage = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    mcuData.LV_Buck_Temp = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    mcuData.eFuse_Current[0] = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    mcuData.eFuse_Current[1] = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  case 0x0E1:
    mcuData.eFuse_Current[2] = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    mcuData.eFuse_Current[3] = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    mcuData.eFuse_Current[4] = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    mcuData.eFuse_Current[5] = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  case 0x002:
    accumData.IR_POS_ACTIVE = (rxMsg.buf[0] & 0x01);
    accumData.IR_NEG_ACTIVE = (rxMsg.buf[0] & 0x02);
    accumData.PRECHARGE_ACTIVE = (rxMsg.buf[0] & 0x04);
    accumData.IMD_ERROR = (rxMsg.buf[0] & 0x08);
    accumData.BMS_ERROR = (rxMsg.buf[0] & 0x10);
    accumData.SDC_ACTIVE = (rxMsg.buf[0] & 0x20);
    break;

  case 0x003:
    mcuData.RESET = (rxMsg.buf[0] & 0x01);
    mcuData.IR_ON_OFF = (rxMsg.buf[0] & 0x02);
    mcuData.CAR_OR_CHARGER = (rxMsg.buf[0] & 0x04);
    break;

  case 0x0D0:
    accumData.CELL_TEMP_LOW = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    accumData.CELL_TEMP_AVG = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    accumData.CELL_TEMP_HIGH = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    break;

  case 0x0D1:
    accumData.CELL_VOLTAGE_LOW = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    accumData.CELL_VOLTAGE_AVG = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    accumData.CELL_VOLTAGE_HIGH = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    accumData.PACK_VOLTAGE = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  default:
    Serial.print("Unknown CAN ID: 0x");
    Serial.println(rxMsg.id, HEX);
    break;
  }

  // Debug print
  Serial.print("Received CAN ID: 0x");
  Serial.println(rxMsg.id, HEX);
}

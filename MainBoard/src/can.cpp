#include "can.h" // Ignore stdlib.h clang error

#ifdef __clang__
void *memset(void *str, int c, size_t n);
#endif

ACCUMData accumData;
FCUData fcuData;
MCUData mcuData;
IMDData imdData;

ACCUMData *get_accumData_struct() { return &accumData; }
FCUData *get_fcuData_struct() { return &fcuData; }
MCUData *get_mcuData_struct() { return &mcuData; }
IMDData *get_imdData_struct() { return &imdData; }

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
  case FCU_THROTTLE_BRAKE:
    fcuData.Throttle_Pot1 = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    fcuData.Throttle_Pot2 = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    fcuData.Brake_Transducer1 = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    fcuData.Brake_Transducer2 = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  case MCU_POWER_DISTRO_BROADCAST1:
    mcuData.LV_Batt_Voltage = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    mcuData.LV_Buck_Temp = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    mcuData.eFuse_Current[0] = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    mcuData.eFuse_Current[1] = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  case MCU_POWER_DISTRO_BROADCAST2:
    mcuData.eFuse_Current[2] = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    mcuData.eFuse_Current[3] = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    mcuData.eFuse_Current[4] = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    mcuData.eFuse_Current[5] = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  case ACC_STATUS_BROADCAST:
    accumData.IR_POS_ACTIVE = (rxMsg.buf[0] & 0x01);
    accumData.IR_NEG_ACTIVE = (rxMsg.buf[0] & 0x02);
    accumData.PRECHARGE_ACTIVE = (rxMsg.buf[0] & 0x04);
    accumData.IMD_ERROR = (rxMsg.buf[0] & 0x08);
    accumData.BMS_ERROR = (rxMsg.buf[0] & 0x10);
    accumData.SDC_ACTIVE = (rxMsg.buf[0] & 0x20);
    break;

  case MCU_ACCUM_BROADCAST_WATCHDOG:
    mcuData.RESET = (rxMsg.buf[0] & 0x01);
    mcuData.IR_ON_OFF = (rxMsg.buf[0] & 0x02);
    mcuData.CAR_OR_CHARGER = (rxMsg.buf[0] & 0x04);
    break;

  case ACC_LOW_HIGH_AVG_CELL_TEMP:
    accumData.CELL_TEMP_LOW = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    accumData.CELL_TEMP_AVG = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    accumData.CELL_TEMP_HIGH = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    break;

  case ACC_LOW_HIGH_AVG_VOLTAGE:
    accumData.CELL_VOLTAGE_LOW = (rxMsg.buf[0] | (rxMsg.buf[1] << 8)) / 100.0;
    accumData.CELL_VOLTAGE_AVG = (rxMsg.buf[2] | (rxMsg.buf[3] << 8)) / 100.0;
    accumData.CELL_VOLTAGE_HIGH = (rxMsg.buf[4] | (rxMsg.buf[5] << 8)) / 100.0;
    accumData.PACK_VOLTAGE = (rxMsg.buf[6] | (rxMsg.buf[7] << 8)) / 100.0;
    break;

  default:
    Serial.print("Unknown CAN ID: 0x");
    Serial.println(rxMsg.id, HEX);
    return; // FIXME: Remove this. It's just here so we don't print the message
            // out twice.
    break;
  }

  // Debug print
  Serial.print("Received CAN ID: 0x");
  Serial.println(rxMsg.id, HEX);
}

#ifndef CAN_H
#define CAN_H

#include "FlexCAN_T4.h"
#include "SPI.h"
#include <Arduino.h>

// Extern CAN2 object for shared usage
extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;

void precharge_seq();
void sendCANMessage(uint32_t id, uint8_t *data, uint8_t length);
void canISR(const CAN_message_t &rxMsg);
void printIMD();

// Global Structs
struct MCUData {
  boolean RESET;
  boolean IR_ON_OFF;
  boolean CAR_OR_CHARGER;
  float eFuse_Current[6];
  float LV_Batt_Voltage;
  float LV_Buck_Temp;
};
extern MCUData mcuData;

struct FCUData {
  float Throttle_Pot1;
  float Throttle_Pot2;
  float Brake_Transducer1;
  float Brake_Transducer2;
};
extern FCUData fcuData;

struct ACCUMData {
  boolean IR_POS_ACTIVE;
  boolean IR_NEG_ACTIVE;
  boolean PRECHARGE_ACTIVE;
  boolean IMD_ERROR;
  boolean BMS_ERROR;
  boolean SDC_ACTIVE;
  float HV_HALL_CURRENT;
  float INSULATION_Kohms_MEASUREMENT;
  float CELL_TEMP_LOW;
  float CELL_TEMP_AVG;
  float CELL_TEMP_HIGH;
  float CELL_VOLTAGE_LOW;
  float CELL_VOLTAGE_AVG;
  float CELL_VOLTAGE_HIGH;
  float PACK_VOLTAGE;
};
extern ACCUMData accumData;

struct IMDData {
  float IMD_R_ISO_MEASUREMENT;
};
extern IMDData imdData;

#endif

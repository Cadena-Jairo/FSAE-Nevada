#ifndef CAN_H
#define CAN_H

#include "FlexCAN_T4.h"
#include "SPI.h"
#include <Arduino.h>

// CAN MESSAGE IDs
// FCU_ = FRONT CONTROLLER
// MCU_ = MAIN CONTROLLER
// INV_ = INVERTER
// ACC_ = ACCUMULATOR
// CHG_ = CHARGER
// IMD_ = IMD
#define FCU_THROTTLE_BRAKE 0x001
#define ACC_STATUS_BROADCAST 0x002
#define MCU_ACCUM_BROADCAST_WATCHDOG 0x003
#define CHG_ACCUM_BROADCAST_WATCHDOG 0x003
#define IMD_THRESHOLD_SET_REQUEST 0x022
#define IMD_RETURN_MSG 0x023
#define IMD_BROADCAST_MSG 0x037
#define IMD_RESERVED1 0x70e
#define IMD_RESERVED2 0x778
#define INV_TEMP1 0x0a0
#define INV_TEMP2 0x0a1
#define INV_TEMP3 0x0a2
#define INV_ANALOG_IN_VOLT 0x0a3
#define INV_DIGITAL_INPUT_STATUS 0x0a4
#define INV_MOTOR_POS_INFO 0x0a5
#define INV_CURr_INFO 0x0a6
#define INV_VOLT_INFO 0x0a7
#define INV_FLUX_INFO 0x0a8
#define INV_INTERNAL_VOLT 0x0a9
#define INV_INTERNAL_STATES 0x0aa
#define INV_FAULT_CODES 0x0ab
#define INV_TORQUE_TIMER_IN_INFO 0x0ac
#define INV_MOD_IND_FLUX_WEAK_OUT_INFO 0x0ad
#define INV_FIRMWARE_INFO 0x0ae
#define INV_DIAGNOSTIC_DATA 0x0af
#define INV_HIGH_SPEED_MSGS 0x0b0
#define INV_TORQUE_CAPABILITY 0x0b1
#define ACC_LOW_HIGH_AVG_CELL_TEMP 0x0d0
#define ACC_LOW_HIGH_AVG_VOLTAGE 0x0d1
#define ACC_RESERVED1 0x0d2
#define ACC_IMU 0x0d3
#define ACC_RESERVED2 0x0d4
#define MCU_POWER_DISTRO_BROADCAST1 0x0e0
#define MCU_POWER_DISTRO_BROADCAST2 0x0e1
#define MCU_SENSOR1 0x0e2
#define MCU_SENSOR2 0x0e3
#define MCU_IMU 0x0e4
#define FCU_BROADCAST // TODO: Figuire out id for this
#define FCU_SENSOR1 0x0f0
#define FCU_SENSOR2 0x0f1
#define FCU_SENSOR3 0x0f2
#define FCU_SENSOR4 0x0f3
#define FCU_IMU 0x0f4
#define CHG_CONTROL 0x1806e5f4
#define CHG_STATUS_INFO 0x18ff50e5

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

ACCUMData *get_accumData_struct();
FCUData *get_fcuData_struct();
MCUData *get_mcuData_struct();
IMDData *get_imdData_struct();

#endif

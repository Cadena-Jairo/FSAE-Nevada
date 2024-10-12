#pragma once
#include <ACAN_T4.h>

#define BIT_RATE1 250
#define BIT_RATE2 500
#define BIT_RATE3 500

// Inverter can message ID's
#define COMMAND_MESSAGE       0x0c0
#define TEMPS_1               0x0a0
#define TEMPS_2               0x0a1
#define TEMPS_3               0x0a2
#define DIGITAL_INPUT_STATUS  0x0a4
#define MOTOR_POSITION_INFO   0x0a5
#define CURRENT_INFO          0x0a6
#define VOLTAGE_INFO          0x0a7
#define FLUX_INFO             0x0a8
#define INTERNAL_VOLTAGES     0x0a9
#define INTERNAL_STATES       0x0aa
#define FAULT_CODES           0x0ab
#define TORQUE_AND_TIMER_INFO 0x0ac

struct CommandMessage {
  unsigned char direction_command;
  unsigned char inverter_discharge;
  unsigned char inverter_enable;
  unsigned char rolling_counter;
  unsigned short speed_command;
  unsigned char speed_mode_enable;
  unsigned short torque_command;
  unsigned short commanded_torque_limit;
};

struct TempratureMessage1 {
  unsigned short moduleA_temp;
  unsigned short moduleB_temp;
  unsigned short moduleC_temp;
  unsigned short gate_driver_board_temp;
};
struct TempratureMessage2 {
  unsigned short control_board_temp;
  unsigned short rtd1_temp;
  unsigned short rtd2_temp;
  unsigned short rtd3_temp;
};
struct TempratureMessage3 {
  unsigned short rtd4_coolant_temp;
  unsigned short rtd5_hot_spot_temp;
  unsigned short motor_temp;
  unsigned short torque_shudder;
};
struct DigitalInputStatus {
  // All values are booleans
  unsigned char digital_input_1;
  unsigned char digital_input_2;
  unsigned char digital_input_3;
  unsigned char digital_input_4;
  unsigned char digital_input_5;
  unsigned char digital_input_6;
  unsigned char digital_input_7;
  unsigned char digital_input_8;
};
struct MotorPositionInfo {
  unsigned short motor_angle;
  unsigned short motor_speed;
  unsigned short electrical_output_req;
  unsigned short delta_resolver_filtered;
};
struct CurrentInfo {
  unsigned short phasea_current;
  unsigned short phaseb_current;
  unsigned short phasec_current;
  unsigned short dc_bus_current;
};
struct VoltageInfo {
  unsigned short dc_bus_voltage;
  unsigned short output_voltage;
  unsigned short vab_vd_voltage;
  unsigned short vbc_vq_voltage;
};
struct FluxInfo {
  unsigned short flux_command;
  unsigned short flux_feedback;
  unsigned short id_feedback;
  unsigned short iq_feedback;
};
struct InternalVoltages {
  unsigned short reference_voltage_15v;
  unsigned short reference_voltage_25v;
  unsigned short reference_voltage_50v;
  unsigned short system_voltage_120v;
};
struct InternalStates {
  // 8 bit numbers
  unsigned char vsm_state;
  unsigned char pwm_freq;
  unsigned char inverter_state;
  unsigned char relay_state;
  // 1 bit booleans
  unsigned char inverter_run_mode;
  unsigned char self_sensing_assist_enable;
  // 4 bit number (yes that's right)
  unsigned char inverter_active_discharge_state;
  // 1 bit booleans
  unsigned char inverter_command_mode;
  // 4 bit number (again)
  unsigned char rolling_counter_value;
  // 1 bit booleans
  unsigned char inverter_state_enable;
  unsigned char burst_model_mode;
  unsigned char start_mode_active;
  unsigned char inverter_enable_lockout;
  unsigned char direction_command;
  unsigned char bms_active;
  unsigned char bms_limiting_torque;
  unsigned char limit_max_speed;
  unsigned char limit_hot_spot;
  unsigned char low_speed_limiting;
  unsigned char coolant_temp_limiting;
  unsigned char limit_stall_burst_model;
};
struct FaultCodes {
  unsigned short post_fault_lo;
  unsigned short post_fault_hi;
  unsigned short run_fault_lo;
  unsigned short run_fault_hi;
};
struct TorqueAndTimerInfo {
  unsigned short commanded_torque;
  unsigned short torque_feedback;
  unsigned long power_on_timer;
};

struct StatusData {
  struct TempratureMessage1 tempratureMessage1;
  struct TempratureMessage2 tempratureMessage2;
  struct TempratureMessage3 tempratureMessage3;
  struct DigitalInputStatus digitalInputStatus;
  struct MotorPositionInfo motorPositionInfo;
  struct CurrentInfo currentInfo;
  struct VoltageInfo voltageInfo;
  struct FluxInfo fluxInfo;
  struct InternalVoltages internalVoltages;
  struct InternalStates internalStates;
  struct FaultCodes faultCodes;
  struct TorqueAndTimerInfo torqueAndTimerInfo;
};

void can_init();
void can_rx(StatusData* statusData);
void can_tx(CANMessage message);

void send_command_message(struct CommandMessage message);

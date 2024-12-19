#include <FlexCAN_T4.h>

// Comment the following line to disable debug information
#define SHOW_DEBUG

// TODO: Set the button voltage threshold to a proper value
#define BUTTON_VOLTAGE_THRESHOLD 100
#define SEND_DELAY    99 // Time in miliseconds
#define CAN_BIT_RATE  500000
// All used pin numbers
#define BRAKE_FRONT   14
#define BRAKE_REAR    15
#define ACCELERATOR_1 16
#define ACCELERATOR_2 17
#define BSPD_CURRENT  18
#define RTD_BUTTON    19
#define SENSOR_7      20
#define SENSOR_8      21
#define SENSOR_16     22
#define SENSOR_18     23
#define SENSOR_17     24
#define SENSOR_9      25
#define SENSOR_15     26
#define SENSOR_14     27
#define SENSOR_10     38
#define SENSOR_11     39
#define SENSOR_12     40
#define SENSOR_13     41
// Message information
#define ACCELERATOR_MESSAGE 1
#define BRAKE_MESSAGE       2
#define BSPD_MESSAGE        3
#define BUTTON_MESSAGE      4

struct can_data {
  // Accelerator message
  uint32_t accelerator_value1;
  uint32_t accelerator_value2;
  // Brake message
  uint32_t brake_value1;
  uint32_t brake_value2;
  // Bspd message
  uint32_t bspd_value;
  // Button message
  uint8_t  start_button_value;
};

// Initialize can_data in the global space
struct can_data data;

// Create a FlexCAN_T4 instance for CAN2
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to be ready to continue

  // Initialize CAN2 with 500 kbps
  can2.begin();
  can2.setBaudRate(500000); // Set bitrate to 500 kbps
#ifdef SHOW_DEBUG
  Serial.println("CAN2 initialized at 500 kbps");
#endif

  // Initialize pins
  pinMode(BRAKE_FRONT,    INPUT);
  pinMode(BRAKE_REAR,     INPUT);
  pinMode(ACCELERATOR_1,  INPUT);
  pinMode(ACCELERATOR_2,  INPUT);
  pinMode(BSPD_CURRENT,   INPUT);
  pinMode(RTD_BUTTON,     INPUT);
  pinMode(SENSOR_7,       INPUT);
  pinMode(SENSOR_8,       INPUT);
  pinMode(SENSOR_16,      INPUT);
  pinMode(SENSOR_18,      INPUT);
  pinMode(SENSOR_17,      INPUT);
  pinMode(SENSOR_9,       INPUT);
  pinMode(SENSOR_15,      INPUT);
  pinMode(SENSOR_14,      INPUT);
  pinMode(SENSOR_10,      INPUT);
  pinMode(SENSOR_11,      INPUT);
  pinMode(SENSOR_12,      INPUT);
  pinMode(SENSOR_13,      INPUT);
}

void loop() {
  // You can change these to constant values for testing
  data.accelerator_value1 = analogRead(ACCELERATOR_1);
  data.accelerator_value2 = analogRead(ACCELERATOR_2);
  data.brake_value1 =       analogRead(BRAKE_FRONT);
  data.brake_value2 =       analogRead(BRAKE_REAR);
  data.bspd_value =         analogRead(BSPD_CURRENT);
  data.start_button_value = analogRead(RTD_BUTTON);
  
  // If needed we can set the rest of the code in this function to be delayed so we don't overflow the can network
  
  CAN_message_t accelerator_message;
  accelerator_message.id = 1;
  accelerator_message.len = 8;
  accelerator_message.buf[0] = data.accelerator_value1 & 0xFF;
  accelerator_message.buf[1] = (data.accelerator_value1 >> 8) & 0xFF;
  accelerator_message.buf[2] = (data.accelerator_value1 >> 16) & 0xFF;
  accelerator_message.buf[3] = (data.accelerator_value1 >> 24) & 0xFF;
  accelerator_message.buf[4] = data.accelerator_value2 & 0xFF;
  accelerator_message.buf[5] = (data.accelerator_value2 >> 8) & 0xFF;
  accelerator_message.buf[6] = (data.accelerator_value2 >> 16) & 0xFF;
  accelerator_message.buf[7] = (data.accelerator_value2 >> 24) & 0xFF;
#ifdef SHOW_DEBUG
  if (can2.write(accelerator_message)) {
    Serial.printf("Accelerator_message sent successfully!\n");
  } else {
    Serial.printf("Accelerator_message failed to send!\n");
  }
#endif
#ifndef SHOW_DEBUG
  can2.write(accelerator_message);
#endif

  CAN_message_t brake_message;
  brake_message.id = 2;
  brake_message.len = 8;
  brake_message.buf[0] = data.brake_value1 & 0xFF;
  brake_message.buf[1] = (data.brake_value1 >> 8) & 0xFF;
  brake_message.buf[2] = (data.brake_value1 >> 16) & 0xFF;
  brake_message.buf[3] = (data.brake_value1 >> 24) & 0xFF;
  brake_message.buf[4] = data.brake_value2 & 0xFF;
  brake_message.buf[5] = (data.brake_value2 >> 8) & 0xFF;
  brake_message.buf[6] = (data.brake_value2 >> 16) & 0xFF;
  brake_message.buf[7] = (data.brake_value2 >> 24) & 0xFF;
#ifdef SHOW_DEBUG
  if (can2.write(brake_message)) {
    Serial.printf("Brake_message sent successfully!\n");
  } else {
    Serial.printf("Brake_message failed to send!\n");
  }
#endif
#ifndef SHOW_DEBUG
  can2.write(brake_message);
#endif

  CAN_message_t bspd_message;
  bspd_message.id = 3;
  bspd_message.len = 8;
  bspd_message.buf[0] = data.bspd_value & 0xFF;
  bspd_message.buf[1] = (data.bspd_value >> 8) & 0xFF;
  bspd_message.buf[2] = (data.bspd_value >> 16) & 0xFF;
  bspd_message.buf[3] = (data.bspd_value >> 24) & 0xFF;
#ifdef SHOW_DEBUG
  if (can2.write(bspd_message)) {
    Serial.printf("Bspd_message sent successfully!\n");
  } else {
    Serial.printf("Bspd_message failed to send!\n");
  }
#endif
#ifndef SHOW_DEBUG
  can2.write(bspd_message);
#endif
  CAN_message_t button_message;
  button_message.id = 4;
  button_message.len = 8;
  if (data.start_button_value > BUTTON_VOLTAGE_THRESHOLD) {
    button_message.buf[0] = 1;
  } else {
    button_message.buf[0] = 0;
  }
#ifdef SHOW_DEBUG
  if (can2.write(button_message)) {
    Serial.printf("Button_message sent successfully!\n");
  } else {
    Serial.printf("Button_message failed to send!\n");
  }
#endif
#ifndef SHOW_DEBUG
  can2.write(button_message);
#endif
}
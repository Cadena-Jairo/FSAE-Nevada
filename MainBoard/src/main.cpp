#include "TaskScheduler.h" // Ignore stdlib.h clang error
#include "status.h"
#include "can.h"

void main_isr();

//#define _TASK_PRIORITY
//#define _TASK_TIMEOUT

const int muxSIG = 23;
const int muxS0 = 9;
const int muxS1 = 8;
const int muxS2 = 7;

// CAN Setup for CAN2
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;

// Task Scheduler
Scheduler ts;

Task tprecharge_IR_check(200, TASK_FOREVER, &precharge_seq);
Task main_task(200, TASK_FOREVER, &main_isr);

void setup() {
  Serial.begin(115200);

  // pinMode(muxSIG, INPUT);
  // pinMode(muxS0, OUTPUT);
  // pinMode(muxS1, OUTPUT);
  // pinMode(muxS2, OUTPUT);

  // Initialize CAN2
  can2.begin();
  can2.setBaudRate(500 * 1000);
  can2.onReceive(canISR);
  // can2.setMaxMB(16);
  can2.enableFIFO();
  can2.enableFIFOInterrupt();
  // can2.mailboxStatus();

  ts.addTask(tprecharge_IR_check);

  tprecharge_IR_check.enable();
}

void loop() {
  ts.execute(); // Execute scheduled tasks
}

void main_isr() {
  switch (get_status()) {
    case STATUS_SHUTDOWN:
      // Car is not running
      // TODO: Check to see if we should turn on the car 
      break;
    case STATUS_RUNNING:
      // Car is running
      // TODO: Run checks
      // TODO: Send commands to the inverter
      break;
    default:
      shutdown_car("Car has reached an unknown status!");
      break;
  }
}


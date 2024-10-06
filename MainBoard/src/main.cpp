#include "core_pins.h"
#include "usb_serial.h"
#ifndef __IMXRT1062__
#error "This program is for teensy4.1 only."
#endif

// #include "../lib/can/can.h"
#ifdef __clang__
#include "../lib/interrupts/interrupts.h"
#endif
#ifndef __clang__
#include <interrupts.h>
#endif
#include <Arduino.h> // Ignore clang error
#include "can/can.h"


/*
 * Interrupt service routines:
 *   Only change global variables here and put your logic elsewhere like the
 * loop function Do not write to serial or put any delays in your ISR.
 */
void button_interrupt() {
  Serial.printf("Pin 1 interrupt triggered!\n");
  // Executes on button press
}
void setup() {
  Serial.begin(115200);
  Serial.printf("Starting...\n");
  can_init();
  // Each entry = {pin_number, mode (INPUT, INPUT_PULLDOWN, INPUT_PULLUP, or
  // INPUT_DISABLE), ISR(type void))}
  DIGITAL_INPUT_PIN_CREATE(){{1, INPUT, button_interrupt}};
  // Sets pinmode for all entries and attaches the ISR (interrupt service
  // routine) to the IDT (interrupt descriptor table)
  DIGITAL_INPUT_PIN_INIT();
}

void loop() {
  struct Message message = can_rx();
  if (  message.length != 0x00) {
    Serial.printf("MessageID: 0x%lx Message: 0x%llx\n", message.message_id, message.data_u64);
  }
}

#ifndef __IMXRT1062__
  #error "This program is for teensy4.1 only."
#endif
#include <Arduino.h>
#include <interrupts.h>
#include <can.h>

#define BUTTON_PIN 1

/*
  * Interrupt service routines:
  *   Only change global variables here and put your logic elsewhere like the loop function
  *   Do not write to serial or put any delays in your ISR.
*/
void button_interrupt() {
  // Executes on button press
}
void setup() {
  // Each entry = {pin_number, mode (INPUT, INPUT_PULLDOWN, INPUT_PULLUP, or INPUT_DISABLE), ISR(type void))}
  DIGITAL_INPUT_PIN_CREATE() { 
    {1, INPUT_PULLUP, button_interrupt}
  };
  // Sets pinmode for all entries and attaches the ISR (interrupt service routine) to the IDT (interrupt descriptor table)
  DIGITAL_INPUT_PIN_INIT();
}

void loop() {
  unsigned long long message = can_rx();
}

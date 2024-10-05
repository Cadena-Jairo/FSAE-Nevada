#pragma once
#define DIGITAL_INPUT_PIN_CREATE()  struct DigitalInputPin digitalInputPins[] = 
#define DIGITAL_INPUT_PIN_INIT()    for (unsigned int i = 0; i < (sizeof(digitalInputPins) / sizeof(digitalInputPins[0])); i++) { pinMode(digitalInputPins[i].pin, digitalInputPins[i].mode); } for (unsigned int i = 0; i < (sizeof(digitalInputPins) / sizeof(digitalInputPins[0])); i++) { attachInterrupt(digitalPinToInterrupt(digitalInputPins[i].pin), digitalInputPins[i].isr, CHANGE); }
struct DigitalInputPin {
  unsigned char pin;    // Pin number
  unsigned char mode;   // INPUT, INPUT_PULLDOWN, INPUT_PULLUP, or INPUT_DISABLE
  void (*isr) ();       // Interrupt service routine (onButtonPress kind of thing)
};

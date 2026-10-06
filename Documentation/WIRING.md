# Wiring / Pin Reference

This document records the pin assignments used by the current Arduino code.

| Component | Arduino Pin | Mode / Note |
|---|---:|---|
| IR Sensor | D5 | INPUT |
| Proximity Sensor | D6 | INPUT_PULLUP |
| Servo Motor signal | D7 | Servo |
| Stepper IN1 | D11 | CheapStepper |
| Stepper IN2 | D10 | CheapStepper |
| Stepper IN3 | D9 | CheapStepper |
| Stepper IN4 | D8 | CheapStepper |
| Buzzer | D12 | OUTPUT |
| Moisture Sensor | A0 | Analog input |

Power and GND connections depend on the exact modules and motors used.
Use an appropriate external motor power supply where required and make sure
the grounds are common when the hardware design requires it.

Do not power a high-current motor directly from an Arduino I/O pin.

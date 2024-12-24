#include "input.h"
#include "Arduino.h"

#define ANALOG_X_PIN A2
#define ANALOG_Y_PIN A4
#define ANALOG_BUTTON_PIN A3
 
//Default values when axis not actioned
#define ANALOG_X_CORRECTION 128
#define ANALOG_Y_CORRECTION 128

byte readAnalogAxisLevel(int pin)
{
  return map(analogRead(pin), 0, 1023, 0, 255);
}
 
bool isAnalogButtonPressed(int pin)
{
  return digitalRead(pin) == 0;
}

InputActions GetInput()
{
  InputActions actions;

  static short lastXValue = 0;
  static short lastYValue = 0;
  static short lastButtonPress = 0;

  short x = readAnalogAxisLevel(ANALOG_X_PIN) - ANALOG_X_CORRECTION;
  short y = readAnalogAxisLevel(ANALOG_Y_PIN) - ANALOG_Y_CORRECTION;
  bool buttonPress = isAnalogButtonPressed(ANALOG_BUTTON_PIN);

  actions.hold = !lastButtonPress && buttonPress;
  actions.rotate = lastYValue >= -100 && y < -100;
  actions.moveDown = y > 100;
  actions.moveLeft = lastXValue <= 100 && x > 100;
  actions.moveRight = lastXValue >= -100 && x < -100;
  
  lastXValue = x;
  lastYValue = y;
  lastButtonPress = buttonPress;

  return actions;
}
void InitializeInput()
{
  pinMode(ANALOG_BUTTON_PIN, INPUT_PULLUP);
}
 
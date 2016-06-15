// PPM_Joystick.ino - Converter of PPM sygnal to USB joystick controls. 
// 
// PPM pulse train:
//   __   ___________________________________   _____   ____   _______   ___ //___   __________   __ HIGH
//     |_|      Sync pulse > 3000 us         |_|     |_|    |_|       |_|   //    |_|          |_|
// Channel pulse 1000 to 2000 us                                                                     LOW
// Pulse start and end on falling edge. The end of one pulse is the begining of next.

// The code uses Jystick library from https://github.com/MHeironimus/ArduinoJoystickLibrary/
#include <Joystick.h>

unsigned long curTime = 0;
unsigned long oldTime = 1;
volatile int pulseLength = 0;
volatile int channel = 0;

// Pulse timing interupt routine.
void ISR1()
{
  curTime = micros();
  pulseLength = curTime - oldTime;  
  oldTime = curTime;  

  if (pulseLength > 3000) {
    channel = 0;
    Joystick.sendState();
  } else {
    channel++;
    setChannel(channel, pulseLength);  
  }  
}

// The function converts length of the pulse 
// to USB joystick control value.
void setChannel(int ch, int pulse)
{
  // Processing pulses with length 800 - 2200 mks. 
  float value = (pulse - 800) / 1400.0;
  
  if (value < 0) 
    value = 0;
  else if (value > 1.0)
    value = 1.0;

  switch(ch) {
    case 2:
    Joystick.setXAxis(value*255 - 127);
    break;

    case 3:
    Joystick.setYAxis(value*255 - 127);
    break;

    case 1:
    Joystick.setZAxis(value*255 - 127);
    break;

    case 4:
    Joystick.setXAxisRotation(value*360);
    break;

    case 5:
    Joystick.setThrottle(value*255);    
    break;

    case 6:
    Joystick.setRudder(value*255);    
    break;    
  }
}

void setup()
{
  Joystick.begin(false);
  attachInterrupt(0, ISR1, FALLING); 
}

void loop()
{
  // Place your debug code here...
  delay(100);
}

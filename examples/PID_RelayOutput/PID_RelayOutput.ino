/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay, let's say in this case for powering a heater to 
 * control temperature.  The PID is designed to Output an analog 
 * value, but the relay can only be On/Off.
 *
 *   To connect them together we use "time proportioning
 * control" -- it's essentially a really slow version of PWM.
 * first we decide on a window size (i.e. 5000mS). We then
 * set the PID to adjust its output between 0 and that window
 * size.  Lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the
 * window being "Relay Off Time".
 ********************************************************/

#include <PID_v1.h>

#define PIN_INPUT 0
#define RELAY_PIN 6

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int WindowSize = 5000; // in milliseconds
unsigned long windowStartTime;

void setup()
{
  windowStartTime = millis();

  //initialize the variables we're linked to
  Setpoint = 100;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  Input = analogRead(PIN_INPUT);
  myPID.Compute();

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  
  // ensure our window doesn't exceed the WindowSize
  if (millis() - windowStartTime > WindowSize) windowStartTime += WindowSize;
  
  // The error value (Output) will be positive if we are below the setpoint
  if (Output > millis() - windowStartTime) digitalWrite(RELAY_PIN, HIGH);
  else digitalWrite(RELAY_PIN, LOW);
}




#if 0
#include "Arduino.h"

#include"GarageHardware.h"

/**
 * Setup the door hardware (all I/O should be configured here)
 *
 * This routine should be called only once from setup()
 */
 const int CLOSING_LED_PIN = D2;
 const int OPENING_LED_PIN = D1;
 const int LIGHT_PIN = D0;

 const int DoorBtn = D3;
 const int OpenSwitch = D4;
 const int ClosingSwitch = D5;
 const int FaultBtn = D6;
 const int testPin = D7;

void setupHardware() {
  // TODO
  pinMode(DoorBtn, INPUT_PULLUP);
  pinMode(OpenSwitch, INPUT_PULLUP);
  pinMode(ClosingSwitch, INPUT_PULLUP);
  pinMode(FaultBtn, INPUT_PULLUP);

  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(CLOSING_LED_PIN, OUTPUT);
  pinMode(OPENING_LED_PIN, OUTPUT);
  pinMode(testPin, OUTPUT);
  digitalWrite(testPin, HIGH);

  Serial.begin(9600);
}

/**
 * Return true if the door open/close button is pressed
 *
 * Note: this is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if buttons is currently pressed, false otherwise
 */
bool isButtonPressed() {
  return !digitalRead(DoorBtn);
}

/**
 * Return true if the door is fully closed
 *
 * Note: This is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if the door is completely closed, false otherwise
 */
bool isDoorFullyClosed() {
  return !digitalRead(ClosingSwitch);
}


/**
 * Return true if the door has experienced a fault
 *
 * Note: This is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if the door is has experienced a fault
 */
bool isFaultActive() {
  return !digitalRead(FaultBtn);
}

/**
 * Return true if the door is fully open
 *
 * Note: This is directly based on hardware.  No debouncing or
 *       other processing is performed.
 *
 * return  true if the door is completely open, false otherwise
 */
bool isDoorFullyOpen() {
  return !digitalRead(OpenSwitch);
}

/**
 * This function will start the motor moving in a direction that opens the door.
 *
 * Note: This is a non-blocking function.  It will return immediately
 *       and the motor will continue to opperate until stopped or reversed.
 *
 * return void
 */
void startMotorOpening() {
    digitalWrite(OPENING_LED_PIN, HIGH);
}

/**
 * This function will start the motor moving in a direction closes the door.
 *
 * Note: This is a non-blocking function.  It will return immediately
 *       and the motor will continue to opperate until stopped or reversed.
 *
 * return void
 */
void startMotorClosing() {
  digitalWrite(CLOSING_LED_PIN, HIGH);
}

/**
 * This function will stop all motor movement.
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */
void stopMotor() {
  digitalWrite(CLOSING_LED_PIN, LOW);
  digitalWrite(OPENING_LED_PIN, LOW);
}

/**
 * This function will control the state of the light on the opener.
 *
 * Parameter: on: true indicates the light should enter the "on" state;
 *                false indicates the light should enter the "off" state
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */

void setLight(boolean on) {
  digitalWrite(LIGHT_PIN, on);
}

/**
 * This function will control the state of the light on the opener.
 *
 * Parameter: cycle (0-100).  0 indicates completely Off, 100 indicates completely on.
 *            intermediate values are the duty cycle (as a percent)
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */
void setLightPWM(int cyclePct) {
  // TODO
      analogWrite(LIGHT_PIN,(int)(cyclePct*2.55));
}

/**
 * This function will send a debugging message.
 *
 * Parameter: message. The message (no more tha 200 bytes)
 *
 * Note: This is a non-blocking function.  It will return immediately.
 *
 * return void
 */
void sendDebug(String message) {
  // TODO
  Serial.println("debug " + message);
}

#endif

#include <MIDIUSB.h>
#include "shiftRegister.h"
#include "serial.h"
#include "midi.h"

extern const bool DEBUG_MODE = false;
volatile long Sys_time = 0;

void setup()
{
	intitializeRegisters();
	delay(2000);

	Serial.begin(38400);
	Serial1.begin(38400);

	pinMode(19, OUTPUT);
	pinMode(20, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(19, HIGH);
	digitalWrite(20, HIGH);

	if(DEBUG_MODE)
		testRegisters();
}

void loop()
{
  	Sys_time = millis();
	checkForMidiUSB();
	checkForSerial();
  	checkForMotor(Sys_time);
}

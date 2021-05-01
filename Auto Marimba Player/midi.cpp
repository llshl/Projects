#include <MIDIUSB.h>
#include "midi.h"
#include "shiftRegister.h"
#include "serial.h"

void decodeMidi(uint8_t header, uint8_t byte1, uint8_t byte2, uint8_t byte3);
extern const bool DEBUG_MODE;



void checkForMidiUSB()
{
	midiEventPacket_t rx; //midi data struct from midiUSB libray 
	do
	{
		rx = MidiUSB.read(); //get queued info from USB
		if(rx.header != 0)
		{
			decodeMidi(rx.header, rx.byte1, rx.byte2, rx.byte3);
		}
	} while(rx.header != 0);
}

void decodeMidi(uint8_t header, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
  
  
	if(DEBUG_MODE)
	{
		String message[] =
		{
		  "-----------",
		  static_cast<String>(header),
		  static_cast<String>(byte1), 
		  static_cast<String>(byte2), //pitch
		  static_cast<String>(byte3), //velocity
		  "-----------"
		};
		sendSerialToUSB(message, 6);
	}

	const uint8_t NOTE_ON_HEADER        = 9;
	const uint8_t NOTE_OFF_HEADER       = 8;
	const uint8_t CONTROL_CHANGE_HEADER = 8;
	const uint8_t SUSTAIN_STATUS_BYTE   = 176;
	const uint8_t MIN_NOTE_PITCH        = 48; //C2
	const uint8_t MAX_NOTE_PITCH        = 84; //C5
  
	switch(header)
	{
	case NOTE_ON_HEADER: //reverse motor controller
	if(byte2 >= MIN_NOTE_PITCH && byte2 <= MAX_NOTE_PITCH && byte3 >= 0 && byte3 <= 127)
	{
    	uint8_t note;
    	note = (byte2 - MIN_NOTE_PITCH) * -2 + 73;    
		/*
       * () *-2 : 최소 노트 값 부터 레지스트 핀 지정, 모터 하나당 2개의 핀이 사용되므로 *-2 
       * 79 = 레지스터 개수*8 -1 (제어핀 수)
		*/
      	activateNote(note, byte3);      //12msec down stroke for full pwm
	}
	break;

	case NOTE_OFF_HEADER:    
	if(byte2 >= MIN_NOTE_PITCH && byte2 <= MAX_NOTE_PITCH )
	{
		Ftime = millis();
		Btime = millis();
		uint8_t note = (byte2 - MIN_NOTE_PITCH) * -2 + 73;
		//100msec Up stroke after 30msec stick move
		Serial.print("Up - note :");
		Serial.print(note+1);
		Serial.print(", BRun Time : ");
		activateNote(note+1, 127);
		while(Btime - Ftime < 100)    
		{
			Btime = millis();
		}
		Ttime = Btime - Ftime;
		Serial.println(Ttime);
		Serial.println();
		activateNote(note+1,0); //stop motor after up 100msec stroke
		//activateNote(note, 0);
	}
	break;
  }
}
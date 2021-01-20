//Latchpin: Pin 10 goes to second register input
//Datapin:  Pin 16 goes to first register input
//Clockpin: Pin 15 goes to third register input
const int ShiftPWM_latchPin=18; //values assigned before includes
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;
#include <ShiftPWM.h>
#include <CShiftPWM.h>
#include "shiftRegister.h"


void CNote::motor_down()
  {
    if(F_motor == false)
    {
    Serial.println();
    F_motor = true;
    Serial.print("down - note : ");
    Serial.print(note);
    Serial.print(", full Runtime : ");
    Pre_Time = millis();
    Run_Time = millis();
    ShiftPWM.SetOne(note+1, 0);
    ShiftPWM.SetOne(note, 255);
    while(Run_Time - Pre_Time <12)
    {
      Run_Time = millis();
    }
    Serial.print(Run_Time - Pre_Time);

    ShiftPWM.SetOne(note, Cvelocity);
    Serial.print(", Velocity : ");
    Serial.print(Cvelocity);
    /*
    while(Run_Time - Pre_Time < 30)
    {
      Run_Time = millis();     
    } 
    Serial.print(", PWM Runtime : ");
    Serial.println(Run_Time - Pre_Time);

    ShiftPWM.SetOne(note, 0);
    */
    F_motor = true;
    T_Time = millis();
    }
  }
void CNote::motor_up(long Timer)
  {
    if(Timer - T_Time > 30 && F_motor == true)
    {
      F_motor = false;
      Serial.print(", PWM Runtime : ");
      Serial.println(Timer - T_Time);
      ShiftPWM.SetOne(note, 0);
      
      ShiftPWM.SetOne(note+1, 127);
      
      Serial.print("Stop&up - note : ");
      Serial.print(note+1);
      
      T_Time = millis();
      B_motor = true;
     }
     if(Timer - T_Time > 30 && B_motor == true)
     {
      Serial.print(", Up Runtime : ");
      Serial.println(Timer - T_Time);
      Serial.println("End Motor");
      ShiftPWM.SetOne(note+1, 0);
      B_motor = false;
     }
   }


CNote Ch_activate[80];


void conformVelocity(uint8_t& velocity);

extern uint8_t pwmPercent = 40;
const char     MAX_PWM = 255;

void intitializeRegisters()
{
	const char    PWM_FREQUENCY = 75;
	const int     NUM_REGISTERS = 10;   //레지스터개수지정
	ShiftPWM.SetAmountOfRegisters(NUM_REGISTERS);
	ShiftPWM.Start(PWM_FREQUENCY, MAX_PWM);
  
}

void activateNote(uint8_t note, uint8_t velocity)
{
	if(velocity > 0)
  {
		conformVelocity(velocity);
    Ch_activate[note].Cnote(note,velocity);
    Ch_activate[note].motor_down();
  }
}

void checkForMotor(long sys_time)
{
  for(int i =0; i<81; i++)
  {
    Ch_activate[i].motor_up(sys_time);
  }
}

void conformVelocity(uint8_t& velocity)
{
  velocity = map(velocity,1,127,50,255);
  /*
	//conforms velocity from 0-127 to 0-255 while taking into account the minimum possible solenoid PWM
	const double MIN_PWM = round(MAX_PWM * pwmPercent / static_cast<double>(100));
	velocity = round(velocity * (MAX_PWM - MIN_PWM) / static_cast<double>(127) + MIN_PWM);
  */
}



void testRegisters()
{
	for(int led = 0; led < 88; led++)
	{
		ShiftPWM.SetOne(led, MAX_PWM);
		delay(50);
		ShiftPWM.SetOne(led, 0);
	}
}

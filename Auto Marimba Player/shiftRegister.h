#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <Arduino.h>

extern uint8_t pwmPercent;

void intitializeRegisters();
void activateNote(uint8_t note, uint8_t velocity);
void testRegisters();
void checkForMotor(long sys_time);

class CNote
{
  public:
  
  uint8_t note;
  uint8_t Cvelocity;
  boolean F_motor = false , B_motor = false;
  unsigned long Run_Time=0, Pre_Time=0; 
  long T_Time=0;
  void Cnote(uint8_t Key, uint8_t vel)
  {
    note = Key;
    Cvelocity = vel;
  }
   
  void motor_down();
  
  void motor_up(long Timer);
  
};

#endif

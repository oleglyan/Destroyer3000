/*CHANGE LOG
2013.03.30
- Time measurement correction:
  "TimeOld=millis();" and "millis()-TimeOld;"
  in states changed to "stateMachine.timeInCurrentState()",
  which was in the FSM library since 2010.03.08
- Mistake with the unsigned long variables:
  in "SpeedControl(){..}"  "StepTimeOldLeft" and "StepTimeOldRight"
  changed to "StepTimeOld"
*/

#include <IRLib.h>
#include "definitions.h"
#include "remote.h"
#include <EEPROM.h>

signed int STOP, STOP2,
           START, START2,
           SpeedSetValueLeft,
           SpeedSetValueRight,
           SpeedCurrentValueLeft,
           SpeedCurrentValueRight;
           
byte sensorSum, Strategy,
     sensorMask=0xFF,
     StateValue,
     CommandValue,
     lastLineSensor,
     lastSideSensor,
     lastFrontSensor;

const byte SpeedStepValue = 5;
float batteryVoltage;
word timer;

unsigned long Time,
              TimeOld,
              StepTimeOld,
              SlewRateTime=54,
              FaultTime;
              
boolean driverState,
        LEDstate,
        overtakeEnable;
        
/*SENSOR WEIGHT CONSTANTS*/
//const byte sharp_sl[] ={1, 0};
//const byte sharp_fl[] = {2, 0};
//const byte sharp_fr[] = {4, 0};
//const byte sharp_sr[] = {8, 0};
//const byte vishay_l[] = {0, 16};
//const byte vishay_r[] = {0, 32};
//const byte microswitch[] = {0, 64, 128, 192};

//byte Inputs []={_FS_L, _FS_R, FB_L, FB_R, RECV, SW, BAT};
//byte PullUps[]={SHARP_SL, SHARP_FL, SHARP_FR, SHARP_SR};
//byte Outputs[]={IN1_L, IN1_R, IN2_L, IN2_R};

IRrecv My_Receiver(RECV);
IRdecode My_Decoder;


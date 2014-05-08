//http://www.arduino.cc/playground/uploads/Code/FSM_1-6.zip
#include <FiniteStateMachine.h>

State powerOnState    = State(powerOnEnter, powerOnUpdate, powerOnExit); //According to RobotChallenge rules
State secondsState    = State(secondsUpdate);
State idleRunState    = State(idleRunUpdate);
State dischargedState = State(dischargedUpdate);
State spinState       = State(spinEnter, spinUpdate, NULL);
State lineAvoidState  = State(lineAvoidEnter, lineAvoidUpdate, NULL);
State attackState     = State(attackEnter, attackUpdate, NULL);
State stoppedState    = State(stoppedSafe,stoppedUpdate, NULL); //According to RobotChallenge rules

FiniteStateMachine stateMachine = FiniteStateMachine(powerOnState);

/*POWER ON STATE*/ 
void powerOnEnter(){
//  Serial.println("PowerOnState");
  digitalWrite(LED, LOW);
}

void powerOnUpdate(){
//  switch(sensorSum){
//    case  1: SetSpeed(-80,   0); break;
//    case  2: SetSpeed(  0, 100); break;
//    case  3: SetSpeed(-70,  70); break;
//    case  4: SetSpeed(100,   0); break;
//    case  8: SetSpeed(  0, -80); break;
//    case 12: SetSpeed( 70, -70); break;
//    case  6: SetSpeed( 60,  60); break;
//  }
//  if(sensorSum>0){
//    LED_FLASHING(2, 200);
//    delay_1000(); 
//  }
  
//  switch(Strategy){
//    case 1: SetSpeed(-100, 100); timer=200; break;
//    case 2: SetSpeed(60, 60); timer=0; break;
//    case 3: SetSpeed(100, -100); timer=200; break;
//    case 4: SetSpeed(0, 100); timer=700; break;
//    case 5: SetSpeed(-100, -100); timer=300; break;
//    case 6: SetSpeed(100, 0); timer=700; break;
//    case 7: SetSpeed(-100,-50); timer=300; break;
//    case 9: SetSpeed(-50,-100); timer=300; break;
//  }
}

void powerOnExit(){
  switch(Strategy){
    case 0: SetSpeed(100, 100); timer=200; break;
    case 1: SetSpeed(-100, 100); timer=400; break;
    case 2: SetSpeed(60, 60); timer=0; break;
    case 3: SetSpeed(100, -100); timer=400; break;
    case 4: SetSpeed(0, 100); timer=1000; break;
    case 5: SetSpeed(-100, -100); timer=400; break;
    case 6: SetSpeed(100, 0); timer=1000; break;
    case 7: SetSpeed(-100,-50); timer=500; break;
    case 9: SetSpeed(-50,-100); timer=500; break;
  }
  TimeOld=millis();
}

void delay_1000(){
  delay(8000);
}
/*5 SECOND DELAY STATE*/
void secondsUpdate(){
  LED_FLASHING(5, 730);
  startCommand();
}

/*STOPPED_SAFE STATE*/
void stoppedSafe(){
//  Serial.println("StoppedState");
  LED_FLASHING(4, 250);
  StateValue = POWERON;
  EEPROM.write(StateAddress, StateValue);
}
/*STOPPED STATE*/
void stoppedUpdate(){
  while(1){
    LED_FLASHING(5, 500);
  }
}

void dischargedUpdate(){
  while(batteryVoltage <= 10.5){
    LED_FLASHING(5, 100);
    batteryVoltage=analogRead(BAT)*15/1024.00;
  }
  DRIVERS_ENABLE();
  ReturnToCurrentState();
}

/*IDLE RUN STATE*/  
void idleRunUpdate(){
//  Serial.println("idleRunUpdate");
  switch(sensorSum){
    //Conditions for transition to a Line Avoid State
    case 16 ... 63:
      stateMachine.transitionTo(lineAvoidState);
      break;
    //Conditions for transition to a Attack State
    case 2:
    case 4:
    case 6:
    case 7:
    case 14:
      stateMachine.transitionTo(attackState);
      break;
    //Conditions for transition to a Spin State
    case 1:
    case 3:
    case 8:
    case 12:
      stateMachine.transitionTo(spinState);
      break;
  }
  if(Strategy > 0 && millis()-TimeOld>timer*8){
    SetSpeed(60, 60);
    Strategy=0;
  }
}

/*LINE AVOID STATE*/
void lineAvoidEnter(){
//  Serial.println("lineAvoidEnter");
  //Trim other sensoric values 
  lastLineSensor = sensorSum & LineBoth; 
  switch(lastLineSensor){                        
    case LineLeft:  //Set timer
                    timer=300;
                    SlewRateTime=0;
                    SetSpeed(-30, -70);
                    break;
    case LineRight: //Set timer
                    timer=300;
                    SlewRateTime=0;
                    SetSpeed(-70, -30);
                    break;
    case LineBoth:  //Set timer for 600ms
                    timer=300;
                    SlewRateTime=0;
                    SetSpeed(-100, -100);
                    break;      
  }
  //Reset timer
  TimeOld=millis();
}
void lineAvoidUpdate(){
//  Serial.println("lineAvoidUpdate");
  switch(lastLineSensor){                        
    case LineLeft:  bitClear(sensorSum, 4); break;
    case LineRight: bitClear(sensorSum, 5); break;
    case LineBoth:  bitClear(sensorSum, 4);
                    bitClear(sensorSum, 5); break;  
  }
//  Time = millis() - TimeOld;
  //Check on other active sensor or timeout
  if(sensorSum > 0 || millis() - TimeOld >= timer*8){
    //If no sensor activated
    //Find out which was the last
    SlewRateTime=54;
    switch(lastLineSensor){
      case LineLeft:  SetSpeed(+80, +40);
                      stateMachine.transitionTo(idleRunState);
                      break;
      case LineRight: SetSpeed(+40, +80);
                      stateMachine.transitionTo(idleRunState);
                      break;
      case LineBoth:  SlewRateTime=0;
                      SetSpeed(-100, 0);
                      timer=150;
                      stateMachine.transitionTo(spinState);
                      break;
    }
    lastLineSensor=0;
  }
}

/*SPIN STATE*/
void spinEnter(){
//  Serial.println("spinEnter");
  //Trim other sensoric values
  lastSideSensor = sensorSum & SideBoth;
  switch(lastSideSensor){
    case SideLeft:  //Set timer for 600ms
                    timer=500;
                    // Set diferential motor speed to 25%
                    SetSpeed(-10, +80); break;
    case SideRight: // Set timer for 600ms
                    timer=500;
                    // Set diferential motor speed to 25%
                    SetSpeed(+80, -10); break; //Set timer for 1000ms and both motor cross reverse speed to 50
  }
  //Reset timer
  TimeOld=millis();
}
void spinUpdate(){
//  Serial.println("spinUpdate");
  switch(lastSideSensor){
    case SideLeft:  bitClear(sensorSum, 0); break;
    case SideRight: bitClear(sensorSum, 3); break;
  }
//  Time = millis() - TimeOld;
  //Check on other active sensor or timeout
  if(sensorSum > 0 || millis() - TimeOld >= timer*8){
    SlewRateTime=54;
    //Transit to Idle Run State to make another decition
    if(sensorSum == 0) SetSpeed(60, 60);
    stateMachine.transitionTo(idleRunState);
    lastSideSensor=0;
  }
}

/*ATTACK STATE*/
void attackEnter(){
//  Serial.println("attackEnter");
  //Trim other sensoric values
  lastFrontSensor = sensorSum & FrontBoth;
  SlewRateTime=54;
  switch(lastFrontSensor){
    case FrontLeft:  SetSpeed( 60, 100); break;
    case FrontRight: SetSpeed(100,  60); break;
    case FrontBoth:  SetSpeed(100, 100); break;
  }
}
void attackUpdate(){
//  Serial.println("attackUpdate");
  //Prevent target lost
  if(sensorSum == 0){
    //Recall the last Front sensor from the memory
    switch(lastFrontSensor){
      case FrontLeft:  SetSpeed( 0, 80); break;
      case FrontRight: SetSpeed(80,  0); break;
      case FrontBoth:  SetSpeed(50, 50); break;
    }
    // Set timer for 1000ms
    timer=500;
    //Transit to Spin State to detect opponent again
    stateMachine.transitionTo(spinState);
    lastFrontSensor=0;
  }
  //Trim sensor values from front sensor values
  sensorSum = sensorSum & 0xF6;
  //Check calibration
  //Check if one of front sensors deactivated
  if(sensorSum != lastFrontSensor){
    //Transit to Idle Run State to make another decition
    stateMachine.transitionTo(idleRunState);
    lastFrontSensor=0;
  }
}

void loop() {
  check_irrecv_signal();
  //Read the battery voltage
  batteryVoltageCheck();
  //Open loop control of slope acceleration
  SpeedControl();
//  sensorSerialPrint();
  //Check fault status flags
  statusFeedback();
  //Read the sensor byte value
  SensorSum();
  //THIS LINE IS CRITICAL
  //do not remove the stateMachine.update() call, it is what makes this program 'tick'
  stateMachine.update();
}

void startCommand(){
//  Serial.println("startCommand");
  StateValue = STARTED;
  EEPROM.write(StateAddress, StateValue);
  DRIVERS_ENABLE();
  digitalWrite(LED, HIGH);
  stateMachine.transitionTo(idleRunState);
}
void startDelayCommand(){
  StateValue=SECONDS;
  EEPROM.write(StateAddress, StateValue);
  stateMachine.transitionTo(secondsState);
}
void stopCommand(){
//  Serial.println("stopCommand");
  motor(0, 0);
  DRIVERS_DISABLE();
  StateValue = STOPPED;
  EEPROM.write(StateAddress, StateValue);
  stateMachine.transitionTo(stoppedState);
}

void programmCommand(){
//  Serial.println("programmCommand");
  motor(0, 0);
  DRIVERS_DISABLE();
  StateValue = POWERON;
  EEPROM.write(StateAddress, StateValue);
  CommandValue = My_Decoder.value - 0x1200;
  EEPROM.write(CommandAddress, CommandValue);
  STOP = CommandValue + 0x1100;
  STOP2 = CommandValue + 0x1900;
  START = STOP + 1;
  START2 = STOP2 +1;
  LED_FLASHING(2, 500);
  stateMachine.transitionTo(powerOnState);
}

/*FAIL STATUS FLAG RESETTER*/
void statusFeedback(){
  if(millis()-FaultTime>200){
    boolean driverStatus=digitalRead(_FS_L)&digitalRead(_FS_R);
    if (driverStatus==LOW && driverState==HIGH){
  //    Serial.print  ("Fail Status: ");
  //    Serial.print  (digitalRead(_FS_L), BIN);
  //    Serial.println(digitalRead(_FS_R), BIN);
  //    Serial.println("DRIVERS_DISABLE");
      DRIVERS_DISABLE();
  //    Serial.println("DRIVERS_ENABLE");
      DRIVERS_ENABLE();
    }  
    FaultTime=millis(); 
  }
}
/*BATTERY VOLTAGE CHECK*/
void batteryVoltageCheck(){
  batteryVoltage=analogRead(BAT)*15/1024.00;
  if(batteryVoltage <= 8){
    DRIVERS_DISABLE();
    stateMachine.immediateTransitionTo(dischargedState);
  }
}
/*SENSOR DATABYTE*/
void SensorSum(){
  bitWrite(sensorSum, 0, !digitalRead(SHARP_SL));
  bitWrite(sensorSum, 1, !digitalRead(SHARP_FL));
  bitWrite(sensorSum, 2, !digitalRead(SHARP_FR));
  bitWrite(sensorSum, 3, !digitalRead(SHARP_SR));
  bitWrite(sensorSum, 4, digitalRead(TCRT_L));
  bitWrite(sensorSum, 5, digitalRead(TCRT_R));
}
/*SWITCH DETECT*/
//byte Switch(){
//  int switchVal=analogRead(SWITCH);
//  if(switchVal==0)                   return 3;
//  if(switchVal>200 && switchVal<300) return 2;
//  if(switchVal>400 && switchVal<600) return 1;
//  if(switchVal>700 && switchVal<800) return 0;
//}

void check_irrecv_signal(){
//  Serial<<"check_irrecv_signal()"<<endl;
  if (My_Receiver.GetResults(&My_Decoder)) {
//    Serial<<"My_Decoder.decode();"<<endl;
    My_Decoder.decode();
//    Serial<<_HEX(My_Decoder.value)<<endl;
    switch(StateValue){
      case POWERON: if(My_Decoder.value == START || My_Decoder.value == START2) startCommand();
                    switch(My_Decoder.value){
                      case RMT_814_FWD:
                      case RC_5_POWER_1:
                      case RC_5_POWER_2: startCommand(); break;
                      case RC_5_1_1:
                      case RC_5_1_2: Strategy=1; break;
                      case RC_5_2_1:
                      case RC_5_2_2: Strategy=2; break;
                      case RC_5_3_1:
                      case RC_5_3_2: Strategy=3; break;
                      case RC_5_4_1:
                      case RC_5_4_2: Strategy=4; break; 
                      case RC_5_5_1:
                      case RC_5_5_2: Strategy=5; break;
                      case RC_5_6_1:
                      case RC_5_6_2: Strategy=6; break;
                      case RC_5_7_1:
                      case RC_5_7_2: Strategy=7; break;
                      case RC_5_9_1:
                      case RC_5_9_2: Strategy=9; break;
                    }
                    remote_stop_program_commands(); break;
      case STARTED: remote_stop_program_commands(); break;
      case STOPPED: break;
    }
    My_Receiver.resume();      //Prepare to receive the next value 
  }
}
void remote_stop_program_commands(){
  if(My_Decoder.value == STOP || My_Decoder.value == STOP2) stopCommand(); //Cannot use in switch because STOP is not a constant
  switch(My_Decoder.value){
    case RMT_814_POWER:
    case RC_5_SLEEP_1:
    case RC_5_SLEEP_2: stopCommand(); break; //My remote's RM_SR200 button STOP as a "stop command"
    case 0x12C4 ... 0x12FE:
    case 0x1AC4 ... 0x1AFE: programmCommand(); break;
  }
}

void LED_FLASHING(int times, int delayed){
  for(int i=0; i<times*2; i++){
    digitalWrite(LED, LEDstate);
    LEDstate=!LEDstate;
    delay(delayed*4);
  }
}

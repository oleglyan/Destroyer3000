void setup() {
//  Serial.begin(19200);
//  Serial.println("Initialisation!");
  pinMode(  LED, OUTPUT);
  //Data Direction Setup for MC33887 control:
  pinMode(_FS_L,  INPUT); //Set Left  Fail    Status   pin as  input
  pinMode(_FS_R,  INPUT); //Set Right Fail    Status   pin as  input
  pinMode(IN1_L, OUTPUT); //Set Left  First   Input    pin as output
  pinMode(IN1_R, OUTPUT); //Set Right First   Input    pin as output
  pinMode(IN2_L, OUTPUT); //Set Left  Second  Input    pin as output
  pinMode(IN2_R, OUTPUT); //Set Right Second  Input    pin as output
  pinMode(   EN, OUTPUT); //Set Both  Enable           pin as output 
  pinMode( FB_L,  INPUT); //Set Left  Current Feedback pin as  input
  pinMode( FB_R,  INPUT); //Set Right Current Feedback pin as  input
  //Data Direction Setup for Sensors:
  pinMode(SHARP_SL,  INPUT_PULLUP); //Set SHARP 340K of the Left  Side  pin as input
  pinMode(SHARP_FL,  INPUT_PULLUP); //Set SHARP 340K of the Front Left  pin as input
  pinMode(SHARP_FR,  INPUT_PULLUP); //Set SHARP 340K of the Front Right pin as input
  pinMode(SHARP_SR,  INPUT_PULLUP); //Set SHARP 340K of the Right Side  pin as input
  pinMode(  TCRT_L,  INPUT_PULLUP); //Set Left  TCRT1000 pin as input
  pinMode(  TCRT_R,  INPUT_PULLUP); //Set Right TCRT1000 pin as input
  pinMode(    RECV,  INPUT); //Set IR receiver signal pin as input
  pinMode(  SWITCH,  INPUT); //Set Two microswitches with R-2R ladder output pin as input
  pinMode(     BAT,  INPUT); //Set battery reference as input (V_bat/3/V_ref*1024)
  
  setPwmFrequency(5, 8);            //Set Pulse Width Modulation Frequency to 16MHz/256/8=7812.5Hz on pins 5 and 6
  My_Receiver.enableIRIn();             //Start the IR receiver
  RestoreCommandStateValues();
  StateValue = EEPROM.read(StateAddress);
//  Serial.println("Initialisation finished!");
  ReturnToCurrentState();
}
void RestoreCommandStateValues(){
//  Serial<<"RestoreCommandStateValues();"<<endl;
  CommandValue = EEPROM.read(CommandAddress);
//  Serial << "CommandValue = EEPROM.read(CommandAddress) ->" << _HEX(CommandValue) << endl;
  switch(CommandValue){
    case UnderMinimumCommandValue: //Serial << "Command Value is Under Minimum" << endl;
    case OverMaximumCommandValue: //Serial << "Command Value is Over Maximum" << endl;
      CommandValue = MinimumCommandValue;
//      Serial << "CommandValue = MinimumCommandValue; ->" << _HEX(CommandValue) << endl;
      EEPROM.write(CommandAddress, CommandValue);
//      Serial << "EEPROM.write(CommandAddress, CommandValue)" << endl;
      break;
  }  
  STOP = CommandValue + 0x1100;
  STOP2 = CommandValue + 0x1900;
  //  Serial << "STOP = CommandValue + 0x100; ->" << _HEX(STOP) << endl;
  START = STOP + 1;
  START2 = STOP2 + 1;
//  Serial << "START = STOP + 1; ->" << _HEX(START) << endl;
  StateValue = EEPROM.read(StateAddress);
//  Serial << "StateValue = EEPROM.read(StateAddress) ->" << StateValue << endl;
  if(StateValue > NumOfStates){
    StateValue = POWERON;
    EEPROM.write(StateAddress, StateValue);
  }
}

void ReturnToCurrentState(){
  switch(StateValue){
    case POWERON: break;
    case STARTED: startCommand(); break;
    case STOPPED: stopCommand(); break;
    case SECONDS: startDelayCommand(); break;
  }
}

/*MOTOR CONTROL*/
void SetSpeed(signed int SpeedLeft, signed int SpeedRight){
  SpeedSetValueLeft = SpeedLeft;
  SpeedSetValueRight = SpeedRight;
}
void SpeedControl(){
  if(SpeedSetValueLeft != SpeedCurrentValueLeft || SpeedSetValueRight != SpeedCurrentValueRight){
    if(millis()-StepTimeOld>= SlewRateTime){
      StepTimeOld=millis();
      if(SpeedSetValueLeft > SpeedCurrentValueLeft) SpeedCurrentValueLeft += SpeedStepValue;
      if(SpeedSetValueLeft < SpeedCurrentValueLeft) SpeedCurrentValueLeft -= SpeedStepValue;
      if(SpeedSetValueRight > SpeedCurrentValueRight) SpeedCurrentValueRight += SpeedStepValue;
      if(SpeedSetValueRight < SpeedCurrentValueRight) SpeedCurrentValueRight -= SpeedStepValue;
      motor(SpeedCurrentValueLeft, SpeedCurrentValueRight);
    }
  }
}

void motor(signed int SpeedLeft, signed int SpeedRight){
  //Constrain speed values
  SpeedLeft=constrain(SpeedLeft, -100, 100);
  SpeedRight=constrain(SpeedRight, -100, 100);
  //Direction control
  if(SpeedLeft >= 0){
    //Clear OC0B on Compare Match, set OC0B at BOTTOM, (non-inverting mode)
    TCCR0A = TCCR0A & 0b11101111;
    digitalWrite(IN2_L,   LOW);
  }else{
    //Convert negative to positive value
    SpeedLeft=-SpeedLeft;
    //Set OC0B on Compare Match, clear OC0B at BOTTOM, (inverting mode).
    TCCR0A = TCCR0A | 0b00110000; 
    digitalWrite(IN2_L, HIGH);
  }
  if(SpeedRight >= 0){
    //Clear OC0A on Compare Match, set OC0B at BOTTOM, (non-inverting mode)
    TCCR0A = TCCR0A & 0b10111111;
    digitalWrite(IN2_R,   LOW);
  }else{
    //Convert negative to positive value
    SpeedRight=-SpeedRight;
    //Set OC0B on Compare Match, clear OC0A at BOTTOM, (inverting mode)
    TCCR0A = TCCR0A | 0b11000000;
    digitalWrite(IN2_R, HIGH);
  }
  //Mapping and correction of speed values
  if(SpeedLeft>0) SpeedLeft=map(SpeedLeft, 1, 100, 40, 235);
  if(SpeedRight>0) SpeedRight=map(SpeedRight, 1, 100, 40, 254);
  //Output
  analogWrite(IN1_L, SpeedLeft);
  analogWrite(IN1_R, SpeedRight);
}

void sensorSerialPrint(){
  Serial.print  ("Sharp 340K: ");
  Serial.print  (digitalRead(SHARP_SL), BIN);
  Serial.print  (digitalRead(SHARP_FL), BIN);
  Serial.print  (digitalRead(SHARP_FR), BIN);
  Serial.print  (digitalRead(SHARP_SR), BIN);
  Serial.print  ("; Switch1: "+String(analogRead(SWITCH)));
  Serial.print  ("; TCRT_L: "+String(digitalRead(TCRT_L))+','+String(analogRead(TCRT_L)));
  Serial.print  ("; TCRT_R: "+String(digitalRead(TCRT_R))+','+String(analogRead(TCRT_R)));
  Serial.print  ("; Battery Voltage = ");
  Serial.print  (batteryVoltage);
  Serial.print  ("; FB_L: "+String(analogRead(FB_L)));
  Serial.print  ("; FB_R: "+String(analogRead(FB_R)));
  Serial.println("; Sensor Sum: "+String(sensorSum));
  
//  Serial.println("{FB_L,T," + String(analogRead(FB_L))+'}');
//  Serial.println("{FB_R,T," + String(analogRead(FB_R))+'}');
//  Serial.print("{Battery Voltage,T,");
//  Serial.print(batteryVoltage);
//  Serial.println('}');
}


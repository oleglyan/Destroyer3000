//IR control PINs
#define RECV 9 //Infrared receiver signal pin

//Freescale Semiconductor DC Motor driver MC33887 Control pins
#define _FS_L 2  //Left  Fail Status pin
#define _FS_R 3  //Right Fail Status pin
#define IN1_L 5  //Left  Input 1 pin
#define IN1_R 6  //Right Input 1 pin
#define IN2_L 4  //Left  Input 2 pin
#define IN2_R 7  //Right Input 2 pin
#define EN    8  //MC33887 motor driver Enable pin
#define FB_L  A0 //Left  Current Feedback pin
#define FB_R  A1 //Right Current Feedback pin

//Vishay Telefunken TCRT1000 Sensor pins
#define SWITCH A2 //Two microswitches with R-2R ladder
#define TCRT_L A4 //Left  TCRT1000 pin
#define TCRT_R A3 //Right TCRT1000 pin
#define BAT    A6 //Battery reference
#define LED    A5 //Start Module Status LED

//Sharp 340K Sensor pins
#define SHARP_SL 10 //SHARP 340K of the Left  Side  pin 
#define SHARP_FL 11 //SHARP 340K of the Front Left  pin
#define SHARP_FR 12 //SHARP 340K of the Front Right pin
#define SHARP_SR 13 //SHARP 340K of the Right Side  pin

//Last Side Sensor
#define SideLeft  1
#define SideRight 8
#define SideBoth  9

//Last Line Sensor
#define LineLeft  16
#define LineRight 32
#define LineBoth  48

//Last Front Sensor
#define FrontLeft  2
#define FrontRight 4
#define FrontBoth  6

//5 second delay strategy code
#define START_5S_DELAY 0x2FE

//EEPROM ADDRESS
#define StateAddress   0
#define CommandAddress 1

#define NumOfStates 3

//LIMIT COMMAND VALUES
#define MinimumCommandValue 0xC4
#define MaximumCommandValue 0xFE
#define UnderMinimumCommandValue 0x00 ... 0xC3
#define OverMaximumCommandValue 0xFF

//EEPROM StateValues
#define POWERON 0
#define STARTED 1
#define STOPPED 2
#define SECONDS 3

//DRIVERS CMD
#define DRIVERS_ENABLE()  driverState=HIGH; digitalWrite(EN, driverState);
#define DRIVERS_DISABLE() driverState=LOW; digitalWrite(EN, driverState);

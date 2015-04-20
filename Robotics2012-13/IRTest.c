#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorNormal, openLoop, reversed)
#pragma config(Sensor, S2,     HTIRS2,         sensorI2CCustom)
#pragma config(Sensor, S3,     HTIRS3,         sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*--------------------------------------------------------------------------------------------------------*\
|*                                                                                                        *|
|*                                    - Tetrix Simple Tank Drive -                                        *|
|*                                          ROBOTC on Tetrix                                              *|
|*                                                                                                        *|
|*  This program allows you to drive a robot via remote control using the ROBOTC Debugger.                *|
|*  This particular method uses "Tank Drive" where each side is controlled individually like a tank.      *|
|*                                                                                                        *|
|*                                        ROBOT CONFIGURATION                                             *|
|*    NOTES:                                                                                              *|
|*                                                                                                        *|
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port D                  motorD              12V                 Right motor                         *|
|*    Port E                  motorE              12V                 Left motor                          *|
\*---------------------------------------------------------------------------------------------------4246-*/

#include "/drivers/hitechnic-irseeker-v2.h"

#include "JoystickDriver.c"
void displayText(int nLineNumber, const string cChar, int nValueDC, int nValueAC);

/*
#define leftZ
#define rightZ 0
#define yValue 0
#define eMotorSpeed 0
*/
 int _dirDC = 0;
  int _dirAC = 0;
	//int dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
	//int acS1, acS2, acS3, acS4, acS5 = 0;
  int acS3 = 0;
	int irdc[5];
	int irac[5];
void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

  return;
}


task main()
{
  initializeRobot();
  //waitForStart();   // wait for start of tele-op phase

	int leftZ = 0;
	int rightZ = 0;
	int yValue = 0;
	bool yIsPositive = true;

  while(true)                            // Infinite loop:
  {
    getJoystickSettings(joystick);

   yValue = -joystick.joy1_y1;         // Motor D is assigned a power level equal to the right analog stick's Y-axis reading.
//	 nxtDisplayTextLine(1, joystick.joy1_y1);
 //   motor[motorD] = joystick.joy1_y1;
//    motor[motorE] = joystick.joy1_y1;// Motor E is assigned a power level equal to the left analog stick's Y-axis reading./*

   _dirDC = HTIRS2readDCDir(HTIRS2);
      if (_dirDC < 0)
        break; // I2C read error occurred

      // read the current modulated signal direction
      _dirAC = HTIRS2readACDir(HTIRS2);
      if (_dirAC < 0)
        break; // I2C read error occurred

      // Read the individual signal strengths of the internal sensors
      // Do this for both unmodulated (DC) and modulated signals (AC)
      if (!HTIRS2readAllDCStrength(HTIRS2, irdc[0], irdc[1], irdc[2], irdc[3], irdc[4]))
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS2, irac[0], irac[1], irac[2], irac[3], irac[4]))
        break; // I2C read error occurred

           nxtDisplayTextLine(6,"value %i", _dirAC);
           /*
      displayText(1, "D", _dirDC, _dirAC);
      displayText(2, "0", irdc[0], irac[0]);
      displayText(3, "1", irdc[1], irac[1]);
      displayText(4, "2", irdc[2], irac[2]);
      displayText(5, "3", irdc[3], irac[3]);
      displayText(6, "4", irdc[4], irac[4]);
      */

    leftZ = -joystick.joy1_x2;
//	 rightZ = -leftZ;
	if(yValue<0)
	{
		yIsPositive = false;
	}

if(yIsPositive)
{
		if(leftZ<0)
		{
			motor[motorD] = yValue+leftZ+2-5;
			motor[motorE] = yValue;
		}
		else if(leftZ>0)
		{
			motor[motorD] = yValue;
			motor[motorE] = yValue+leftZ-5;
		}
		else
		{
			motor[motorD] = yValue;
			motor[motorE] = yValue;
		}
}
else
{
	if(leftZ<0)
	{
		motor[motorD] = yValue+leftZ-2+5;
		motor[motorE] = yValue;
	}
	else if(leftZ>0)
	{
		motor[motorD] = yValue;
		motor[motorE] = yValue-leftZ+5;
	}
	else
	{
		motor[motorD] = yValue;
		motor[motorE] = yValue;
	}
}


}
}
void displayText(int nLineNumber, const string cChar, int nValueDC, int nValueAC)
{
  string sTemp;

  StringFormat(sTemp, "%4d  %4d", nValueDC, nValueAC);
  // Check if the new line is the same as the previous one
  // Only update screen if it's different.
//  if (sTemp != sTextLines[nLineNumber])
  {
    string sTemp2;

 //   sTextLines[nLineNumber] = sTemp;
    StringFormat(sTemp2, "%s:  %s", cChar, sTemp);
    nxtDisplayTextLine(nLineNumber, sTemp2);
  }
}
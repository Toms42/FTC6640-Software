#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S3,     HTIRS2,         sensorI2CCustom)
#pragma config(Sensor, S4,     HTIRS3,         sensorI2CCustom)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     fL,            tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     bL,            tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     fR,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     bR,            tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     brush,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     lift1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_2,     lift2,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    backboard,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    grabber,               tServoNone)
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

#include "../../drivers/hitechnic-irseeker-v2.h"

#include "JoystickDriver.c"
void displayText(int nLineNumber, const string cChar, int nValueDC, int nValueAC);

//bool balanced(int* irac);
void pointTurnDegrees(int speed, int degrees, char direction);
void moveDistanceAtSpeed(int speed, float dist);  // dist in inches
void testDrive();
void turnRight(int x2, int mSec);
void turnLeft(int x1, int mSec);
void shiftRight(int x2, int mSec);
void shiftLeft(int x1, int mSec);
void moveDistanceAtSpeedRL(int RPower, int LPower, float dist);
void crabWalk(int RPower, int LPower, float dist);

#define MAXANGLE 220
#define SERVORATE 3
#define MAXTRIP 70
#define DISTTORAMPENTRY 34
#define DISTTORAMPCENTER 32
#define BACKBOARDUP 250
#define BACKBOARDDOWN 15
#define GRABBERDOWN 100
#define GRABBERUP 140

// place robot 10 inches or 25.4 cm away from bucket, move right to left
// with infrared sensor facing buckets on right side of robot.
//  Trip to far end should be about 200 cm.  Give up 20 cm for start and finish
// so travel 60 inches (150 cm) across field, turn 90 degrees right, move 30 inches (76 cm),
// turn 90 degrees right and move 40 inches (102 cm), stop
  char infraredBearing = '0';
  int  infraredDegrees = 0;
  int _dirDC = 0;
  int _dirDC3 = 0;
  int _dirAC = 0;
  int _dirAC3 = 0;
	//int dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
	//int acS1, acS2, acS3, acS4, acS5 = 0;
  int acS3 = 0;
	int irdc[5];
	int irac[5];
	int irdc3[5];
	int irac3[5];
	int degrees = 90;
	const float wheelBase = 15.75; // front wheelbase in inches
	const float conversion = 0.667;

const float wheelDiameter = 3.875;  // front wheel diameter in inches
const float wheelCircum = 3.14*wheelDiameter;  // front wheel circumference in inches
const int polarity = 1; //originally at 1 for forward
const float ratioWBtoWD = wheelBase/wheelDiameter;  // ratio of wheelbase to wheel diameter

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
//servo[servo1] = 0;
//servo[servo2] = 0;
//servo[servo3] = 0;

  motor[bR] = 0;
  motor[fR] = 0;
  nMotorEncoder[bR] = 0;
  nMotorEncoder[fR] = 0;
  motor[bL] = 0;
  motor[fL] = 0;
  nMotorEncoder[bL] = 0;
  nMotorEncoder[fL] = 0;

 	nMotorEncoder[lift2] = 0;
  servo[backboard] = BACKBOARDDOWN;  // start with position low and in back
	servo[grabber] = GRABBERUP;  // start with position up

	motor[brush] = 0;
	motor[lift2] = 0;
	motor[lift1] = 0;

  wait1Msec(500);
  return;
}

task infraredVision()
{

	while (true)
	{
		 _dirDC = HTIRS2readDCDir(HTIRS2);
     _dirDC3 = HTIRS2readDCDir(HTIRS3);

      if ((_dirDC < 0) || (_dirDC3 < 0))
        break; // I2C read error occurred

      // read the current modulated signal direction
      _dirAC = HTIRS2readACDir(HTIRS2);
      _dirAC3 = HTIRS2readACDir(HTIRS3);
      if ((_dirAC < 0) || (_dirAC3 < 0))
        break; // I2C read error occurred


        if ((_dirDC <= 4)  && (_dirDC3 <= 4))  // true turn left
        {
        	//turnLeft(40, 1000);
           infraredBearing = 'L';
           infraredDegrees = abs(((_dirDC + _dirDC3)/2)-5) * 10;
        }
        else
        {
        		if ((_dirDC >= 6)  && (_dirDC3 >= 6))  // true turn right
        	  {
        	  //	turnRight(40, 1000);
              infraredBearing = 'R';
              infraredDegrees = abs(((_dirDC + _dirDC3)/2)-5) * 10;
        	  }
        	  else
        	  {
        	    if ((_dirDC >= 8)  && (_dirDC3 <= 2))  // delivery zone stop
        	    {

  	        	   // moveDistanceAtSpeed( 0,0.0);
               infraredBearing = 'S';  // indicate stop
               infraredDegrees = 0;

        	    }
        	    else
        	    {
		        	  	if ((_dirDC >= 5)  && (_dirDC3 <= 5))  //  move forward
		              {
		        	     //  moveDistanceAtSpeedRL( 50-7*abs(_dirAC - 5),50 - 7*abs(_dirAC3 - 5),5.0);
		                infraredBearing = 'O';  // indicate forward
                    infraredDegrees = 0;
		        	    }
		        	    else
		        	    {
		         	      	if ((_dirDC <= 3)  && (_dirDC3 >= 7))  //  emmittor is behind robot
		                  {
		        //	           pointTurnDegrees(30,45,'r');
	                         infraredBearing = 'R';
                           infraredDegrees = 45;
		        	        }
		        	        else
		        	        {
		        //	           moveDistanceAtSpeed( 15,5.0);
		                      infraredBearing = 'O';  // indicate forward
                          infraredDegrees = 0;
		        	        }
		        	     }
		        	  }
		        	}


        }  // end of the big IF

        wait1Msec(250);

}  // end of while true

}

task main()
{
//  waitForStart();   // wait for start of competition
  initializeRobot();
  StartTask(infraredVision);

 // shiftLeft(20, 30000);

  motor[bR] = 0;
  motor[fR] = 0;
  nMotorEncoder[bR] = 0;
  nMotorEncoder[fR] = 0;
  motor[bL] = 0;
  motor[fL] = 0;
  nMotorEncoder[bL] = 0;
  nMotorEncoder[fL] = 0;

  bool done = false;

	int distanceToWall = MAXTRIP;
//	crabWalk(30, 30, distanceToWall);
	moveDistanceAtSpeedRL(-30, -30, distanceToWall);
//   testDrive();
  while(true && !done)                            // Infinite loop:
  {

      if (infraredBearing == 'O')
      {
 //     	moveDistanceAtSpeed(30, 10.0);
      	motor[fL] = 30;
      	motor[fR] = 30;
      	motor[bL] = 30;
      	motor[bR] = 30;
      }
      else if (infraredBearing == 'R')
      {
   //   	pointTurnDegrees(30, infraredDegrees , 'r');
        motor[fL] = 35;
      	motor[fR] = 20;
      	motor[bL] = 35;
      	motor[bR] = 20;
      }
      else if (infraredBearing == 'L')
      {
//      	pointTurnDegrees(30, infraredDegrees , 'l');
      	motor[fL] = 20;
      	motor[fR] = 35;
      	motor[bL] = 20;
      	motor[bR] = 35;
      }
      else if (infraredBearing == 'S')
      {
      	// stop
      //deliver payload
      	motor[fL] = 0;
      	motor[fR] = 0;
      	motor[bL] = 0;
      	motor[bR] = 0;
       done = true;
      }
 //   getJoystickSettings(joystick);


//      motor[motorD] = 30;
//      motor[motorE] = 30;

   //   motor[motorF]=10;
//   yValue = -joystick.joy1_y1;         // Motor D is assigned a power level equal to the right analog stick's Y-axis reading.
//	 nxtDisplayTextLine(1, joystick.joy1_y1);
 //   motor[motorD] = joystick.joy1_y1;
//    motor[motorE] = joystick.joy1_y1;// Motor E is assigned a power level equal to the left analog stick's Y-axis reading./*

/* comment out due to new infrared task


   _dirDC = HTIRS2readDCDir(HTIRS2);
   _dirDC3 = HTIRS2readDCDir(HTIRS3);

      if ((_dirDC < 0) || (_dirDC3 < 0))
        break; // I2C read error occurred

      // read the current modulated signal direction
      _dirAC = HTIRS2readACDir(HTIRS2);
      _dirAC3 = HTIRS2readACDir(HTIRS3);
      if ((_dirAC < 0) || (_dirAC3 < 0))
        break; // I2C read error occurred




      // Read the individual signal strengths of the internal sensors
      // Do this for both unmodulated (DC) and modulated signals (AC)
      if (!HTIRS2readAllDCStrength(HTIRS2, irdc[0], irdc[1], irdc[2], irdc[3], irdc[4]))
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS2, irac[0], irac[1], irac[2], irac[3], irac[4]))
        break; // I2C read error occurred
      if (!HTIRS2readAllDCStrength(HTIRS3, irdc3[0], irdc3[1], irdc3[2], irdc3[3], irdc3[4]))
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS3, irac3[0], irac3[1], irac3[2], irac3[3], irac3[4]))
         break;
   //     if ((_dirAC==5) && (irac[2] > 30) && (irac[2]>2*irac[1]) && (irac[2]>2*irac[3]))
   //       if ( (irac[2] > 30) && (irac[2]>3*irac[1]) && (irac[2]>3*irac[3])&& ( _dirDC == 5))

         // _dirDC is left eye, _dirDC3 is the right eye

        if ((_dirDC <= 4)  && (_dirDC3 <= 4))  // true turn left
        {
        	//turnLeft(40, 1000);
           pointTurnDegrees(20, 10, 'l');
        }
        else
        {
        		if ((_dirDC >= 6)  && (_dirDC3 >= 6))  // true turn right
        	  {
        	  //	turnRight(40, 1000);
        	     pointTurnDegrees(20,10,'r');
        	  }
        	  else
        	  {
        	    if ((_dirDC >= 8)  && (_dirDC3 <= 2))  // delivery zone stop
        	    {

  	        	    moveDistanceAtSpeed( 0,0.0);
        	    }
        	    else
        	    {
		        	  	if ((_dirDC >= 5)  && (_dirDC3 <= 5))  //  move forward
		              {
		        	       moveDistanceAtSpeedRL( 50-7*abs(_dirAC - 5),50 - 7*abs(_dirAC3 - 5),5.0);
		        	    }
		        	    else
		        	    {
		         	      	if ((_dirDC <= 3)  && (_dirDC3 >= 7))  //  emmittor is behind robot
		                  {
		        	           pointTurnDegrees(30,45,'r');
		        	        }
		        	        else
		        	        {
		        	           moveDistanceAtSpeed( 15,5.0);
		        	        }
		        	     }
		        	  }
		        	}


        }    end of big IF ELSE  */

  /*
        	moveDistanceAtSpeed(50,100);
        	wait1Msec(10000);
  //      	pointTurnDegrees(30,  degrees, 'r');
      	  moveDistanceAtSpeed(40,DISTTORAMPENTRY);
      	  wait1Msec(200);
  //    	  pointTurnDegrees(30,  degrees, 'r');
      	  wait1Msec(200);
      	  moveDistanceAtSpeed(40,DISTTORAMPCENTER);
  //       	motor[motorD] = 0;
   //    	motor[motorE] = 0;
          done = true;
        }  // end if after find beacon

          	moveDistanceAtSpeed(50,36);
        	wait1Msec(10000);
  //      	pointTurnDegrees(30,  degrees, 'r');
      	  moveDistanceAtSpeed(40,24);
      	  wait1Msec(10000);
  //    	  pointTurnDegrees(30,  degrees, 'r');
      	  wait1Msec(200);
      	  moveDistanceAtSpeed(40,50);
  //       	motor[motorD] = 0;
   //    	motor[motorE] = 0;
    	  wait1Msec(10000);

           nxtDisplayTextLine(7,"value  dirAC %i", _dirAC);
            nxtDisplayTextLine(6,"value  dirAC3 %i", _dirAC3);
            nxtDisplayTextLine(5,"value  dirDC %i", _dirDC);
            nxtDisplayTextLine(4,"value  dirDC3 %i", _dirDC3);

      displayText(1, "D", _dirDC, _dirAC);
      displayText(2, "0", irdc[0], irac[0]);
      displayText(3, "1", irdc[1], irac[1]);
      displayText(4, "2", irdc[2], irac[2]);
      displayText(5, "3", irdc[3], irac[3]);
      displayText(6, "4", irdc[4], irac[4]);
      */

  }   //end while
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


void pointTurnDegrees(int speed, int degrees, char direction)
// A point turn--both wheels move
  {
    if (direction == 'r')
    {
      nMotorEncoder[motorE]=0;
      while((nMotorEncoder[motorE]) < 4.0*degrees*ratioWBtoWD)  // motors are smaller than wheels
      {
          motor[motorD] = -polarity*speed;
          motor[motorE] = (polarity*speed);
      }
      motor[motorE] = 0;
      motor[motorD] = 0;
    }
    else
    {
      nMotorEncoder[motorD]=0;
      while((nMotorEncoder[motorD]) < 4.0*degrees*ratioWBtoWD)
      {
          motor[motorE] = -polarity*speed;
          motor[motorD] = (polarity*speed);
      }
      motor[motorD] = 0;
      motor[motorE] = 0;
    }
}

/*
   	int y1=joystick.joy1_y1;
		int x1=joystick.joy1_x1;
		int x2=joystick.joy1_x2;
	//EXECUTE CONTROL
		motor[fL]=y1+(x1)/2+(x2)/2;	//set front left speed
		motor[bL]=y1-(x1)/2+(x2)/2;	//set back left speed
		motor[fR]=-(-y1+(x1)/2+(x2)/2);	//set front right speed
		motor[bR]=-(-y1-(x1)/2+(x2)/2);	//set back right speed


*/
void moveDistanceAtSpeed(int speed, float dist)  // dist in inches, speed is power
{

	    nMotorEncoder[fL]=0;
      while((abs(nMotorEncoder[fL])) < (1440.0*dist/wheelCircum))
      {
    motor[fL]=speed;	//set front left speed
		motor[bL]=speed;	//set back left speed
		motor[fR]=(speed);	//set front right speed
		motor[bR]=(speed);	//set back right speed


      }


    motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;	//set back right speed

 }

 void testDrive() //  test drive
 {

 pointTurnDegrees(30,  30+degrees, 'l');

 wait1Msec(2000);
 pointTurnDegrees(30,  30+degrees, 'r');

/*
drive forward: 2 circumference (19 in)
turn right : 90 degrees
move forward : 25 inches
turn left: 90 degrees
move forward 20 inches
turn left: 120 degrees
move forward 12 inches
turn right : 60 degrees
move forward : 20 inches
*/

  moveDistanceAtSpeed(15,20.);
 /*
   nMotorEncoder[motorD] = 0;
   while (nMotorEncoder[motorD] > polarity*2880)
   {
   motor[motorD] = -10;   // Left Motor=Motor D is run at a power level of 25.
   motor[motorE] = -10;
   }
   motor[motorD] = 0;   // Left Motor=Motor D is run at a power level of 25.
   motor[motorE] = 0;
*/
   pointTurnDegrees(30, 90, 'r');
   moveDistanceAtSpeed(15,20.);


   pointTurnDegrees(30, 90, 'l');
   moveDistanceAtSpeed(20,20.0);

    pointTurnDegrees(30, 120, 'l');
    moveDistanceAtSpeed(20,12.0);

     pointTurnDegrees(30, 60, 'r');
     moveDistanceAtSpeed(40,20.0);
}

void shiftRight(int x2, int mSec)
{
		motor[fL]=(x2)/2;	//set front left speed
		motor[bL]=(x2)/2;	//set back left speed
		motor[fR]=-((x2)/2);	//set front right speed
		motor[bR]=-((x2)/2);	//set back right speed
		wait1Msec(mSec);
		motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;

}

void shiftLeft(int x1, int mSec)
{
		motor[fL]=-(x1)/2;	//set front left speed
		motor[bL]=-(x1)/2;	//set back left speed
		motor[fR]=((x1)/2);	//set front right speed
		motor[bR]= (x1/2);	//set back right speed
		wait1Msec(mSec);
		motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;
}


void turnRight(int x2, int mSec)
{
		motor[fL]=(x2)/2;	//set front left speed
		motor[bL]=(x2)/2;	//set back left speed
		motor[fR]=-((x2)/2);	//set front right speed
		motor[bR]=-((x2)/2);	//set back right speed
		wait1Msec(mSec);
		motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;

}

void turnLeft(int x1, int mSec)
{
		motor[fL]=-(x1)/2;	//set front left speed
		motor[bL]=-(x1)/2;	//set back left speed
		motor[fR]=((x1)/2);	//set front right speed
		motor[bR]= (x1/2);	//set back right speed
		wait1Msec(mSec);
		motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;
}

void moveDistanceAtSpeedRL(int RPower, int LPower, float dist)
// dist in inches, speed is power
{

	    nMotorEncoder[bL]=0;
      while((abs(nMotorEncoder[bL])) < (1440.0*dist/wheelCircum))
      {
    motor[fL]=LPower;	//set front left speed
		motor[bL]=LPower;	//set back left speed
		motor[fR]=(RPower);	//set front right speed
		motor[bR]=(RPower);	//set back right speed


      }


    motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;	//set back right speed

 }

 void crabWalk(int RPower, int LPower, float dist)
// dist in inches, speed is power
{

	    nMotorEncoder[fL]=0;
      while((abs(nMotorEncoder[fL])) < (1440.0*dist/wheelCircum/(.8))*conversion)
      {
    motor[fL]=LPower;	//set front left speed
		motor[bL]=-LPower;	//set back left speed
		motor[fR]=-(RPower);	//set front right speed
		motor[bR]=(RPower);	//set back right speed


      }


    motor[fL]=0;	//set front left speed
		motor[bL]=0;	//set back left speed
		motor[fR]=0;	//set front right speed
		motor[bR]=0;	//set back right speed

 }

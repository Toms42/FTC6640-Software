#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTIRS2,         sensorI2CCustom)
#pragma config(Sensor, S3,     HTIRS3,         sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define MAXSIGNAL 38
#define BASESIGNAL 10
#define PHASE2OFFSET 5
#define MINPOWER 5
#define TESTDRIVE false

#define SECONDTIER 23025

#include "/drivers/hitechnic-irseeker-v2.h"

const float wheelBase = 13.0; // front wheelbase in inches
const float wheelCircum = 9.5;  // front wheel circumference in inches
const float wheelDiameter = 3.03;  // front wheel diameter in inches
const int polarity = -1; //originally at -1 for reverse
const float ratioWBtoWD = wheelBase/wheelDiameter;  // ratio of wheelbase to wheel diameter

bool raiseLift =false;
bool raiseBucket = false;
//int secondTier = SECONDTIER;
/*
each motor encoder unit = 1/4 of a degree of wheel rotation.
So 1440 motor encoder units = 360 degrees
*/
long nInits = 0;
string sTextLines[8];
/*
struct speedAdjust {
	int leftMotor;
	int rightMotor;
} speedMod;
*/
//typedef struct speedAdjust speedMod;

//speedMod speedChange;

int speedTarget = MAXSIGNAL - BASESIGNAL;
int numberOff = 0;
float currentTenPtMovingAvg = 2.0;
float oldTenPtMovingAvg = 1.0;

// function prototypes
void displayText(int nLineNumber, const string cChar, int nValueDC, int nValueAC);
void speedAdjust(int* irses, int* adjuster);
bool balanced(int* irac);
void pointTurnDegrees(int speed, int degrees, char direction);
void moveDistanceAtSpeed(int speed, float dist);  // dist in inches
void testDrive();
void bringItHome();
void depositDoughNut();
void lift(int speed, int target);
void bucketMove(int s5, int s6);


void initializeRobot()
{
	raiseLift = false;
	raiseBucket = true;
	  servoChangeRate[servo5] = 1;
	servo[servo5] = 50;
	servoChangeRate[servo6] = 1;
	servo[servo6] = 0;

}

task liftIt()
{
	lift(20, SECONDTIER);
}

task raiseTheLift()
{
	while (true)
	{
		if (raiseLift)
		{
			lift(20, SECONDTIER);
			raiseLift = false;
		}
  }
}

task raiseTheBucket()
{
	while (true)
	{
		if (raiseBucket)
		{
		bucketMove(50, 115);
		raiseBucket = false;
	  }
//	  lift(20, SECONDTIER);

	}

}

task main()
{
  initializeRobot();

  // StartTask(liftIt)
 // lift(20,SECONDTIER);
 // StartTask(raiseTheLift);
//  raiseLift = true;
  StartTask(raiseTheBucket);
  raiseBucket = true;
  int _dirDC = 0;
  int _dirAC = 0;
	//int dcS1, dcS2, dcS3, dcS4, dcS5 = 0;
	//int acS1, acS2, acS3, acS4, acS5 = 0;
  int acS3 = 0;
	int irdc[5];
	int irac[5];
	int rightPower = speedTarget;
	int leftPower = speedTarget;

	int speedModifier[2];

	// the default DSP mode is 1200 Hz.
  tHTIRS2DSPMode _mode = DSP_1200;
   motor[motorD] = 0;   // Left Motor=MotorD is run at a power level of 25.
   motor[motorE] = 0;   // right motor = MotorE



   wait1Msec(500);

 //  raiseTheRoof(1,2);


 //  moveDistanceAtSpeed(25, 36.0);  // dist in inches

//  waitForStart(); // Wait for the beginning of autonomous phase.

   motor[motorD] = polarity*leftPower;   // Left Motor=Motor D is run at a power level of 25.
   motor[motorE] = polarity*rightPower;   // Right Motor=Motor E is run at a power level of 25.

 //motor tests complete

   acS3 = 0;

   if(TESTDRIVE)
   {
     testDrive();
   }

  wait1Msec(1000);       // The program waits 2000 milliseconds (2 seconds) before running further code
//	liftIt();
//  while(acS3 < MAXSIGNAL/2)
  while(acS3 < MAXSIGNAL/2)
  {
//  nSyncedMotors = synchED;  // illegal
	speedModifier[0] = 0;
	speedModifier[1] = 0;
//	wait1Msec(50);       // The program waits 50 milliseconds (.05 seconds) before runnin
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

      acS3 = irac[2];
      speedTarget = polarity*(MAXSIGNAL-BASESIGNAL);
      oldTenPtMovingAvg = currentTenPtMovingAvg;
      currentTenPtMovingAvg = (9.0*currentTenPtMovingAvg + acS3)/10.0;

      if ((_dirAC > 0) && (_dirAC!=5))
      {
 //       speedModifier[0] = (_dirAC-5)*5;
 //       speedModifier[1] = - speedModifier[0];
          if (_dirAC > 5)
          {
          	numberOff++;

					 }
					 else
					 {
					  numberOff--;
				   }
      }
      else
      {
 //     	speedAdjust(irac, speedModifier);

      }
      if(numberOff >= 6)  // 3 sequential signals off to right
      {
         	  pointTurnDegrees(20,90,'r');
          	moveDistanceAtSpeed(20,20.5);
					  pointTurnDegrees(20,90,'l');
					  numberOff = 0;
			}
			else if (numberOff <= -6)  // 3 sequential signals off to left
			{
						 pointTurnDegrees(20,90,'l');
					   moveDistanceAtSpeed(20,20.5);
					   pointTurnDegrees(20,90,'r');
					   numberOff = 0;
		  }
   //   displayText(1, "L ", speedModifier[0], _dirDC);
     // displayText(2, "R ", speedModifier[1], _dirAC);

        //leftPower =


      	motor[motorD] = speedTarget +polarity*speedModifier[0];


       // rightPower =

    	  motor[motorE] = speedTarget +polarity*speedModifier[1];


 //     wait1Msec(100);       // The program waits 100 milliseconds (.10 seconds) before runnin

      displayText(1, "D", _dirDC, _dirAC);
      displayText(2, "0", irdc[0], irac[0]);
      displayText(3, "1", irdc[1], irac[1]);
      displayText(4, "2", irdc[2], irac[2]);
      displayText(5, "3", irdc[3], irac[3]);
      displayText(6, "4", currentTenPtMovingAvg, oldTenPtMovingAvg);

      nxtDisplayTextLine(7, "phase A Enter to switch");

}  // end of loop for rough approach

numberOff = 0;
int averFlip = 0;
bool flipped = false;

//while((currentTenPtMovingAvg >= oldTenPtMovingAvg))
while(averFlip < 10)
 //while((acS3<MAXSIGNAL)  && (acS2<MAXSIGNAL) && (acS4<MAXSIGNAL))  // first part of end game to close in on infra-red beacon
{
	speedModifier[0] = 0;
	speedModifier[1] = 0;
//  wait1Msec(speedTarget);       // The program waits 50 milliseconds (.05 seconds) before runnin
      _dirDC = HTIRS2readDCDir(HTIRS2);
      if (_dirDC < 0)
        break; // I2C read error occurred

      // read the current modulated signal direction
      _dirAC = HTIRS2readACDir(HTIRS2);
      if (_dirAC < 0)
        break; // I2C read error occurred

      // Read the individual signal strengths of the internal sensors
      // Do this for both unmodulated (DC) and modulated signals (AC)
      if (!HTIRS2readAllDCStrength(HTIRS2, irdc[0], irdc[1], irdc[2], irdc[3], irdc[4] ))
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS2, irac[0], irac[1], irac[2], irac[3], irac[4]))
        break; // I2C read error occurred


      acS3 = irac[2];
//      speedTarget = polarity*min2((MAXSIGNAL-BASESIGNAL),(MAXSIGNAL-acS3));
      speedTarget = polarity*20;
      oldTenPtMovingAvg = currentTenPtMovingAvg;
      currentTenPtMovingAvg = (9.0*currentTenPtMovingAvg + acS3)/10.0;

      if (oldTenPtMovingAvg > currentTenPtMovingAvg)
      {
      	flipped = true;
      	averFlip++;
      }
      else
      {
      	flipped = false;
      	averFlip = 0;
      }

           if ((_dirAC > 0) && (_dirAC!=5))
      {
 //       speedModifier[0] = (_dirAC-5)*5;
 //       speedModifier[1] = - speedModifier[0];
          if (_dirAC > 5)
          {
          	numberOff++;

					 }
					 else
					 {
					  numberOff--;
				   }
      }
      else
      {
 //     	speedAdjust(irac, speedModifier);

      }
      if(numberOff >= 10)  // 3 sequential signals off to right
      {
         	  pointTurnDegrees(20,90,'r');
          	moveDistanceAtSpeed(20,20.5);
					  pointTurnDegrees(20,90,'l');
					  numberOff = 0;
			}
			else if (numberOff <= -10)  // 3 sequential signals off to left
			{
						 pointTurnDegrees(20,90,'l');
					   moveDistanceAtSpeed(20,20.5);
					   pointTurnDegrees(20,90,'r');
					   numberOff = 0;
		  }
   //   displayText(1, "L ", speedModifier[0], _dirDC);
     // displayText(2, "R ", speedModifier[1], _dirAC);

        //leftPower =



      if ((_dirAC > 0) && (_dirAC!=5))
      {
        speedModifier[0] = (_dirAC-5)*3;
        speedModifier[1] = - speedModifier[0];
      }
      else
      {
      	speedAdjust(irac, speedModifier);
      }



 //       displayText(1, "L ", speedModifier[0], _dirDC);
   //     displayText(2, "R ", speedModifier[1], _dirAC);

        //leftPower =
      	motor[motorD] = speedTarget +polarity*speedModifier[0];


       // rightPower =

    	  motor[motorE] = speedTarget +polarity*speedModifier[1];




      displayText(1, "D", _dirDC, _dirAC);
      displayText(2, "0", irdc[0], irac[0]);
      displayText(3, "1", irdc[1], irac[1]);
      displayText(4, "2", irdc[2], irac[2]);
      displayText(5, "3", irdc[3], irac[3]);
      displayText(6, "4", irdc[4], irac[4]);
      nxtDisplayTextLine(7, "Phase B Enter to switch");

}  // end of loop for first part of end game

 // acS3=MAXSIGNAL -PHASE2OFFSET;
  motor[motorD] = 0;
  motor[motorE] = 0;
/*

final part of end game

*/

bool centered = false;
while (!centered)
{
	     //read the current modulated signal direction
      _dirAC = HTIRS2readACDir(HTIRS2);
      if (_dirAC < 0)
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS2, irac[0], irac[1], irac[2], irac[3], irac[4]))
        break; // I2C read error occurred

      acS3 = irac[2];
      speedTarget = polarity*MINPOWER;
      centered = balanced(irac);
      if (!centered)
      {
              speedAdjust(irac, speedModifier);
      }
      else
      {
      	      speedModifier[0]=0;
      	      speedModifier[1]=0;
      }
        //leftPower =
      	motor[motorD] = speedTarget +polarity*speedModifier[0];

       // rightPower =
    	  motor[motorE] = speedTarget +polarity*speedModifier[1];

      displayText(1, "D", _dirDC, _dirAC);
      displayText(2, "0", irdc[0], irac[0]);
      displayText(3, "1", irdc[1], irac[1]);
      displayText(4, "2", irdc[2], irac[2]);
      displayText(5, "3", irdc[3], irac[3]);
      displayText(6, "4", irdc[4], irac[4]);
      nxtDisplayTextLine(7, "phase C Enter to switch");


//
}// end of final end game loop


  motor[motorD] = 0;
  motor[motorE] = 0;

/*
while(acS3<(MAXSIGNAL))
 //while((acS3<MAXSIGNAL)  && (acS2<MAXSIGNAL) && (acS4<MAXSIGNAL))  // first part of end game to close in on infra-red beacon
{
	      _dirAC = HTIRS2readACDir(HTIRS2);
      if (_dirAC < 0)
        break; // I2C read error occurred
      if (!HTIRS2readAllACStrength(HTIRS2, irac[0], irac[1], irac[2], irac[3], irac[4]))
        break; // I2C read error occurred

      acS3 = irac[2];
      speedTarget = polarity*min2((MAXSIGNAL-acS3),MINPOWER);
           //leftPower =
      	motor[motorD] = speedTarget;

       // rightPower =
    	  motor[motorE] = speedTarget;
      displayText(1, "D", _dirDC, _dirAC);
      displayText(2, "0", irdc[0], irac[0]);
      displayText(3, "1", irdc[1], irac[1]);
      displayText(4, "2", irdc[2], irac[2]);
      displayText(5, "3", irdc[3], irac[3]);
      displayText(6, "4", irdc[4], irac[4]);
      nxtDisplayTextLine(7, "phase D Enter to switch");

 }
*/
  motor[motorD] = 0;
  motor[motorE] = 0;

  bringItHome();

  nxtDisplayTextLine(7, "phase E Enter to switch");
  wait10Msec(1000);
}

// Minimize LCD screen flicker by only updating LCD when data has changed
void displayText(int nLineNumber, const string cChar, int nValueDC, int nValueAC)
{
  string sTemp;

  StringFormat(sTemp, "%4d  %4d", nValueDC, nValueAC);
  // Check if the new line is the same as the previous one
  // Only update screen if it's different.
  if (sTemp != sTextLines[nLineNumber])
  {
    string sTemp2;

    sTextLines[nLineNumber] = sTemp;
    StringFormat(sTemp2, "%s:  %s", cChar, sTemp);
    nxtDisplayTextLine(nLineNumber, sTemp2);
  }
}
  void speedAdjust(int* irses, int* adjuster)  // [0] left, [1] right
  {
  	int adj = 0;
  	int max = 0;
  	adjuster [0] = adjuster[1] = 0;

  	for (int n = 0; n < 5; n++)
  	{
  		if (irses[n] > irses[max])
  		{
  			max = n;
  		}
  	}

  	if (max == 2)  // true for center
  	{
  		adj = (irses[3] -irses[1])/2;
  		if (adj > 0)
  		{
  			adjuster[1]= -1;//-1
  			adjuster[0]= 1; //+1
  		}
  			else
  		{
  			adjuster[1] =  1;
  			adjuster[0] = -1;
  		}
  		adjuster[0] = 0; // for now
  		adjuster[1] = 0; // for now
  	}
  	else
    {
    	 adj = (max -2)*2;  // if  adj is pos, left [0] stronger, right [1] weaker
    	                    // if adj is neg, left[0] is weaker, right [1] stronger
  			adjuster[1]=-(adj);
  			adjuster[0]=(adj);

  	}

     //displayText(1, "L", adjuster[0]);
     //displayText(2, "R", adjuster[1]);
  }
bool balanced(int* irac)
{
	bool answer = false;
	int sum = 0;
	  	for (int n = 3; n < 5; n++)
  	{
  		sum = irac[n]-irac[4-n];
  	}
    if (abs(sum) <=1 )
    	answer = true;

	return answer;
}

void pointTurnDegrees(int speed, int degrees, char direction)
// A point turn--both wheels move
  {
    if (direction == 'r')
    {
      nMotorEncoder[motorE]=0;
      while((nMotorEncoder[motorE]) < 4.0*degrees*ratioWBtoWD)  // motors are smaller than wheels
      {
          motor[motorD] = polarity*speed;
          motor[motorE] = -(polarity*speed);
      }
      motor[motorE] = 0;
      motor[motorD] = 0;
    }
    else
    {
      nMotorEncoder[motorD]=0;
      while((nMotorEncoder[motorD]) < 4.0*degrees*ratioWBtoWD)
      {
          motor[motorE] = polarity*speed;
          motor[motorD] = -(polarity*speed);
      }
      motor[motorD] = 0;
      motor[motorE] = 0;
    }
}
void moveDistanceAtSpeed(int speed, float dist)  // dist in inches, speed is power
{

	    nMotorEncoder[motorD]=0;
      while((polarity*nMotorEncoder[motorD]) < 1440.0*dist/wheelCircum)
      {
          motor[motorE] = polarity*speed;
          motor[motorD] = polarity*speed;
      }
     motor[motorE] = 0;
     motor[motorD] = 0;

 }

 void testDrive() //  test drive
 {
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

void bringItHome()
{
	   depositDoughNut();
  // moveDistanceAtSpeed(6, 16.0);

}
void depositDoughNut()
{
//lift(20, SECONDTIER);
 // raiseBucket();
//raiseLift=true;

			servo[servo6] = 0;
			wait1Msec(100);


}

void lift(int speed, int target)
{
	nMotorEncoder[motorH]=0;
	motor[motorH] = -speed;
	while(nMotorEncoder[motorH] > -target)  //  counts decrease as the lift rises
	{
	   	motor[motorH] = -speed;
	}
	motor[motorH] = 0;
}

void bucketMove(int s5, int s6)
{
		servo[servo5] = s5;
		servo[servo6] = s6;
}

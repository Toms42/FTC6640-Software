#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     touch,          sensorTouch)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     fl,            tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     bl,            tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     fr,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     br,            tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     brush,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     lift1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_2,     lift2,         tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S2_C1_1,    backboard,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    grabber,              tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-sensormux.h"
#include "JoystickDriver.c"

//-------------------//
//MULTIPLEXER SENSORS//
//-------------------//

// Assuming the Sensor MUX is connected to NXT sensor port 4 (S4)
// Assuming the following sensors are connected to the Sensor MUX ports:
// Port 1: Touch
// Port 2: IR
// Port 3: IR
// Port 4: Sonar
#include "drivers/lego-touch.h"    //for touch sensor
#include "drivers/hitechnic-irseeker-v2.h"    //for ir sensor
#include "drivers/lego-ultrasound.h"    //for sonar sensor
#define liftLimit                msensor_S4_1
#define irLeft                   msensor_S4_2
#define irRight                  msensor_S4_3
#define Sonar                    msensor_S4_4

//---------------//
//DRIVE CONSTANTS//
//---------------//

#define forwardSpeed 0.4 /*0.5 sideways speed is good. sets forwards speed. (1 = full sensitivity)*/
#define sideSpeed 0.8 /*sets sideways speed. (1 = full sensitivity)*/
#define slowModeThreshold 20 /*sets "slow mode" threshold.(1-128) (when going below this speed in either
															direction DON'T move diagonally. makes driving slowly easier.)*/
#define fastMultiplier 2 /*how much faster to go in fast mode (1 = standard speed.)*/
#define slowMultiplier 0.5 /*how much slower to go in slow mode(1 = standard speed.)*/

#define backboardUp 250
#define backboardDown 15

#define grabberDown 100
#define grabberUp 140

int y1=0;
int x1=0;
int x2=0;
int y2=0;
int joy1y2=0;



//  		-------			//
//  		PROGRAM			//
//	  	-------			//

//SMART DRIVE CALCULATIONS//

float sideFactor = 0.2;
float forwardFactor = 0.2;

//Execute the drive task

task drive()
{
	nSchedulePriority=99;
	while(true)
	{

		y1=joystick.joy1_y1;
		x1=joystick.joy1_x1;

		if(abs(y1)<4)
		{
			y1=0;
		}
		if(abs(x1)<4)
		{
			x1=0;
		}
		if(abs(x2)<4)
		{
			x2=0;
		}
		if(abs(y2)<4)
		{
			y2=0;
		}

		//EXECUTE CONTROL
		motor[fl]=y1*100/127+x1*100/127;
		motor[bl]=y1*100/127+x1*100/127;
		motor[fr]=y1*100/127-x1*100/127;
		motor[br]=y1*100/127-x1*100/127;
		wait1Msec(2);
	}
}

task brushSpin()
{
	while(true)
	{
		if(joy1Btn(5)||joy2Btn(5))
		{
			motor[brush]=20;
		}
		else
			{
			if(joy1Btn(6)||joy2Btn(6))
			{
				motor[brush]=-20;
			}
			else
				{
				if(joy1Btn(7)||joy2Btn(7))
				{
					motor[brush]=70;
				}
				else
					{
					if(joy1Btn(8)||joy2Btn(8))
					{
						motor[brush]=-70;
					}
					else
						{
						motor[brush]=0;
						}
					}
				}
			}
		}
		wait1Msec(2);
	}

task lift()
{
	nSchedulePriority=97;
	while(true)
	{
		//joy1y2=joystick.joy1_y2;
		//if(abs(joy1y2)<5)
		//{
		//	joy1y2=0;
		//}
		//else
		{
			motor[lift1]=joystick.joy1_y2*100/127;
			motor[lift2]=joystick.joy1_y2*100/127;
		}
		wait1Msec(2);
	}
}

task backBoardTask()
{
	while(true)
	{
		if(joy1Btn(3)==1)
		{
			servo[backboard]=backboardUp;
		}
		if(joy1Btn(2)==1)
		{
			servo[backboard]=backboardDown;
		}
		wait1Msec(2);
	}
}

task grabberTask()
{
	while(true)
	{
		if(joy1Btn(1)==1)
		{
			servo[grabber]=grabberDown;
		}
		if(joy1Btn(4)==1)
		{
			servo[grabber]=grabberUp;
		}
		wait1Msec(2);
	}
}

task initialize()
{
	servo[backboard]=backboardDown;
	servo[grabber]=grabberUp;
}

task main()
{
	StartTask(initialize);
	waitForStart();
	StartTask(brushSpin);
	StartTask(drive);
	StartTask(lift);
	StartTask(backBoardTask);
	StartTask(grabberTask);
	while(true)
	{
		getJoystickSettings(joystick);
		wait1Msec(2);
	}
}

#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
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

#include "JoystickDriver.c"



//-------------------//
//MULTIPLEXER SENSORS//
//-------------------//

#include "drivers/hitechnic-sensormux.h"
#include "drivers/hitechnic-irseeker-v2.h"
#include "drivers/hitechnic-gyro.h"
#include "drivers/lego-ultrasound.h"

// Assuming the Sensor MUX is connected to NXT sensor port 4 (S4)
// Assuming the following sensors are connected to the Sensor MUX ports:
// Port 1: irLeft
// Port 2: gyro
// Port 3: irRight
// Port 4: ultrasound

#define IRLeft msensor_S4_1
#define gyro msensor_S4_2
#define IRRight msensor_S4_3
#define ultrasound msensor_S4_4

//---------//
//CONSTANTS//
//---------//

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

		int IRLeftValue = HTIRS2readDCDir(IRLeft);
		int IRRightValue = HTIRS2readDCDir(IRRight);

		float gyroValue = HTGYROreadCal(gyro);
		float gyroValue2 = HTGYROreadRot(gyro);

		int ultrasoundValue = USreadDist(ultrasound);

task displaySensorValues()
{
	while(true)
	{
		IRLeftValue = HTIRS2readDCDir(IRLeft);
		IRRightValue = HTIRS2readDCDir(IRRight);

		gyroValue = HTGYROreadCal(gyro);
		gyroValue2 = HTGYROreadRot(gyro);

		ultrasoundValue = USreadDist(ultrasound);

		eraseDisplay();

		nxtDisplayString(0, "ir1: %d", IRLeftValue);
		nxtDisplayString(2, "gyrocal: %d", gyroValue);
		nxtDisplayString(3, "gyrorot: %d", gyroValue2);
		nxtDisplayString(5, "ir2: %d", IRRightValue);
		nxtDisplayString(7, "ultra: %d", ultrasoundValue);
		Wait1Msec(2);
	}
}

task initialize()
{
	servo[backboard]=backboardDown;
	servo[grabber]=grabberUp;

	HTGYROsetCal(gyro, 0);
}

task main()
{
startTask(initialize);
startTask(displaySensorValues);
while(true)
{
	wait1Msec(2);
}
}

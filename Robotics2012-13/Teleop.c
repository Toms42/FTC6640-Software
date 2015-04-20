#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoStandard)
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

#define firstLevel 0
#define secondLevel 23025
#define thirdLevel 39000
#include "JoystickDriver.c"
/*
#define leftZ
#define rightZ 0
#define yValue 0
#define eMotorSpeed 0
*/


 float totalEncoderCount = -23025;
 float motorEncVal = 0;
 const float autoArray [5] = {0,secondLevel,firstLevel,0,thirdLevel};


 void autoPegs(int number);

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.
  nMotorEncoder[motorH]=0;
  nMotorEncoder[motorD]=0;
  nMotorEncoder[motorE]=0;
  motor[motorH] = 0;
  motor[motorD] = 0;
  motor[motorE] = 0;
  float totalEncoderCount = -23025;  // start of teleop is at end of autonomous
  float motorEncVal = 0;


  return;
}


task main()
{
  initializeRobot();
  waitForStart();   // wait for start of tele-op phase
  nMotorEncoder[motorH]=0;
  nMotorEncoder[motorD]=0;
  nMotorEncoder[motorE]=0;
	int leftZ = 0;
	int rightZ = 0;
	int yValue = 0;
	bool yIsPositive = true;
	bool bucketup = false;
	int bucketVal = 30;
	int armServoVal = 50;

	int armUp = 80;
	int armDown = 80;
	int changeRate = 5;
	//	int motorEncVal = 0;


	servoChangeRate[servo1] = changeRate;
	servoChangeRate[servo2] = changeRate;
	servoChangeRate[servo5] = changeRate;
	servoChangeRate[servo6] = changeRate;

	nMotorEncoder[motorH] = 0;

servo[servo5] = bucketVal;
servo[servo6] = armServoVal;
  while(true)                            // Infinite loop:
  {
    getJoystickSettings(joystick);

    // driving


   yValue = -joystick.joy1_y1;         // Motor D is assigned a power level equal to the right analog stick's Y-axis reading.
//	 nxtDisplayTextLine(1, joystick.joy1_y1);
 //   motor[motorD] = joystick.joy1_y1;
//    motor[motorE] = joystick.joy1_y1;// Motor E is assigned a power level equal to the left analog stick's Y-axis reading./*

    leftZ = joystick.joy1_x2;
//	 rightZ = -leftZ;
	if(yValue<0)
	{
		yIsPositive = false;
	}

if(!yIsPositive)
{
		if(leftZ<0)
		{
			motor[motorD] = (yValue-leftZ+2-5)/1.5;
			motor[motorE] = yValue/1.5;
		}
		else if(leftZ>0)
		{
			motor[motorD] = yValue/1.5;
			motor[motorE] = (yValue+leftZ-5)/1.5;
		}
		else
		{
			motor[motorD] = yValue/2;
			motor[motorE] = yValue/2;
		}
}
else
{
	if(leftZ<0)
	{
		motor[motorD] = (yValue+leftZ-2+5)/1.5;
		motor[motorE] = yValue/1.5;
	}
	else if(leftZ>0)
	{
		motor[motorD] = yValue/1.5;
		motor[motorE] = (yValue-leftZ+5)/1.5;
	}
	else
	{
		motor[motorD] = yValue/2;
		motor[motorE] = yValue/2;
	}
}

//end driving code

/* begin other motor Control settings */

    if(joy1Btn(1))
    {
    	autoPegs(1);
    }
    else if (joy1Btn(2))
    {
    	autoPegs(2);
    }
    else if (joy1Btn(4))
    {
    	autoPegs(4);
    }

    /*********************** second level auto

    if (joy1Btn(1)) // b botton ----- second level
    {

//    	if(abs(nMotorEncoder[motorH])<secondLevel)
     if((totalEncoderCount)> -secondLevel)  //  count goes down as arm goes up
    	{
    		nxtDisplayBigTextLine(0,"OTHER ONE");
    //		int go = secondLevel-nMotorEncoder[motorH];
    //	motorEncVal = motorEncVal+nMotorEncoder[motorH];
    	  nMotorEncoder[motorH] = 0;
    	  bool wereGood = true;
	    	while((nMotorEncoder[motorH]+totalEncoderCount) > -secondLevel &&wereGood)
	    	{
	    		getJoystickSettings(joystick);
	    		if(!joy1Btn(3))
	      		motor[motorH] = -20; //up
	      	else
	      	{
	      		wereGood = false;
	      	}
	      }
      	motor[motorH] = 0;
    	}
    	else if(totalEncoderCount <= -secondLevel)
    	{
    		nxtDisplayBigTextLine(6," I'M INNNNNNNNNN");
        nMotorEncoder[motorH] = 0;
    		while((nMotorEncoder[motorH]+totalEncoderCount) <= -secondLevel)
    		{
    			motor[motorH] = 10; //down
    		}
    		motor[motorH] = 0;
    	}
    	totalEncoderCount += nMotorEncoder[motorH];
    }
    *********************** second level auto */

    /*********************** third level auto

     if (joy1Btn(4)) // y button ----- third level
    {

     if((totalEncoderCount)> -thirdLevel)  //  count goes down as arm goes up
    	{
    		nxtDisplayBigTextLine(0,"OTHER ONE");
    //		int go = secondLevel-nMotorEncoder[motorH];
    //	motorEncVal = motorEncVal+nMotorEncoder[motorH];
    	  nMotorEncoder[motorH] = 0;
	    	while((nMotorEncoder[motorH]+totalEncoderCount) > -thirdLevel)
	    	{
	      	motor[motorH] = -20; //up
	      	motorEncVal = nMotorEncoder[motorH];
	      }
      	motor[motorH] = 0;
    	}
   	else if(totalEncoderCount <= -thirdLevel)
    	{
    		nxtDisplayBigTextLine(6," I'M INNNNNNNNNN");
        nMotorEncoder[motorH] = 0;
    		while((nMotorEncoder[motorH]+totalEncoderCount) <= -thirdLevel)
    		{
    			motor[motorH] = 10; //down
    		}
    		motor[motorH] = 0;
    	}
    	totalEncoderCount = -thirdLevel;
    }
*********************** end third level auto */


/*********************** first level auto
        if (joy1Btn(2)) // A botton ----- first level
    {

     if((totalEncoderCount)> -firstLevel)  //  count goes down as arm goes up
    	{
    		nxtDisplayBigTextLine(0,"OTHER ONE");
    	  nMotorEncoder[motorH] = 0;
	    	while((nMotorEncoder[motorH]+totalEncoderCount) > -firstLevel)
	    	{
	      	motor[motorH] = -20; //up
	      }
      	motor[motorH] = 0;
    	}
    	else if(totalEncoderCount <= -firstLevel)
    	{
    		nxtDisplayBigTextLine(6," I'M INNNNNNNNNN");
        nMotorEncoder[motorH] = 0;
    		while((nMotorEncoder[motorH]+totalEncoderCount) <= -firstLevel)
    		{
    			motor[motorH] = 10; //down
    		}
    		motor[motorH] = 0;
    	}
    	totalEncoderCount += nMotorEncoder[motorH];
    }
*************** end first level auto */


  /*  else if (joy1Btn(4))		// 'y' botton
    {
      nMotorEncoder[motorH] = 0;
    	while(abs(nMotorEncoder[motorH])<4428*4)
    	{
      	motor[motorH] = -10;
      }
      motor[motorH] = 0;
    }
    */
    else if (joy1Btn(10))	// start botton - manual down
    {

    	nMotorEncoder[motorH] = 0;
    	while(abs(nMotorEncoder[motorH])<=25 && totalEncoderCount>-thirdLevel /*&& totalEncoderCount<=0*/)
    	{
	      motor[motorH] = 20; //up
    	}
    	motor[motorH] = 0;
    	totalEncoderCount += nMotorEncoder[motorH];
    }
    else if (joy1Btn(9)) // back botton
  	{
    	nMotorEncoder[motorH] = 0;
 //   	nxtDisplayStringAt(3,5,"encode %i",newEncoderVal);
 //   		wait10Msec(50);
    	while(abs(nMotorEncoder[motorH])<=25 && totalEncoderCount>-thirdLevel /*&& totalEncoderCount<=0*/)
    	{
	      motor[motorH] = -20; //up
    	}
    	motor[motorH] = 0;
    	totalEncoderCount += nMotorEncoder[motorH];
 //   	    	nxtDisplayStringAt(3,5,"encode %i",nMotorEncoder[motorH]);
 //   		wait10Msec(50);
 //   	wait1Msec(.1);
    }
		else if (joy1Btn(5)) //left top
		{
			int newArmVal = armServoVal+5;
			servo[servo6] = newArmVal;
			armServoVal = newArmVal;
			wait1Msec(100);
		}
		else if (joy1Btn(6))//right top
		{
			int newArmVal = armServoVal-5;
			servo[servo6] = newArmVal;
			armServoVal = newArmVal;
			wait1Msec(100);
		}
		else if (joy1Btn(7)) // left trigger
		{
			int newBucketVal = bucketVal+5;
			servo[servo5] = newBucketVal;
			bucketVal = newBucketVal;
			wait1Msec(100);
		}
		else if (joy1Btn(8)) //right trigger
		{
			int newBucketVal = bucketVal-5;
			servo[servo5] = newBucketVal;
			bucketVal = newBucketVal;
			wait1Msec(100);
		}
}
}

void autoPegs(int number)
{
			float autoName = autoArray[number];
	     if((totalEncoderCount)> -autoName)  //  count goes down as arm goes up
    	{
    		nxtDisplayBigTextLine(0,"OTHER ONE");
    	  nMotorEncoder[motorH] = 0;
    	  bool wereGood = true;
	    	while((nMotorEncoder[motorH]+totalEncoderCount) > -autoName && wereGood)
	    	{
	    		getJoystickSettings(joystick);
	    		if(!joy1Btn(3))
					{
	  	    	motor[motorH] = -20; //up
	  	    }
	  	    else
	  	    {
	  	    	wereGood = false;
	  	    }
	      }
      	motor[motorH] = 0;
    	}
    	else if(totalEncoderCount <= -autoName)
    	{
    		nxtDisplayBigTextLine(6," I'M INNNNNNNNNN");
        nMotorEncoder[motorH] = 0;
        bool wereGood = true;
    		while((nMotorEncoder[motorH]+totalEncoderCount) <= -autoName && wereGood)
    		{
    			getJoystickSettings(joystick);
    			if(!joy1Btn(3))
    			{
    				motor[motorH] = 20; //down
    			}
    			else
    			{
    				wereGood = false;
    			}
    		}
    		motor[motorH] = 0;
    	}
    	totalEncoderCount += nMotorEncoder[motorH];

}

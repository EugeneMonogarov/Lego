#pragma config(Sensor, S1,     sSonarLeft,     sensorSONAR)
#pragma config(Sensor, S2,     sLight,         sensorI2CCustom9V)
#pragma config(Sensor, S3,     sTouch,         sensorTouch)
#pragma config(Sensor, S4,     sSonarRight,    sensorSONAR)
#pragma config(Motor,  motorA,          mShLeft,       tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          mRight,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          mLeft,         tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "mindsensors-lineleader.h"
#include "mindsensors-irdist.h"


#define RELEASE//
//#define DEBUG

bool IsHereWhite();

task main()
{

	//-----------------------------------------------------
	while (SensorValue(sTouch)== 0)
	{
		sleep (10);
	}
	while (SensorValue(sTouch)== 1)
	{
		sleep (10);
	}

	//-----------------------------------------------------
#ifdef RELEASE
	sleep (5000);
#endif
	//-----------------------------------------------------
//motor[mShLeft]=15; // shovel light pressure
//sleep(20000);
//return;
	//-----------------------------------------------------
#ifdef RELEASE
	motor[mShLeft]=100; 				// shovel down
	LLinit(sLight);
	LLwakeUp(sLight);
	for (int i = 10; i <= 130; i = i + 10)
	{
		motor[mLeft] = i/10;					// go ahead smoothly
		motor[mRight]  = i/10;					// go ahead smoothly
		sleep(10);
	}
	motor[mShLeft]=15; // shovel light pressure
#endif

	//-----------------------------------------------------

	//-----------------------------------------------------
	const float sonarDistance  =59;
	float sonarLeft 					 = 0;				// current left sonar sensor
	float sonarRight 					 = 0;				// current left sonar sensor
	float sonarLeftOld 					 = 0;				// previous left sonar sensor
	float sonarRightOld 					 = 0;				// previous left sonar sensor
	SensorValue(sSonarLeft)	 = 0;				// init left sonar sensor
	SensorValue(sSonarRight) = 0;				// init right sonar sensor
	//-----------------------------------------------------

	int sensor = LLreadResult(sLight);

	// Initit motors
	//-----------------------------------------------------
	float vLeft 	= 10;					// right motor power
	float vRight 	= 10;					// left motor power

#ifdef RELEASE
	motor[mLeft] = vLeft;					// go ahead
	motor[mRight]  = vRight;					// go ahead

#endif

	//-----------------------------------------------------

	while (true)
	{
#ifdef RELEASE
		// Get light sensor value and turn when current value less or more than start value
		//-----------------------------------------------------
		if ( IsHereWhite() ) // U turn
		{
			motor[mLeft]   = -100;		  // move back
			motor[mRight]  = -100; 	  	// move back
			sleep(350);
			motor[mLeft]   = -50;		  // turn
			motor[mRight]  =  50;		  // turn
			sleep(350);
		}
#endif
		//-----------------------------------------------------

		// use sonar sensor here
		//-----------------------------------------------------
		sonarLeft  = SensorValue(sSonarLeft);
		sonarRight = SensorValue(sSonarRight);


		if (( sonarLeft  < 5 ) || ( sonarLeft > sonarDistance )) sonarLeft  = sonarDistance + 1; 		// protects from stupid sonar issue when value is 0
			if (( sonarRight < 5 ) || (sonarRight >  sonarDistance )) sonarRight = sonarDistance + 1; // protects from stupid sonar issue when value is 0

		sonarLeft = (sonarLeft + sonarLeftOld) / 2 ;
		sonarRight = (sonarRight + sonarRightOld) / 2 ;

		sonarLeftOld = sonarLeft;
		sonarRightOld = sonarRight;

#ifdef DEBUG
		displayVariableValues(0,sonarLeft);
		displayVariableValues(1,sonarRight);
#endif

		if ( sonarLeft < sonarDistance ) // see on the left
		{
			if ( sonarRight < sonarDistance ) // see on the right too. Go ahead
			{
				if (( sonarLeft < 24 ) || ( sonarRight < 24 )) // max power go ahead. Kill it
				{
					vLeft = 100 ;
					vRight  = 100 ;

				}
				else  // smart go ahead
				{
					vLeft = 100  * pow(sonarLeft / sonarRight , 1.2) ;
					vRight  = 100 * pow(sonarRight / sonarLeft , 1.2) ;

				}
			}
			else // see only left only. Turn left
			{
				vLeft = -20 + sonarLeft/1.5 ;
				vRight  = 20 + sonarLeft/1.5;
			}
		}
		else
		{
			if ( sonarRight < sonarDistance ) // see on the right only. Turn right
			{
				vLeft =  20 + sonarRight/1.5;

				vRight  = -20 + sonarRight/1.5 ;
			}
		}

#ifdef RELEASE
		// sets motors power value
		motor[mLeft] = vLeft;
		motor[mRight]  = vRight;

#endif

#ifdef DEBUG
		displayVariableValues(3,vLeft);
		displayVariableValues(4,vRight);
#endif

		sleep(10);
		if (SensorValue(sTouch) == 1) break ;
	}
	LLsleep(sLight);
}

// returns true when value of anysensor more than 45
bool IsHereWhite()
{
	tByteArray signalstr;
	LLreadSensorRaw(sLight, signalstr); // read the raw sensor data (8 bit data)

	for (int i = 0 ; i < 8; i++)
	{
		if ( signalstr[i] > 10 ) return true;
	}
	return false;
}
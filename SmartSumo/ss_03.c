#pragma config(Sensor, S1,     sSonarLeft,     sensorSONAR)
#pragma config(Sensor, S2,     sSonarRight,    sensorSONAR)
#pragma config(Sensor, S4,     sLight,         sensorI2CCustom9V)
#pragma config(Motor,  motorA,          mShLeft,       tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          mLeft,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          mRight,        tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "mindsensors-lineleader.h"

bool IsHereWhite();

task main()
{
	//-----------------------------------------------------
	sleep (5000);
	motor[mShLeft]=100;					// shovel down
	sleep(200);
	motor[mShLeft]=10;					  // shovel light pressure
	//-----------------------------------------------------

	//-----------------------------------------------------
	const int sonarDistance  = 55;
	int sonarLeft 					 = 0;				// current left sonar sensor
	int sonarRight 					 = 0;				// current left sonar sensor
	SensorValue(sSonarLeft)	 = 0;				// init left sonar sensor
	SensorValue(sSonarRight) = 0;				// init right sonar sensor
	//-----------------------------------------------------

	// Initit motors
	//-----------------------------------------------------
	int vLeft 	= 100;					// left motor power
	int vRight 	= 100;					// right motor power

	motor[mLeft]  = vLeft;					// go ahead
	motor[mRight] = vRight;					// go ahead
	//-----------------------------------------------------

	while (true)
	{
		// Get light sensor value and turn when current value less or more than start value
		//-----------------------------------------------------
		if ( IsHereWhite() ) // U turn
		{
			motor[mLeft]  = -100; 		// move back
			motor[mRight] = -100;		  // move back
			sleep(350);
			motor[mLeft]  =  35;		  // turn
			motor[mRight] = -35;		  // turn
			sleep(250);
		}
		//-----------------------------------------------------

		// use sonar sensor here
		//-----------------------------------------------------
		sonarLeft  = SensorValue(sSonarLeft);
		sonarRight = SensorValue(sSonarRight);

		if ( sonarLeft  < 5 ) sonarLeft  = 255; // protects from stupid sonar issue when value is 0
		if ( sonarRight < 5 ) sonarRight = 255; // protects from stupid sonar issue when value is 0

		if ( sonarLeft < sonarDistance ) // see on the left
		{
			if ( sonarRight < sonarDistance ) // see on the right too. Go ahead
			{
				vLeft  = 100 * sonarLeft / sonarRight ;
				vRight = 100 * sonarRight / sonarLeft ;
			}
			else // see only left only. Turn left
			{
				vLeft  =  15;
				vRight = -15;
			}
		}
		else
		{
			if ( sonarRight < sonarDistance ) // see on the right only. Turn right
			{
				vLeft  = -15;
				vRight =  15;
			}
		}

		// sets motors power value
		motor[mLeft]  = vLeft;
		motor[mRight] = vRight;
		sleep(1);
	}
}

// returns true when value of anysensor more than 25
bool IsHereWhite()
{
	tByteArray signalstr;
	LLreadSensorRaw(sLight, signalstr); // read the raw sensor data (8 bit data)

	for (int i = 0 ; i < 8; i++)
	{
		if ( signalstr[i] > 35 ) return true;
	}
	return false;
}

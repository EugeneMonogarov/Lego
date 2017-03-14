#pragma config(Sensor, S1,     sSonarLeft,     sensorSONAR)
#pragma config(Sensor, S2,     sSonarRight,    sensorSONAR)
#pragma config(Sensor, S3,     sSumo,          sensorLightActive)
#pragma config(Sensor, S4,     sLight,         sensorI2CCustom9V)
#pragma config(Motor,  motorA,          mShLeft,       tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          mLeft,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          mRight,        tmotorNXT, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "mindsensors-lineleader.h"
#include "mindsensors-sumoeyes.h"


int GetLightAverage();

task main()
{

	motor[mShLeft]=100;									// shovel down
	sleep(250);
	motor[mShLeft]=5;										// shovel light pressure

	MSSUMOsetLongRange(sSumo); 					// sets sumo sensor long range
	tObstacleZone zone = MSSUMO_NONE;		// sumo sensor result

	const int sonarDistance  = 50;
	int sonarLeft 					 = 0;				// current left sonar sensor
	int sonarRight 					 = 0;				// current left sonar sensor
	SensorValue(sSonarLeft)	 = 0;				// init left sonar sensor
	SensorValue(sSonarRight) = 0;				// init right sonar sensor

	int lightStart = GetLightAverage(); // remember initial light
	int lightCurrent = 0;

	int vLeft 	= 100;										// left motor power
	int vRight 	= 100;										// right motor power

	motor[mLeft] = vLeft;									// go ahead
	motor[mRight] = vRight;								// go ahead

	while (true)
	{
		lightCurrent = GetLightAverage();		// gt current light

		if (abs(GetLightAverage() - lightStart) > 70) // U turn
		{
			motor[mLeft]  = -100; 	// move back
			motor[mRight]= -100;		// move back
			sleep(500);
			motor[mLeft]  =  50;		// turn
			motor[mRight] = -50;		// turn
			sleep(350);
		}
		tObstacleZone zone = MSSUMOreadZone(sSumo); 	// get range from sume sensor

		switch (zone) {
		case MSSUMO_FRONT: // go ahead
			vLeft  = 100;
			vRight = 100;
			break;
		case MSSUMO_LEFT: // turn left
			vLeft  = -50;
			vRight =  50;
			break;
		case MSSUMO_RIGHT: // turn right
			vLeft  = 50;
			vRight = -50;
			break;
		case MSSUMO_NONE:
			// use sonar sensor here because sumo sensors returned nothing (none)
			sonarLeft  = SensorValue(sSonarLeft);
			sonarRight = SensorValue(sSonarRight);
			if ( sonarLeft < sonarDistance )
			{
				if ( sonarRight < sonarDistance )  // go ahead
				{
					vLeft  = 100;
					vRight = 100;
				}
				else 															// turn left
				{
					vLeft  = -50;
					vRight =  50;
				}
			}
			else
			{
				if ( sonarRight < sonarDistance ) // turn right
				{
					vLeft  =  50;
					vRight = -50;
				}
			}
			break;
		}

		motor[mLeft]=vLeft;
		motor[mRight]=vRight;
	}
}

// retruns light average from 0 - black to 100 - white
int GetLightAverage()
{
	int average = 0;
	int sensor = LLreadResult(sLight); // fetch the data from the sensor
	tByteArray signalstr;

	LLreadSensorRaw(sLight, signalstr); // read the raw sensor data (8 bit data)

	for (int i = 0 ; i<8; i++)
	{
		average += signalstr[i];
	}
	average /= 8;
	return average;
}

#pragma config(Sensor, S1,     sLightLeft,     sensorLightActive)
#pragma config(Sensor, S2,     Sonar,          sensorEV3_IRSensor)
#pragma config(Sensor, S3,     sLightRight,    sensorLightActive)
#pragma config(Sensor, S4,     sColor,         sensorEV3_Color, modeEV3Color_Ambient)
#pragma config(Motor,  motorA,           ,             tmotorEV3_Large, openLoop)
#pragma config(Motor,  motorB,          motorLeft,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorRight,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	sleep(3000);
	//	SensorType[sColor] = sensorEV3_Color;
	//SensorMode[sColor] = modeEV3Color_Color;
	//wait1Msec(2000);
	SensorType[sColor] = sensorNone ;
	//sleep(2000);
	//SensorType[sColor] = sensorEV3_Color;
	//SensorMode[sColor] = modeEV3Color_Color;
	//wait1Msec(2000);
	//SensorType[sColor] = sensorNone ;
	//sleep(2000);

	//----------------------------------
	int es = SensorValue(sLightLeft) - SensorValue(sLightRight);
	int e       = 0;
	int eOld    = e;
	int u       = 0;
	int const v = 80;
	int i       = 0;
	//----------------------------------
	e  = SensorValue(sLightLeft) - SensorValue(sLightRight) - es;
	eOld = e;
	//----------------------------------
	while(true)
	{


		//-------------------------
		if((SensorValue(sLightLeft) < 30) && (SensorValue(sLightRight) < 30))
		{
			i++ ;
			if( i % 2 != 0)
			{
				motor[motorLeft]  = motor[motorLeft] / 2 ;
				motor[motorRight] = motor[motorRight] / 2;
			}
			while((SensorValue(sLightLeft) < 30) && (SensorValue(sLightRight) < 30))
			{
				sleep(10);
			}

			if( i % 2 != 0)
			{
				SensorType[sColor] = sensorEV3_Color;
				SensorMode[sColor] = modeEV3Color_Color;
				SensorType[sLightLeft] = sensorLightInactive;
				SensorType[sLightRight] = sensorLightInactive;
				sleep(100);
				motor[motorLeft]  = motor[motorLeft] / 2;
				motor[motorRight] = motor[motorRight] / 2;


				while(true)
				{
					if(getColorName(sColor) == colorGreen) break;
					motor[motorLeft]  = 0;
					motor[motorRight] = 0;
				}
				SensorType[sColor] = sensorNone;
				SensorType[sLightLeft] = sensorLightActive;
				SensorType[sLightRight] = sensorLightActive;
			}
		}
		else
		{
			//-------------------------
			e = SensorValue(sLightLeft) - SensorValue(sLightRight) - es;
			u = e * 1.5 + (e - eOld) * 5;
			eOld = e;
			//-------------------------
			//motor[motorLeft] = v - u;
			//motor[motorRight] = v + u;
		}
		sleep(10);
	}
}
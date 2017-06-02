#pragma config(Sensor, S1,     sLightLeft,     sensorLightActive)
#pragma config(Sensor, S2,     Sonar,          sensorEV3_IRSensor)
#pragma config(Sensor, S3,     sLightRight,    sensorLightActive)
#pragma config(Sensor, S4,     sColor,         sensorEV3_Color, modeEV3Color_Color)
#pragma config(Motor,  motorA,           ,             tmotorEV3_Large, openLoop)
#pragma config(Motor,  motorB,          mLeft,         tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          mRight,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	sleep(2500);
	//	SensorType[sColor] = sensorEV3_Color;
	//SensorMode[sColor] = modeEV3Color_Color;
	//wait1Msec(2000);
	//SensorType[sColor] = sensorNone ;
	//sleep(2000);
	//SensorType[sColor] = sensorEV3_Color;
	//SensorMode[sColor] = modeEV3Color_Color;
	//wait1Msec(2000);
	//SensorType[sColor] = sensorNone ;
	//sleep(2000);

	//----------------------------------
	int es 					= SensorValue(sLightLeft) - SensorValue(sLightRight);
	int e     			= 0;
	int eOld  			= e;
	int u     			= 0;
	int const vMax  = 80;
	int const vMin  = 0;
	int v 					= vMax;
	int i     			= 0;
	int s 					= SensorValue(Sonar);
	int vLeft 			= 0;
	int vRight 			= 0;
	//----------------------------------
	e  = SensorValue(sLightLeft) - SensorValue(sLightRight) - es;
	eOld = e;
	//----------------------------------
	while(true)
	{
		s = SensorValue(Sonar);
		//-------------------------
		if ( s > 40 )      v = vMax;
		else if ( s > 35 ) v = vMax - 10;
		else if ( s > 30 ) v = vMax - 20;
		else if ( s > 25 ) v = vMax - 30;
		else if ( s > 20 ) v = vMax - 45;
		else if ( s > 15 ) v = vMax - 60;
		else               v = vMin ;
		//-------------------------
		if((SensorValue(sLightLeft) < 30) && (SensorValue(sLightRight) < 30))
		{
			i++ ;
			if( i % 2 != 0 )
			{
				motor[mLeft]  = v / 2 ;
				motor[mRight] = v / 2 ;
				//SensorType[sColor] = sensorEV3_Color;
				//SensorMode[sColor] = modeEV3Color_Color;
			}
			while((SensorValue(sLightLeft) < 30) && (SensorValue(sLightRight) < 30))
			{
				sleep(10);
			}

			if( i % 2 != 0)
			{
				sleep(100);
				//motor[motorLeft]  = motor[motorLeft] / 2;
				//motor[motorRight] = motor[motorRight] / 2;
				while(true)
				{
					if(getColorName(sColor) == colorGreen) break;
					motor[mLeft]  = 0;
					motor[mRight] = 0;
					sleep(10);
					//SensorType[sLightLeft] = sensorLightInactive;
					//SensorType[sLightRight] = sensorLightInactive;
				}
				//SensorType[sColor] = sensorNone;
				//SensorType[sLightLeft] = sensorLightActive;
				//SensorType[sLightRight] = sensorLightActive;
			}
		}
		else
		{
			//-------------------------
			e = SensorValue(sLightLeft) - SensorValue(sLightRight) - es;
			u = e * 1.5 + (e - eOld) * 5;
			eOld = e;
			vLeft = v - u;
			vRight = v + u;
			//-------------------------

			if ( vLeft  < vMin ) vLeft  = vMin;
			if ( vRight < vMin ) vRight = vMin;
			if ( vLeft  > vMax ) vLeft  = vMax;
			if ( vRight > vMax ) vRight = vMax;
			motor[mLeft]  = vLeft;
			motor[mRight] = vRight;
		}
		sleep(10);
	}
}

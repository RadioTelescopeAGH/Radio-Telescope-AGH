#include <Wire.h>
#include <LSM6.h>
#include <math.h>

LSM6 imu;

struct Axis {
	float x;
	float y;
	float z;
};

struct Angle {
	float pitch;
	float roll;
};

Axis acceleration;
Axis offset;
Angle angle;

float accelerometer(float tab[]) {
 
	acceleration = {};
	angle = {};
	/*
  offset.x = -16;
  offset.y = 14;
  offset.z = -263;
  */
	imu.read();
	delay(1);
  
	acceleration.x = imu.a.x;// + offset.x; ??
  acceleration.y = imu.a.y;// + offset.y; ??
  acceleration.z = imu.a.z;// + offset.z; ??
	
	//Accelerometer pitch, yawn and roll angles:
	angle.pitch = 57.2957795 * atan(acceleration.x / sqrt(acceleration.y*acceleration.y + acceleration.z*acceleration.z));
	angle.roll =  57.2957795 * atan(acceleration.y / sqrt(acceleration.x*acceleration.x + acceleration.z*acceleration.z));
  tab[0] = angle.pitch;
  tab[1] = angle.roll;
}


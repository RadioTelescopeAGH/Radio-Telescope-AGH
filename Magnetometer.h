#include <Wire.h>
#include <LIS3MDL.h>
#include <math.h>

struct Magnetic {
	int x;
	int y;
	int z;
};
LIS3MDL mag;
float magnetometer(float magnetic[]) {
	mag.read();
	delay(1);
	Magnetic offset;
	
	offset.x = -2373 + 2700 - 1500 - 200; //XDDDDDDDDDDDDD
	offset.y = 5909 - 1500 - 1500; //XDDDDDDDDDD
	magnetic[0] = mag.m.x + offset.x; //6842.00000;
	magnetic[1] = mag.m.y + offset.y; //6842.00000;
	magnetic[2] = mag.m.z; //6842.00000;

//	float heading = atan2(magnetic.y, magnetic.x);
}

float wrap(float angle){
    float pi = 3.1415;
    if (angle > pi){
        angle -= (2*pi);
        }
    if (angle < -pi){
        angle += (2*pi);
        }
    if (angle < 0){
        angle += 2*pi;
        }
    return angle;
}


float tilt_compensated_magnetometer(float mag_x, float mag_y, float mag_z, float roll, float pitch){
    /*
  KRAKOW
  Latitude: 50� 4' 60" N
  Longitude: 19� 55' 0" E
  Magnetic declination: +5� 16'
  Declination is POSITIVE (EAST)
  Inclination: 66� 22'
  Magnetic field strength: 49445.3 nT
  */
  float offset = 5.26;
  Magnetic compensated_mag={};
  compensated_mag.x = mag_x * cos(pitch) + mag_y * sin(roll) * sin(pitch) +/* -??*/ mag_z * cos(roll) * sin(pitch);
  compensated_mag.y = mag_y * cos(roll) - mag_z * sin(roll); 
  return wrap((atan2(-compensated_mag.y, compensated_mag.x) + offset ));
}

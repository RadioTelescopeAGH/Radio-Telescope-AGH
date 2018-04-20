#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>
#include <math.h>

#include "Accelerometer.h"
#include "Magnetometer.h"
//

//
#define BIG_PUSH_BUTTON      4
#define IGNATION_SWITCH_1    6
#define IGNATION_SWITCH_2    7
#define ONEWIRE_PIN          5
#define TERMOMETERS          5

const byte address[5][8] PROGMEM{
  {0x28, 0xFF, 0xC4, 0x67, 0x31, 0x17, 0x3, 0x76},// green
  {0x28, 0xEA, 0x53, 0xE, 0xA, 0x0, 0x0, 0x5C},   // violet
  {0x28, 0xFF, 0x7E, 0x5, 0x31, 0x17, 0x3, 0x13}, // yellow
  {0x28, 0xFF, 0x28, 0x4C, 0x31, 0x17, 0x3, 0x69},// blue
  {0x28, 0xE6, 0x92, 0xD, 0xA, 0x0, 0x0, 0x46}    // black
 };

float temperature[5] = {0,0,0,0,0};
float angles[2] = {0, 0};
float magnetic[3] = {0,0,0};
float magnetic_angle = 0;

bool power_up = true;

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  attachInterrupt(digitalPinToInterrupt(3), emergency_shutdown, RISING);
  
  //switches 
  pinMode(4, INPUT_PULLUP);  
  pinMode(6, INPUT_PULLUP);  
  pinMode(7, INPUT_PULLUP);
  
  //relays
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  //serial
  Serial.begin(9600);
  Serial.println("HELLO");
  //onewire protocol
  Wire.begin();
  //termometers
  sensors.begin();
  sensors.request();

//check if main sensors are operational  
 if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
  
   if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }

  mag.enableDefault();

  
  // LCD: 16 chars 2 lines
  // Intro sequence 
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("S.H.I.EL.D.");
  lcd.setCursor(0,1);
  lcd.print("of AGH");//??
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INSERT 2 KEYS");
  lcd.setCursor(0,1);
  lcd.print("TO POWER UP");
  while (!power_up){
    if(digitalRead(6) == HIGH && digitalRead(7)== HIGH){
      //main 230VAC power line relay 
      digitalWrite(8, HIGH);
      power_up = false;
      
    }
  }*/
}

void loop() {
    accelerometer(angles);
    magnetometer(magnetic);
    magnetic_angle = tilt_compensated_magnetometer(magnetic[0], magnetic[1], magnetic[2], angles[0], angles[1]);    
    
}

void emergency_shutdown (){
  digitalWrite(8, LOW); //cut off main PSU voltage 230VAC
  lcd.setCursor(0,0);
  lcd.print("EMERGENCY");
  lcd.setCursor(0,1);
  lcd.print("SHUTDOWN");
   
}

void termometer(){ //get readings from all ddigital termometers
   if (sensors.available())
  {
    for (byte i=0; i<TERMOMETERS; i++)
    {
      float temperatureC = sensors.readTemperature(FA(address[i]));

       *(temperature+i) = temperatureC;
      
    }

    sensors.request();
  }
}


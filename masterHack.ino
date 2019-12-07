#include <EEPROM.h>
#include <Wire.h>     
#include <MPU6050.h>
MPU6050 accelerometer;
int timeF = 10;
int sect = 1;
int val = 0;
int val2 = 0;
int dev1 = 0;
int dev2 = 0;
int sto1 = 0;
int sto2 = 0;
int avg1 = 0;
double avg2 = 130;
int fdev1 = 0;
int fdev2 = 0;
int occ = 0;
int d=0;
int tim = 0;
int sd = 0;
int dWin = 0;
double sWin = 0;
void setup() {
  pinMode(2, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600); 
  delay(2000);
  while(!accelerometer.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) // Checking whether the mpu6050 is sensing or not
  {
    Serial.println("Failed to read from the sensor. Something is Wrong!");
    delay(500);
  }  
}

void loop() {
  Vector sensor_data = accelerometer.readNormalizeAccel();
  int pitch_value = -(atan2(sensor_data.XAxis, sqrt(sensor_data.YAxis*sensor_data.YAxis + sensor_data.ZAxis*sensor_data.ZAxis))*180.0)/M_PI;  // Calculating the pitch value
  Serial.print(pitch_value);
  Serial.print(",");// Printing the pitch values on the Serial Monitor
  int roll_value = (atan2(sensor_data.YAxis, sensor_data.ZAxis)*180.0)/M_PI;    // Calculating the Raw value
  Serial.print(roll_value);
  Serial.print(",");// printing the Roll values on the serial monitor
  int windSpeed = (analogRead(A2)-400)/4;
  delay(10);
  val = val+pitch_value;
  val2 = val2+windSpeed; 
  if (sect == 1){
    sto1 = val;
    sect++;
  }
  else if(sect>=timeF){
    sect = 1;
    avg1 = val/timeF;
    avg2 = val2/timeF;
    val = 0;
    val2 = 0;
    occ = 0;
    dev1 = pitch_value-avg1;
  }
  else{
    sect++;
    dev1 = pitch_value-avg1;
  }
  fdev1 = dev1*7;
  d = fdev1;
  if(d<0){
    d = d*-1;
  }
  if (d<sd){
    d = sd-1;
  }
  if (d>400){
    d = 400;
  }
  sd = d;
  if (avg2>sWin){
    avg2 = sWin+0.5;
  }
  if (avg2<140){
    dWin = 140-avg2;
  }
  else{
    dWin = 0;
  }
  sWin = avg2;
  int led = (d/40)+6+dWin;
  if(led>13){
    led = 13;
  }
  for(int i = 6;i<=led;i++){
    digitalWrite(i,HIGH);
  }
  for(int i = led+1;i<=13;i++){
    digitalWrite(i,LOW);
  }
  int awareness = led-5;
  if(awareness>5){
    digitalWrite(2,HIGH);
  }
  else{
    digitalWrite(2,LOW);
  }
  Serial.print(avg1);
  Serial.print(",");
  Serial.print(fdev1);
  Serial.print(",");
  Serial.print(awareness);
  Serial.print(",");
  Serial.println(avg2);
  Serial.print(",");

}

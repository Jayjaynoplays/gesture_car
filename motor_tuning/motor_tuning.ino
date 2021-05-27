/*
FL:        FR:
 
BL:        BR:


*/


int motoDrivers[] = {2, 3, 4, 5, 9, 10, 11, 12};
int led = 14;

int fr_in3 = motoDrivers[1];
int fr_in4 = motoDrivers[0];

int br_in1 = motoDrivers[3];
int br_in2 = motoDrivers[2];

int fl_in1 = motoDrivers[4];
int fl_in2 = motoDrivers[5];

int bl_in3 = motoDrivers[6];
int bl_in4 = motoDrivers[7];

void setup() {
  for(int i=0; i<=7; i++ ){
    pinMode(motoDrivers[i],OUTPUT);
  }
  pinMode(led, OUTPUT);
  delay(2000);
}

void loop() {
  digitalWrite(led,HIGH);
  forward(52,55,50,50);
//  digitalWrite(9,HIGH);
//  bl_ccw(255);                             

//  stop_car();
//  forward(12, 12, 12, 12);
//  back(12, 12, 12, 12);
//  go_left(12, 12, 12, 12);
//  go_right(12, 12, 12, 12);
//  turn_CW(12, 12, 12, 12);
//  turn_CCW(12, 12, 12, 12);
//  forward_right(12, 12);
//  forward_left(12, 12);
//  back_right(12, 12);
//  back_left(12, 12);
}

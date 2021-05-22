void fr_stop(){
  analogWrite(fr_in3, 0);
  analogWrite(fr_in4, 0);
}

void fl_stop(){
  analogWrite(fl_in1, 0);
  analogWrite(fl_in2, 0);
}

void br_stop(){
  analogWrite(br_in1, 0);
  analogWrite(br_in2, 0);
}

void bl_stop(){
  analogWrite(bl_in3, 0);
  analogWrite(bl_in4, 0);
}

// --------------------------------------------------------------------------------------------------------------------------------- //

void fr_cw(int anaValue){
  analogWrite(fr_in4, 0);
  analogWrite(fr_in3, anaValue);
}

void fl_cw(int anaValue){
  analogWrite(fl_in1, 0);
  analogWrite(fl_in2, anaValue);
}

void br_cw(int anaValue){
  analogWrite(br_in2, 0);
  analogWrite(br_in1, anaValue);
}

void bl_cw(int anaValue){
  analogWrite(bl_in3, 0);
  analogWrite(bl_in4, anaValue);
}

// --------------------------------------------------------------------------------------------------------------------------------- //

void fr_ccw(int anaValue){
  analogWrite(fr_in3, 0);
  analogWrite(fr_in4, anaValue);
}

void fl_ccw(int anaValue){
  analogWrite(fl_in2, 0);
  analogWrite(fl_in1, anaValue);
}

void br_ccw(int anaValue){
  analogWrite(br_in1, 0);
  analogWrite(br_in2, anaValue);
}

void bl_ccw(int anaValue){
  analogWrite(bl_in4, 0);
  analogWrite(bl_in3, anaValue);
}

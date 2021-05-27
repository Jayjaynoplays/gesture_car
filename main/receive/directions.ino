void stop_car(){
  fr_stop();
  fl_stop();
  br_stop();
  bl_stop();
}

void forward(int fr, int fl, int br, int bl){
  fr_cw(fr);
  fl_cw(fl);
  br_cw(br);
  bl_cw(bl);
}

void back(int fr, int fl, int br, int bl){
  fr_ccw(fr);
  fl_ccw(fl);
  br_ccw(br);
  bl_ccw(bl);
}

void go_left(int fr, int fl, int br, int bl){
  fr_cw(fr);
  fl_ccw(fl);
  br_ccw(br);
  bl_cw(bl);
}

void go_right(int fr, int fl, int br, int bl){
  fr_ccw(fr);
  fl_cw(fl);
  br_cw(br);
  bl_ccw(bl);
}

void turn_CW(int fr, int fl, int br, int bl){
  fr_ccw(fr);
  fl_cw(fl);
  br_ccw(br);
  bl_cw(bl);
}

void turn_CCW(int fr, int fl, int br, int bl){
  fr_cw(fr);
  fl_ccw(fl);
  br_cw(br);
  bl_ccw(bl);
}

void forward_right(int fl, int br){
  fr_stop();
  fl_cw(fl);
  br_cw(br);
  bl_stop();
}

void forward_left(int fr, int bl){
  fr_cw(fr);
  fl_stop();
  br_stop();
  bl_cw(bl);
}

void back_right(int fr, int bl){
  fr_ccw(fr);
  fl_stop();
  br_stop();
  bl_ccw(bl);
}

void back_left(int fl, int br){
  fr_stop();
  fl_ccw(fl);
  br_ccw(br);
  bl_stop();
}

void displacementCalc()
{
  // accounting for acceleration due to gravity
  float No_Gravity = Accel_Vector[2] - GRAVITY;
  float Accel_Check[3] = {0,0,0};
  
  int zeroCounter=0;
  
  Accel_Check[0] = abs(Accel_Prev[0]-Accel_Vector[0]);
  Accel_Check[1] = abs(Accel_Prev[1]-Accel_Vector[1]);
  Accel_Check[2] = abs(Accel_Prev[2]-Accel_Vector[2]);
  
  // accounting for drift in acceleration  
  if (timeCounter > 1 && Accel_Check[0] < 3 && Accel_Check[1] < 3 && Accel_Check[2] < 3)
  {
    dis_x = 0;
    dis_y = 0;
    dis_z = 0;
    zeroCounter++;
    if (zeroCounter>10)
    {
      Vel_Vector[0]=0;
      Vel_Vector[1]=0;
      Vel_Vector[2]=0;
    }
  }
  else
  {
    removeSmallAcceleration();
    zeroCounter=0;
    //computing displacement
    dis_x = Vel_Vector[0]*SAMPLING_PERIOD + 0.5*Accel_Vector[0]*SAMPLING_PERIOD*SAMPLING_PERIOD;
    dis_y = Vel_Vector[1]*SAMPLING_PERIOD + 0.5*Accel_Vector[1]*SAMPLING_PERIOD*SAMPLING_PERIOD;
    dis_z = Vel_Vector[2]*SAMPLING_PERIOD + 0.5*No_Gravity*SAMPLING_PERIOD*SAMPLING_PERIOD;
    
    //calculating final velocity
    New_Vel_Vector[0] = Vel_Vector[0] + Accel_Vector[0]*SAMPLING_PERIOD;
    New_Vel_Vector[1] = Vel_Vector[1] + Accel_Vector[1]*SAMPLING_PERIOD;
    New_Vel_Vector[2] = Vel_Vector[2] + No_Gravity*SAMPLING_PERIOD;
    
    //setting velocity vector to final velocity
    Vel_Vector[0] = New_Vel_Vector[0];
    Vel_Vector[1] = New_Vel_Vector[1];
    Vel_Vector[2] = New_Vel_Vector[2];
  }
}

void removeSmallAcceleration() {
  if (Accel_Vector[0]<3) {
    Accel_Vector[0]=0.0;
    Vel_Vector[0]=0;
  }
  if (Accel_Vector[1]<3) {
    Accel_Vector[1]=0;
    Vel_Vector[1]=0;
  }
  if (Accel_Vector[2]<3) {
    Accel_Vector[2]=0;
    Vel_Vector[2]=0;
  }
}

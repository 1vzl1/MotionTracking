/*

MinIMU-9-Arduino-AHRS
Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

Copyright (c) 2011 Pololu Corporation.
http://www.pololu.com/

MinIMU-9-Arduino-AHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
http://code.google.com/p/sf9domahrs/

sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
Julio and Doug Weibel:
http://code.google.com/p/ardu-imu/

MinIMU-9-Arduino-AHRS is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

MinIMU-9-Arduino-AHRS is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License along
with MinIMU-9-Arduino-AHRS. If not, see <http://www.gnu.org/licenses/>.

*/

void printdata(void)
{    
      Serial.print("!");

      #if PRINT_EULER == 1
      Serial.print("ANG:");
      Serial.print(ToDeg(roll));
      Serial.print(",");
      Serial.print(ToDeg(pitch));
      Serial.print(",");
      Serial.print(ToDeg(yaw));
      #endif      
      #if PRINT_ANALOGS==1
      Serial.print(",AN:");
      Serial.print(AN[0]);  //(int)read_adc(0)
      Serial.print(",");
      Serial.print(AN[1]);
      Serial.print(",");
      Serial.print(AN[2]);  
      Serial.print(",");
      Serial.print(AN[3]);
      Serial.print (",");
      Serial.print(AN[4]);
      Serial.print (",");
      Serial.print(AN[5]);
      Serial.print(",");
      Serial.print(c_magnetom_x);
      Serial.print (",");
      Serial.print(c_magnetom_y);
      Serial.print (",");
      Serial.print(c_magnetom_z);
      #endif
      /*#if PRINT_DCM == 1
      Serial.print (",DCM:");
      Serial.print(convert_to_dec(DCM_Matrix[0][0]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[0][1]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[0][2]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[1][0]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[1][1]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[1][2]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[2][0]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[2][1]));
      Serial.print (",");
      Serial.print(convert_to_dec(DCM_Matrix[2][2]));
      #endif*/
      Serial.println();    
      
}

void printDataToFile() {
     Serial.print("VALUE:");
      Serial.print(timeCounter);
      Serial.print(",");
      Serial.print(time);
      Serial.print(",");
      /*Serial.print(ToDeg(roll));
      Serial.print(",");
      Serial.print(ToDeg(pitch));
      Serial.print(",");
      Serial.print(ToDeg(yaw));
      Serial.print(",");*/
      Serial.print(Accel_Vector[0]);
      Serial.print(",");
      Serial.print(Accel_Vector[1]);
      Serial.print(",");
      Serial.print(Accel_Vector[2]);
      Serial.print(",");
      Serial.print(Dis_Vector[0]);
      Serial.print(",");
      Serial.print(Dis_Vector[1]);
      Serial.print(",");
      Serial.println(Dis_Vector[2]);
}

void oneAxisPrint() {
  if (Dis_Vector[2]!=0)
  {
    Serial.print(time);
    Serial.print(",");
    Serial.print(Accel_Vector[2]);
    Serial.print(",");
    Serial.print(Vel_Vector[2]);
    Serial.print(",");
    Serial.println(Dis_Vector[2]);
  }
}

void printToFormat() {
  //Format is:
  //Packet number, Gyroscope x, Gyroscope y, Gyroscope z, Accelerometer x, Accelerometer y, Accelerometer z, Magnetometer x, Mangetometer y, Magnetometer z
  Serial.print("VALUE:");//this line is used to seperate relevent data from non-relevent data in CSV generator. Processing removes this string from the generated file
  Serial.print(timeCounter);
  Serial.print(",");
  Serial.print(Gyro_Vector[0]);
  Serial.print(",");
  Serial.print(Gyro_Vector[1]);
  Serial.print(",");
  Serial.print(Gyro_Vector[2]);
  Serial.print(",");
  Serial.print(Accel_Vector[0]);
  Serial.print(",");
  Serial.print(Accel_Vector[1]);
  Serial.print(",");
  Serial.print(Accel_Vector[2]-256);//-256 to account for gravity
  Serial.print(",");
  Serial.print(c_magnetom_x);
  Serial.print(",");
  Serial.print(c_magnetom_y);
  Serial.print(",");
  Serial.println(c_magnetom_z);
}

long convert_to_dec(float x)
{
  return x*10000000;
}


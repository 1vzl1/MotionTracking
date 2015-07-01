//Jeff's custom code used for recording change in motion
float momentOne[3]={0,0,0};
float momentTwo[3];
float change[3][250];
int count=0;
void recordMoment(void)
{
  if (momentOne[0]==0&&momentOne[1]==0&&momentOne[2]==0)
  {
    Serial.print("FIRST MOMENT IS RECORDED");
      momentOne[0]=ToDeg(roll);
      momentOne[1]=ToDeg(pitch);
      momentOne[2]=ToDeg(yaw);
  }
  else if(significantChange())
  {
    momentTwo[0]=ToDeg(roll);
    momentTwo[1]=ToDeg(pitch);
    momentTwo[2]=ToDeg(yaw);
    calculateChange();
    momentOne[0]=ToDeg(roll);
    momentOne[1]=ToDeg(pitch);
    momentOne[2]=ToDeg(yaw);
  }
}
void calculateChange(void)
{
  change[0][count]=abs(momentTwo[0]-momentOne[0]);
  change[1][count]=abs(momentTwo[1]-momentOne[1]);
  change[2][count]=abs(momentTwo[2]-momentOne[2]);
  count++;
}
boolean significantChange()
{
  if (abs(ToDeg(roll)-momentOne[0])>1)
  {
    if(abs(ToDeg(pitch)-momentOne[1])>1)
    {
      if(abs(ToDeg(yaw)-momentOne[2])>1)
      {
        Serial.println("SIGNIFICANT CHANGE DETECTED");
        return true;
      }
    }
  }
  return false;
}

void printChanges(void)
{
  for (count=0;count<250;count++)
  {
    if (change[0][count]!=0&&change[1][count]!=0&&change[2][count]!=0)
    {
      Serial.println(" ");
      Serial.print("ANG:");
      Serial.print(change[0][count]);
      Serial.print(",");
      Serial.print(change[1][count]);
      Serial.print(",");
      Serial.print(change[2][count]);
    }
  }
}
/*
ANG:1.11,-1.10,46.80 

ANG:1.52,1.02,-8.66 

ANG:-2.10,-1.06,-8.67 

ANG:-1.86,-1.02,-49.64 

ANG:1.01,1.39,-4.75
*/

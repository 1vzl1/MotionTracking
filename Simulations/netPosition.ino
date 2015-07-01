#define NUMBER_OF_SIMULATIONS 6
#define NUMBER_OF_DELAYS 10
//max delay is in seconds
#define MAX_DELAY 5
//simulation length in seconds
#define SIMULATION_LENGTH 25

float delayIncrement=(float)(MAX_DELAY)/((float)(NUMBER_OF_DELAYS));
int simulationCounter=0;
float driftArray[3]={0,0,0};
float oldPosition[3]={0,0,0};
float largestDrift[3][NUMBER_OF_SIMULATIONS]={0,0,0};
//[the three metrics][number of simulations][neg=0, pos=1]
float averagePosNegDrifts[3][NUMBER_OF_SIMULATIONS][2];
float averageOfAveragePosNegDrifts[3][NUMBER_OF_DELAYS][2];
float averageDrift[3][NUMBER_OF_DELAYS];
float startupDelay=0;
int delayCount=0;
int negativeDriftCount[3]={0,0,0};
int positiveDriftCount[3]={0,0,0};
boolean simulationsComplete=false;

boolean getSimulationsComplete() {
  return simulationsComplete;
}
int getSimulationLength()
{
  return SIMULATION_LENGTH;
}
int getSimulationCounter()
{
  return simulationCounter;
}
//DOES NOT CONSIDER FREQUENCY
float getStartupDelay() 
{
  return startupDelay;
}
void nextDelay()
{
  startupDelay+=delayIncrement;
  calculateAverageDrift();
  calculateAverageOfAveragestDrifts();
  setupAveragestDrifts();
  delayCount++;
  if (startupDelay>=MAX_DELAY)
  {
    simulationsComplete=true;
  }
}
void setFirstPosition() {
  oldPosition[0]=ToDeg(roll);
  oldPosition[1]=ToDeg(pitch);
  oldPosition[2]=ToDeg(yaw);
}

void calculateDrift()
{
  driftArray[0]=oldPosition[0]-ToDeg(roll);
  driftArray[1]=oldPosition[1]-ToDeg(pitch);
  driftArray[2]=oldPosition[2]-ToDeg(yaw);
  saveAveragestDrifts();
  setFirstPosition();
}
void printDrift()
{
  Serial.print("Drift from previous position: ");
  Serial.print(driftArray[0]);
  Serial.print(",");
  Serial.print(driftArray[1]);
  Serial.print(",");
  Serial.println(driftArray[2]);
}
void saveLargestDrift()
{
   if (abs(driftArray[0])>abs(largestDrift[0][simulationCounter]))
   {
    largestDrift[0][simulationCounter]=driftArray[0];
   }
   if (abs(driftArray[1])>abs(largestDrift[1][simulationCounter]))
   {
    largestDrift[1][simulationCounter]=driftArray[1];
   }
   if (abs(driftArray[2])>abs(largestDrift[2][simulationCounter]))
   {
    largestDrift[2][simulationCounter]=driftArray[2];
   }
}
void setupAveragestDrifts() {
    for (int i=0;i<2;i++)
    {
     for (int k=0;k<NUMBER_OF_SIMULATIONS;k++)
      {
       for (int j=0;j<2;j++)
       {
         averagePosNegDrifts[i][k][j]=0;
       }
    }
  }
}
void saveAveragestDrifts() {
  for (int i=0;i<3;i++)
  {
    if (driftArray[i]<0)
    {
      averagePosNegDrifts[i][simulationCounter][0]+=driftArray[i];
      negativeDriftCount[i]++;
    }
    else
    {
      averagePosNegDrifts[i][simulationCounter][1]+=driftArray[i];
      positiveDriftCount[i]++;
    }
  }
}
void calculateAveragestDrifts() {
  for (int i=0;i<3;i++)
  {
    averagePosNegDrifts[i][simulationCounter][0]=averagePosNegDrifts[i][simulationCounter][0]/negativeDriftCount[i];
    averagePosNegDrifts[i][simulationCounter][1]=averagePosNegDrifts[i][simulationCounter][1]/positiveDriftCount[i];
    negativeDriftCount[i]=0;
    positiveDriftCount[i]=0;
  }
}
//I KNOW THIS IS A BAD NAME! it's getting the average of the drifts across a simulation and averaging it across delays (each with a series of simulations)
void calculateAverageOfAveragestDrifts(){
  for (int k=0;k<2;k++)
  {
    for (int j=0;j<3;j++)
    {
      averageOfAveragePosNegDrifts[j][delayCount][k]=0;
      for (int i=0;i<NUMBER_OF_SIMULATIONS;i++)
      {
         averageOfAveragePosNegDrifts[j][delayCount][k]+=averagePosNegDrifts[j][i][k];
      }
      averageOfAveragePosNegDrifts[j][delayCount][k]=averageOfAveragePosNegDrifts[j][delayCount][k]/NUMBER_OF_SIMULATIONS;
    }
  }
}
void printLargestDrift()
{
  for (int i=0;i<NUMBER_OF_SIMULATIONS;i++)
  {
    Serial.println("");
    Serial.println("");
    Serial.print("The largest drift detected in simulation ");
    Serial.print(i+1);
    Serial.println(" was:");
    Serial.print(largestDrift[0][i]);
    Serial.print(",");
    Serial.print(largestDrift[1][i]);
    Serial.print(",");
    Serial.println(largestDrift[2][i]);
  }
}

void nextSimulation() 
{
  calculateAveragestDrifts();
  if (simulationCounter<(NUMBER_OF_SIMULATIONS-1))
  {
    simulationCounter++;
  }
  else
  {
    simulationCounter=0;
    nextDelay();
  }
}
void calculateAverageDrift()
{
  for (int j=0;j<3;j++)
  {
    averageDrift[j][delayCount]=0;
    for (int i=0;i<NUMBER_OF_SIMULATIONS;i++)
    {
      averageDrift[j][delayCount]+=largestDrift[j][i];
    }
    averageDrift[j][delayCount]=averageDrift[j][delayCount]/NUMBER_OF_SIMULATIONS;
  }
}
void printAverageDrift()
{
  for (int i=0;i<NUMBER_OF_DELAYS;i++)
  {
    Serial.println("");
    Serial.print("The average of the largest absolute drift in ");
    Serial.print(NUMBER_OF_SIMULATIONS);
    Serial.print(" simulations with a ");
    Serial.print(i*delayIncrement);
    Serial.println(" second delay was:");
    Serial.print(largestDrift[0][i]);
    Serial.print(",");
    Serial.print(largestDrift[1][i]);
    Serial.print(",");
    Serial.println(largestDrift[2][i]);
    Serial.print("Where the average positive drift was: ");
    Serial.print( averageOfAveragePosNegDrifts[0][i][1]);
    Serial.print(",");
    Serial.print( averageOfAveragePosNegDrifts[1][i][1]);
    Serial.print(",");
    Serial.println( averageOfAveragePosNegDrifts[2][i][1]);
    Serial.print("And the average negative drift was: ");
    Serial.print( averageOfAveragePosNegDrifts[0][i][0]);
    Serial.print(",");
    Serial.print( averageOfAveragePosNegDrifts[1][i][0]);
    Serial.print(",");
    Serial.println( averageOfAveragePosNegDrifts[2][i][0]);
  }
}
/*
The average of the largest absolute drift in 6 simulations with a 0.00 second delay was:
0.05,-0.06,-1.50
Where the average positive drift was: 0.00,0.00,0.03
And the average negative drift was: -0.00,-0.00,-0.01

The average of the largest absolute drift in 6 simulations with a 0.50 second delay was:
-0.05,0.06,0.58
Where the average positive drift was: 0.00,0.00,0.02
And the average negative drift was: -0.00,-0.00,-0.03

The average of the largest absolute drift in 6 simulations with a 1.00 second delay was:
0.05,0.06,-1.09
Where the average positive drift was: 0.00,0.00,0.03
And the average negative drift was: -0.00,-0.00,-0.04

The average of the largest absolute drift in 6 simulations with a 1.50 second delay was:
-0.05,-0.06,0.48
Where the average positive drift was: 0.00,0.00,0.00
And the average negative drift was: -0.00,-0.00,-0.00

The average of the largest absolute drift in 6 simulations with a 2.00 second delay was:
0.05,0.06,0.02
Where the average positive drift was: 0.00,0.00,0.01
And the average negative drift was: -0.00,-0.00,-0.01

The average of the largest absolute drift in 6 simulations with a 2.50 second delay was:
-0.16,0.16,0.87
Where the average positive drift was: 0.00,0.00,0.01
And the average negative drift was: -0.00,-0.00,-0.00

The average of the largest absolute drift in 6 simulations with a 3.00 second delay was:
-0.06,-1.50,-0.21
Where the average positive drift was: 0.00,0.00,0.00
And the average negative drift was: -0.00,-0.00,-0.00

The average of the largest absolute drift in 6 simulations with a 3.50 second delay was:
0.06,0.58,0.10
Where the average positive drift was: 0.00,0.00,0.01
And the average negative drift was: -0.00,-0.00,-0.00

The average of the largest absolute drift in 6 simulations with a 4.00 second delay was:
0.06,-1.09,48.09
Where the average positive drift was: 0.00,0.00,0.00
And the average negative drift was: -0.00,-0.00,-0.00

The average of the largest absolute drift in 6 simulations with a 4.50 second delay was:
-0.06,0.48,0.01
Where the average positive drift was: 0.00,0.00,0.00
And the average negative drift was: -0.00,-0.00,-0.00
*/
 */


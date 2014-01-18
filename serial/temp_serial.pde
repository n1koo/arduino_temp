// **********************************************************************
// Originally done by: Kaje@iki.fi 2010
// Seedstudio NTC temp sensors
// R(25C)=10k B(25C/50C)=3950
// 1460 resistor analog-gnd
// 10n capacitor to ground
// NTC analog-3.3V
// **********************************************************************
#include <math.h>

double temperature(int channel){
  int voltagelevel = 0;
  double R=0;
  voltagelevel = analogRead(channel);
  R=(1460/(voltagelevel*0.0010742/3.3))-1460;
  
  //Voltage level fixes
  if (voltagelevel < 390) {
    return((3930/(log(double(R/0.017632))))-273.15);
  }
  return((3955/(log(double(R/0.017632))))-273.15);
}  
  

void setup()
{
  Serial.begin(9600);
  analogReference(INTERNAL);

  //Warm-up (first temp usually corrupted)
  temperature(0);
}

void loop()
{
  Serial.print("temp1: ");
  Serial.println(temperature(0),1);
  Serial.print("temp2: ");
  Serial.println(temperature(1),1);
  Serial.print("temp3: ");
  Serial.println(temperature(2),1);
  delay(1000);  
}

#define VBATPIN A13
#include <EEPROM.h>

int pey;



void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
//  EEPROM.begin(1);
//  EEPROM.write(0, 50);
//  pey = EEPROM.read(0);

  
}

// the loop function runs over and over again forever
void loop() {
//  float measuredvbat = analogRead(VBATPIN);
//  measuredvbat *= 2.0;    // we divided by 2, so multiply back
//
//  measuredvbat /= 1150.0; // convert to voltage
//
//  float percentage = (measuredvbat-3.2)*100L/(4.2-3.2);
//  Serial.print("VBat: " ); Serial.print(measuredvbat);Serial.print(" ,percentage: " ); Serial.println(percentage);

  
//  Serial.println(pey);
  get_batt_level(VBATPIN);


  
  delay(2000);                       // wait for a second
}

void get_batt_level(int VBATPIN){
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2.0;    // we divided by 2, so multiply back
  measuredvbat /= 1150.0; // convert to voltage
  float percentage = (measuredvbat-3.2)*100L/(4.2-3.2);
  Serial.print("VBat: " ); Serial.print(measuredvbat);Serial.print(" ,percentage: " ); Serial.println(percentage);

  
}

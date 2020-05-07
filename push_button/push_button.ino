/*
 * One Button two button red button blue button
 * 
 * learnelectronics
 * 23 Sept 2017
 * 
 * www.youtube.com/c/learnelectronics
 * arduino0169@gmail.com
 * 
 * Find the library here: http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
 */

#include "OneButton.h"                              //we need the OneButton library

 
OneButton button(21, true);                         //attach a button on pin A1 to the library



void setup() {
  
  Serial.begin(9600); // open the serial port at 9600 bps:
  pinMode(13, OUTPUT);                              // sets the digital pin as output
  pinMode(12, OUTPUT);                              // sets the digital pin as output
  pinMode(11, OUTPUT);                              // sets the digital pin as output
  
     
  button.attachDoubleClick(doubleclick);            // link the function to be called on a doubleclick event.
  button.attachClick(singleclick);                  // link the function to be called on a singleclick event.
  button.attachLongPressStop(longclick);            // link the function to be called on a longpress event.
} 
  


void loop() {
  
  button.tick();                                    // check the status of the button


  delay(10);                                        // a short wait between checking the button
} // loop



void doubleclick() {                                // what happens when button is double-clicked
// digitalWrite(11,HIGH);                             // light the green LED
// delay(1000);                                       // wait one second
// digitalWrite(11,LOW);                              // turn off green LED
  Serial.println("double click detected");

} 

void singleclick(){                                 // what happens when the button is clicked
//  digitalWrite(13,HIGH);                            // light the red LED
// delay(1000);                                       // wait one second
// digitalWrite(13,LOW);                              // turn off the gren led
  Serial.println("single click detected");
}

void longclick(){                                   // what happens when buton is long-pressed
//  digitalWrite(13,HIGH);                            // light the blue LED
// delay(1000);                                       // wait one second
// digitalWrite(13,LOW);                              // turn off the blue LED
 Serial.println("long click detected");
}

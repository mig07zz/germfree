/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - ESP32 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "VYLmYDLkLw46zqLaeZbONH5L-XMGhCpR";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "knight-hawk";
char pass[] = "7758338D41PEYMAN";

//// Set your LED and physical button pins here
//const int ledPin = 13;
//const int btnPin = 8;
//
//BlynkTimer timer;
//void checkPhysicalButton();
//
//int ledState = LOW;
//int btnState = HIGH;
//
//// Every time we connect to the cloud...
//BLYNK_CONNECTED() {
//  // Request the latest state from the server
//  Blynk.syncVirtual(V2);
//
//  // Alternatively, you could override server state using:
//  //Blynk.virtualWrite(V2, ledState);
//}
//
//// When App button is pushed - switch the state
//BLYNK_WRITE(V2) {
//  ledState = param.asInt();
//  digitalWrite(ledPin, ledState);
//}
//
//void checkPhysicalButton()
//{
//  if (digitalRead(btnPin) == LOW) {
//    // btnState is used to avoid sequential toggles
//    if (btnState != LOW) {
//
//      // Toggle LED state
//      ledState = !ledState;
//      digitalWrite(ledPin, ledState);
//
//      // Update Button Widget
//      Blynk.virtualWrite(V2, ledState);
//    }
//    btnState = LOW;
//  } else {
//    btnState = HIGH;
//  }
//}
//
//void setup()
//{
//  // Debug console
//  Serial.begin(9600);
//
//  Blynk.begin(auth, ssid, pass);
//  // You can also specify server:
//  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
//  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
//
//  pinMode(ledPin, OUTPUT);
//  pinMode(btnPin, INPUT_PULLUP);
//  digitalWrite(ledPin, ledState);
//
//  // Setup a function to be called every 100 ms
//  timer.setInterval(100L, checkPhysicalButton);
//}
//
//void loop()
//{
//  Blynk.run();
//  timer.run();
//}

#define PIN 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

BLYNK_WRITE(V1)
{
  int shift = param.asInt();
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, Wheel(shift & 255));
    // OR: strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + shift) & 255));
  }
  strip.show();
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  strip.begin();
  strip.show();
}

void loop()
{
  Blynk.run();
}

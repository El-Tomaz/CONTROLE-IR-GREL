#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>

//mapa de hardware
#define kIrLed 0

#define bUp 2
#define bDown 3
#define bOnOff 1


IRMideaAC ac(kIrLed);

void setup() {
  Serial.begin(115200);
  //todo: webserver
  ac.begin();
/*
  pinMode(bUp, INPUT);
  pinMode(bDown, INPUT);
  pinMode(bOnOff, INPUT);
*/
}


void loop() {
  Serial.println("Funfando!");

  static bool ligado = false;

  // Set up what we want to send. See ir_Daikin.cpp for all the options.
  ac.on();
 
  // Now send the IR signal.
  ac.send();

  delay(5000);




  // Set up what we want to send. See ir_Daikin.cpp for all the options.
  ac.off();
 
  // Now send the IR signal.
  ac.send();

  delay(5000);
  ligado  = !ligado;
}
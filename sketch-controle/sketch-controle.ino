#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>

//mapa de hardware 
#define kIrLed 3

#define bUp 
#define bDown
#define bOnOff


IRMideaAC ac(kIrLed);  

void setup() {

  //todo: webserver
  ac.begin();

  pinMode(bUp, INPUT);
  pinMode(bDown, INPUT);
  pinMode(bOnOff, INPUT);

}


void loop() {
static bool ligado = false;

if(

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



}
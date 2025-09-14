/*
 * SPDX-FileCopyrightText: 2024 Volodymyr Shymanskyy for Blynk Technologies Inc.
 * SPDX-License-Identifier: Apache-2.0
 *
 * The software is provided "as is", without any warranties or guarantees (explicit or implied).
 * This includes no assurances about being fit for any specific purpose.
 */

#include <PubSubClient.h>
#include "NetworkHelpers.h"
#include <WiFiManager.h>


// ========= HARDWARE MAPPING! ===========
const int pButtons[3] = {0,4,5}; //on/off up down
// ATENCAO: os numeros dos pinos na placa esta diferente, conecte fisicamente aos pinos: 3 2 1

// ========= systems constraints
#define MAX_TEMP 24
#define MIN_TEMP 18



// These commands are executed every time
// the device (re)connects to the Blynk Cloud
void mqtt_connected()
{
  // Publish some data
  mqtt.publish("ds/terminal", "Device connected\n");
}

// Handle incoming datastream changes
void mqtt_handler(const String& topic, const String& value)
{
  Serial.print("Got ");       Serial.print(topic);
  Serial.print(", value: ");  Serial.println(value);

  if (topic == "downlink/ds/terminal") {
    String reply = String("Your command: ") + value;
    mqtt.publish("ds/terminal", reply.c_str());
  }
}

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);


  Serial.begin(115200);

  WiFiManager wm;

  bool res = wm.autoConnect("Controle_GREL", "Uma onca calma serena");

  if(!res){
    Serial.println("Falha na conexao :(");
  }else{
    Serial.println("Conectado com sucesso!!");
  }



  // Wait for serial monitor, up to 3 seconds
  while (!Serial && (millis() < 3000)) { delay(10); }
  delay(100);

  systemShowDeviceInfo();
  setClock();

  
  for(int i = 0; i < 3; i++)pinMode(pButtons[i], INPUT_PULLUP);

}



void loop()
{
  static struct {
    int temp = 20;
    bool on = true;
  } status;

  bool button_pressed = false; //flag to just send commando to ac and to blynk when button pressed
  static int last_button_state[3] = {1}; // on/off up down
  int actual_button_state[3] = {1};

  for(int i = 0; i < 3;i++){
    actual_button_state[i] = digitalRead(pButtons[i]);
    
    //detectando borda de descida
    if(last_button_state[i] == 1 && actual_button_state[i] == 0){
        
        button_pressed = true;

        switch(i){
          // on/off
          case 0:
            status.on = !status.on;
          break;

          // up
          case 1:
            status.temp++;
          break;

          //down
          case 2:
            status.temp--;
          break;

        }
    }

    last_button_state[i] = actual_button_state[i];  
  }

  // tratando dados (evitar limite de temperatura)
  if(status.temp > MAX_TEMP){
    status.temp = MAX_TEMP;
  }
  else if(status.temp < MIN_TEMP){
    status.temp = MIN_TEMP;
  }

  //enviando commando 
  if(button_pressed){
    Serial.println("BUtton pressed!");
    Serial.println(status.temp);
    Serial.println(status.on);
    // send commando to ac
  }

  // sending info to blynk
  if(button_pressed){
    char temp_payload[3] = "00";
    sprintf(temp_payload, "%d", status.temp);
    mqtt.publish("ds/temp", temp_payload);

    char on_off[2] = "";
    on_off[0] = status.on == true? '1':'0';
    mqtt.publish("ds/on", on_off);

  }

  // Keep WiFi and MQTT connection
  if (!mqtt.connected()) {
    connectMQTT();
  } else {
    bool result = mqtt.loop();
  }

  delay(10);


}

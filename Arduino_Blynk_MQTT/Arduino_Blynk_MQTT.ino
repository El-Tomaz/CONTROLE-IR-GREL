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
}

void loop()
{


  // Keep WiFi and MQTT connection
  

  if (!mqtt.connected()) {
    connectMQTT();
  } else {
    bool result = mqtt.loop();
  }

  delay(10);


}

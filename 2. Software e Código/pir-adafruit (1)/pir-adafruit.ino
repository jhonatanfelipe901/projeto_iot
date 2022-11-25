/*
   Jhonatan Felipe do Nascimento - 10922503358, 5 Semestre.
   Projeto SensorKx: Sensor de Sala
*/

#include "EspMQTTClient.h"

EspMQTTClient client(
  "Wokwi-GUEST",         // SSID WiFi
  "",                    // Password WiFi
  //"test.mosquitto.org",  // MQTT Broker
  "broker.hivemq.com",  // MQTT Broker
  "mqtt-mack",          // Client
  1883                   // MQTT port
);

int ledPin = 12;                // choose the pin for the LED
int inputPin = 14;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

void setup() {

  Serial.begin(115200);

  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "Vou ficar offline");

  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
}

void loop() {
  client.loop(); // Run in loop

  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON (Atuador)
    if (pirState == LOW) {
      //turned ON
      Serial.println("Movimento no recinto detectado!");
      client.publish("mackenzieei10922503358/temperatura", "Movimento no recinto detectado!"); 

      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH) {
      //turned OFF
      Serial.println("Movimento no recinto finalizado!");
      client.publish("mackenzieei10922503358/temperatura", "Movimento no recinto finalizado!"); 

      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

/**
 * @brief This function is called when everything is connected (Wifi and MQTT),
 * to use it, you must implement the struct EspMQTTClient
 */
void onConnectionEstablished()
{
  //Subscribe
  client.subscribe("mack10922503358/msgRecebida/#", [](const String & topic, const String & payload) {
  Serial.println("the following message was received in the topic: " + topic + ", with the payload: " + payload);
  });
}

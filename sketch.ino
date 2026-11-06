
#include <WiFi.h>
#include <PubSubClient.h>

#include "driver/gpio.h"
#include "soc/gpio_reg.h"
#include "soc/soc.h"

// WiFi credentials for Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT Broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// MQTT Topic
const char* mqtt_topic = "cse08/smarthome/command";

// GPIO Pins
#define BUTTON1 34
#define BUTTON2 27

#define LED1 25
#define LED2 26

WiFiClient espClient;
PubSubClient client(espClient);

// LED states
bool led1State = false;
bool led2State = false;

// Button debounce variables
int lastButton1 = HIGH;
int lastButton2 = HIGH;

int stableButton1 = HIGH;
int stableButton2 = HIGH;

unsigned long debounceTime1 = 0;
unsigned long debounceTime2 = 0;

const unsigned long debounceDelay = 50;

// Read buttons directly from GPIO registers
void readButtons(bool &button1, bool &button2) {

  uint32_t gpio0 = REG_READ(GPIO_IN_REG);
  uint32_t gpio1 = REG_READ(GPIO_IN1_REG);

  // GPIO34 = bit 2 of GPIO_IN1_REG
  button1 = (gpio1 >> 2) & 1;

  // GPIO27 = bit 27 of GPIO_IN_REG
  button2 = (gpio0 >> 27) & 1;
}

// Update LEDs
void updateLEDs() {

  digitalWrite(LED1, led1State);
  digitalWrite(LED2, led2State);
}

// MQTT callback
void callback(char* topic, byte* payload,
              unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  message.trim();
  message.toUpperCase();

  if (message == "ON") {

    led1State = true;
    led2State = true;

    Serial.println("Both LEDs ON");

  }

  else if (message == "OFF") {

    led1State = false;
    led2State = false;

    Serial.println("Both LEDs OFF");

  }

  updateLEDs();
}

// WiFi connection
void connectWiFi() {

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());
}

// MQTT connection
void reconnectMQTT() {

  while (!client.connected()) {

    Serial.print("Connecting to MQTT...");

    String clientId = "ESP32-SmartHome-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("Connected!");

      client.subscribe(mqtt_topic);

    }

    else {

      Serial.print("Failed, state = ");
      Serial.println(client.state());

      delay(2000);

    }
  }
}

void setup() {

  Serial.begin(115200);

  // Button configuration
  pinMode(BUTTON1, INPUT); 
  pinMode(BUTTON2, INPUT_PULLUP);

  // LED configuration
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  updateLEDs();

  // Connect WiFi
  connectWiFi();

  // MQTT setup
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();

  bool button1, button2;

  // Direct GPIO register reading
  readButtons(button1, button2);

  unsigned long currentTime = millis();

  // Button 1 debounce
  if (button1 != lastButton1) {
    debounceTime1 = currentTime;
    lastButton1 = button1;
  }

  if ((currentTime - debounceTime1) > debounceDelay) {

    if (button1 != stableButton1) {

      stableButton1 = button1;

      if (stableButton1 == LOW) {

        led1State = !led1State;

        Serial.println("Button 1 Pressed");

        updateLEDs();
      }
    }
  }

  // Button 2 debounce
  if (button2 != lastButton2) {
    debounceTime2 = currentTime;
    lastButton2 = button2;
  }

  if ((currentTime - debounceTime2) > debounceDelay) {

    if (button2 != stableButton2) {

      stableButton2 = button2;

      if (stableButton2 == LOW) {

        led2State = !led2State;

        Serial.println("Button 2 Pressed");

        updateLEDs();
      }
    }
  }

}

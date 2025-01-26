#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 4        
#define DHTTYPE DHT11   

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

const char* mqtt_broker = "Your_VM_IP_Address";
const int mqtt_port = 1883;
const char* mqtt_topic = "temperature";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  client.setServer(mqtt_broker, mqtt_port);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    while (!client.connect("ArduinoClient")) {
      Serial.println("Connecting to MQTT...");
      delay(1000);
    }
    Serial.println("Connected to MQTT");
  }
  
  float temperature = dht.readTemperature();
  if (!isnan(temperature)) {
    String payload = String(temperature);
    client.publish(mqtt_topic, payload.c_str());
    Serial.println("Published: " + payload);
  }
  delay(5000);
}

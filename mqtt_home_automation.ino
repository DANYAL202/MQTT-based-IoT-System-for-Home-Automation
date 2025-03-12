#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* mqttServer = "mqtt.eclipse.org";  // MQTT broker server
const int mqttPort = 1883;

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

  client.setServer(mqttServer, mqttPort);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    char tempStr[8];
    dtostrf(t, 1, 2, tempStr);  // Convert temperature to string
    client.publish("home/temperature", tempStr);
  }

  delay(5000);  // Send temperature data every 5 seconds
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(5000);
    }
  }
}

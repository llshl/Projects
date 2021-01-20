#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

const char* ssid = "AndroidHotspot8568";
const char* password= "10239160";
const char* mqttServer = "54.198.221.28";
const int   mqttPort = 1883;

DHTesp dht;
int interval = 4000;
int light = 0;
char charTemperature[7];
char charHumidity[7];
char charLight[7];
float humidity = 0;
float temperature = 0;
unsigned long lastPublished = - interval;
unsigned long lastDHTReadmillis = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.setup(14,DHTesp::DHT22);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
//  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");

    if(client.connect("ESP8266Client boardA")){
      Serial.println("connected");
    }else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
 // client.subscribe("#");
}

void loop() {
  client.loop();

  unsigned long currentMillis = millis();
  if(currentMillis - lastPublished >= interval) {
    lastPublished = currentMillis;
    light = analogRead(0);
    dtostrf(light, 3, 1, charLight);
    readDHT22();
    delay(50);

    client.publish("heum/2016146044/evt/temperature", charTemperature);
    client.publish("heum/2016146044/evt/humidity", charHumidity);
    client.publish("heum/2016146044/evt/light", charLight);
  }
}

void readDHT22(){
    unsigned long currentMillis = millis();
    if(currentMillis - lastDHTReadmillis >= interval){
        lastDHTReadmillis = currentMillis;
        humidity = dht.getHumidity();
        temperature = dht.getTemperature();
        dtostrf(temperature, 3, 1, charTemperature);
        dtostrf(humidity, 3, 1, charHumidity);
    }
}

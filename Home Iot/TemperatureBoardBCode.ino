#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

char command[10];
const char* ssid = "AndroidHotspot0424";
const char* password = "1q2w3e4r";
const char* ssid1 = "AndroidHotspot8568";
const char* password1 = "10239160";
const char* ssid2 = "Galaxy s10+";
const char* password2 = "19970719";
const char* mqttServer = "54.198.221.28";
const int   mqttPort = 1883;
int interval = 2500;
int motor = 4;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(motor, OUTPUT);
  WiFi.begin(ssid1, password1);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");

    if(client.connect("ESP8266Client boardB")){
      Serial.println("connected");
    }else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  if(client.subscribe("heum/2016146044/evt/temperature")) {
    Serial.println("Subscription Valid !");
  }
}

void loop() {
  client.loop();
  delay(50);
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i=0 ; i<length ; i++){
    Serial.print((char)payload[i]);
    command[i] = (char)payload[i];
  } 
  Serial.println("\n--------------------");

  float temp = atoi(command);
  if(temp >= 26){
    Serial.printf("HIGH");
    digitalWrite(motor, LOW);
    delay(3000);
  }
  else{
    Serial.printf("LOW");
    digitalWrite(motor, HIGH);
    delay(3000);
  }
}

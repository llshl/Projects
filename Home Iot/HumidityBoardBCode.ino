#include <ESP8266WiFi.h>
#include <PubSubClient.h>+

#include <string.h>

char command[10];
const char* ssid = "AndroidHotspot8568";
const char* password= "10239160";
const char* mqttServer = "54.198.221.28";
const short int relay = 15;
const int   mqttPort = 1883;
int interval = 5000;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  WiFi.begin(ssid, password);
  
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
  if(client.subscribe("heum/2016146044/evt/humidity")) {
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
  
  int result = atoi(command);     
  if(result <=50){
    digitalWrite(relay, HIGH);
  }
  else{
    digitalWrite(relay, LOW);
  }
}

  /*if(strncmp(command, "relayOn", 7) ==0){
    digitalWrite(relay, HIGH);
  }
  else{
    digitalWrite(relay, LOW);
  }
}*/

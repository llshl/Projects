#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Adafruit_NeoPixel.h>
#include <string.h>
//#define ledPin 15
//#define ledNum 12

  int a;
  int b;
  int c;
  float i=0;
  unsigned long pre_Time = 0;
  unsigned long currentMillis = 0;

  
char command[23];
const char* ssid = "AndroidHotspot8568";
const char* password= "10239160";
const char* mqttServer = "54.198.221.28";
const int   mqttPort = 1883;
//unsigned long pre_Time = 0;


WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel pixels;
void setup() {
  Serial.begin(115200);
  pixels=Adafruit_NeoPixel(12,15,NEO_GRB+NEO_KHZ800);
  pixels.begin();
  ESP.eraseConfig();
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
  if(client.subscribe("heum/2016146044/evt/light")) {
    Serial.println("Subscription Valid !");
  }
}

void loop() {
//  ESP.wdtDisable();
  client.loop();
  delay(50);
//  pixels.show();
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
  int light = atoi(command);


  if(light<=10){
   
      pixels.setPixelColor(0, pixels.Color(255,100,150));
      pixels.setPixelColor(1, pixels.Color(255,100,150));
      pixels.setPixelColor(2, pixels.Color(255,100,150));
      pixels.setPixelColor(3, pixels.Color(255,100,150));
      pixels.setPixelColor(4, pixels.Color(255,100,150));
      pixels.setPixelColor(5, pixels.Color(255,100,150));
      pixels.setPixelColor(6, pixels.Color(255,100,150));
      pixels.setPixelColor(7, pixels.Color(255,100,150));
      pixels.setPixelColor(8, pixels.Color(255,100,150));
      pixels.setPixelColor(9, pixels.Color(255,100,150));
      pixels.setPixelColor(10, pixels.Color(255,100,150));
      pixels.setPixelColor(11, pixels.Color(255,100,150));
   
    pixels.show();
    Serial.print("On Msg rcv");
  }
  else{
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.setPixelColor(5, pixels.Color(0,0,0));
    pixels.setPixelColor(6, pixels.Color(0,0,0));
    pixels.setPixelColor(7, pixels.Color(0,0,0));
    pixels.setPixelColor(8, pixels.Color(0,0,0));
    pixels.setPixelColor(9, pixels.Color(0,0,0));
    pixels.setPixelColor(10, pixels.Color(0,0,0));
    pixels.setPixelColor(11, pixels.Color(0,0,0));
    pixels.show();
    Serial.print("Off Msg rcv");
  }
}

/*
    Program Alarm PIR dengan Telegram
    dibuat oleh Indobot
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "CTBot.h"  //Pendeklarasian Library
CTBot myBot;

const char* ssid = "Universitas Mulawarman";
const char* password = "";
String token = "6115141412:AAFNjoX6lor_O8cd_FwNQ9GwOKQvDdcVLRc";    
const char* mqtt_server = "broker.hivemq.com";
const int id = 1013940006;      //id telegram kalian

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Pemilihan Pin dan pendeklarasian variabel
#define pirPin D1 
int pirValue; 

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("AlarmPIR");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup(){  //Pengaturan Pin dan Variabel
  Serial.begin(9600);
  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, password);
  myBot.setTelegramToken(token);
  if (myBot.testConnection()) {
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Jelek");
  }
//  pinMode(buzzerPin, OUTPUT);
  pinMode(pirPin, INPUT);
//  digitalWrite(buzzerPin, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
}

void loop(){ //Perulangan Program
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  pirValue = digitalRead(pirPin);
  myBot.sendMessage(id, "Gerakan terdeteksi");
  delay(2000);
  snprintf (msg, MSG_BUFFER_SIZE, "%s", itoa(pirValue,msg,10)); // itoa (konversi integer ke string)
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("AlarmPIR", msg); // fungsi untuk publish ke broker
}

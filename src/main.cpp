//!--------------------------------------------------------------------------
//! Inclusões de bibliotecas
//? Bibliotecas já instaladas no framework PlatformIO
#include <Arduino.h>
#include <WiFiClientSecure.h>

//? Bibliotecas baixadas do GitHub e adicionadas ao projeto
#include <PubSubClient.h>

//!--------------------------------------------------------------------------
//! Cabeçalhos de funções

void connectToWiFi();
void connectToMQTT();
void callback(char *topic, byte *payload, unsigned int length);

//!--------------------------------------------------------------------------
//! Constantes

WiFiClientSecure WiFiClient;
PubSubClient mqttClient(WiFiClient);

//!--------------------------------------------------------------------------
//! Main Loops

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  connectToMQTT();
}

void loop() {}

//!--------------------------------------------------------------------------
//! Funções

void connectToWiFi() {
  WiFi.begin("SSID", "SENHA");
  Serial.print("Conectando ao WiFi...");
  while (!WiFi.isConnected()) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.isConnected()) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Falha ao conectar ao WiFi!");
  }
}

void connectToMQTT() {
  mqttClient.setServer("broker.hivemq.com", 8883);

  while (!mqttClient.connected()) {
    Serial.print("Conectando ao Broker MQTT...");
    if (mqttClient.connect("ID_Unico")) {
      mqttClient.subscribe("topico/teste");
      mqttClient.setCallback(callback);

      Serial.println("Conectado ao Broker MQTT");
      Serial.println("Inscrito no tópico: topico/teste");
    } else {
      Serial.println("Falha ao conectar ao Broker MQTT");
      Serial.print("Erro: ");
      Serial.println(mqttClient.state());
      delay(2000);
      Serial.println("Tentando novamente...");
    }
  }
}
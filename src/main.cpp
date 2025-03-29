

//!---------------------       Inclusões de bibliotecas ---------------------

//? Bibliotecas já instaladas no framework PlatformIO
#include <Arduino.h>
#include <WiFiClientSecure.h>

//? Bibliotecas baixadas do GitHub e adicionadas ao projeto
#include <PubSubClient.h>

//!---------------------       Cabeçalho de Funções        ---------------------

void connectToWiFi();
void connectToMQTT();
void callback(char *topic, byte *payload, int length);

//!---------------------       Definições de Constantes ---------------------

WiFiClientSecure WiFiClient;
PubSubClient mqttClient(WiFiClient);

const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

const char* broker = "BROKER";
const int port = 8883;

const char* board_ID = "ID_UNICO_DA_PLACA";
const char* mqtt_user = "SEU_USUARIO";
const char* mqtt_password = "SUA_SENHA";
const char* topic = "Topico";

//!---------------------       Loops Principais        ---------------------

void setup() {
  Serial.begin(115200);

  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!WiFi.isConnected()) {
    Serial.println("Conexão com WiFi perdida!");
    connectToWiFi();
  }
  if (WiFi.isConnected() && !mqttClient.connected()) {
    Serial.println("Conexão com Broker MQTT perdida!");
    connectToMQTT();
  } else {
    String mensagem = "algumaCoisa";
    mqttClient.publish("Topico", mensagem.c_str());
  }
}

//!---------------------       Funções        ---------------------

void connectToWiFi() {
  WiFi.begin(ssid, password);
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
  mqttClient.setServer(broker, port);

  while (!mqttClient.connected()) {
    Serial.print("Conectando ao Broker MQTT...");
    if (mqttClient.connect(board_ID, mqtt_user, mqtt_password)) {
      mqttClient.subscribe(topic);
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

void callback(char *subscribedTopic, byte *payload, int length) {
  String mensagem;
  String topicStr = subscribedTopic;

  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    mensagem += c;
  }
  if (mensagem == "algumaCoisa") {
    //* Executa um comando
  }
}
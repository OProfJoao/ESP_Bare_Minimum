# Guia para Montar o Código do `main.cpp`

Este guia foi criado para ajudar os estudantes do curso técnico de Desenvolvimento de Sistemas, na matéria de IoT, a montar o código do arquivo `main.cpp` de forma organizada e intuitiva.

## 🛠️ Pré-requisitos
- Framework PlatformIO instalado
- Biblioteca PubSubClient (via PlatformIO Libraries)
- ESP32 configurado no PlatformIO


## 📋Passo a Passo

### 1. **Configuração Inicial**
- **Objetivo**: Configurar o ambiente básico do projeto.
- **O que fazer**:
    - Inclua as bibliotecas necessárias, como `<WiFi.h>` e `<PubSubClient.h>`.
    ```cpp
    //Bibliotecas já instaladas no framework PlatformIO
    #include <Arduino.h>
    #include <WiFiClientSecure.h>

    //Bibliotecas baixadas do GitHub e adicionadas ao projeto
    #include <PubSubClient.h>
    ```
    

### 2. **Configuração do Cliente MQTT**
- **Objetivo**: Preparar a comunicação com o broker MQTT.
- **O que fazer**:
    - Defina as credenciais da rede Wi-Fi:
    ```cpp
    const char* ssid = "SEU_SSID";
    const char* password = "SUA_SENHA";
    ```
    - Defina as credenciais do broker MQTT e a porta:
    ```cpp
    const char* broker = "BROKER";
    const int port = 8883;

    const char* board_ID = "ID_UNICO_DA_PLACA";
    const char* mqtt_user = "SEU_USUARIO";
    const char* mqtt_password = "SUA_SENHA";
    const char* topic = "Topico";
    ```
    - Crie objetos para o cliente Wi-Fi e MQTT:
    ```cpp
    WiFiClient espClient;
    PubSubClient client(espClient);
    ```
### 3. **Função `setup()`**
- **Objetivo**: Inicializar o dispositivo.
- **O que fazer**:
    - Configure a comunicação serial e chame as funções de inicialização:
    ```cpp
    void setup() {
        Serial.begin(115200);

        connectToWiFi();
        connectToMQTT();
    }
    ```

### 4. **Função de Conexão Wi-Fi**
- **Objetivo**: Garantir que o dispositivo se conecte à rede Wi-Fi.
- **O que fazer**:
    - Crie uma função chamada `connectToWiFi()`:
    ```cpp
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
    ```
### 5. **Função de Reconexão MQTT**
- **Objetivo**: Reconectar ao broker MQTT caso a conexão seja perdida.
- **O que fazer**:
    - Crie uma função `reconnect()`:
    ```cpp
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
    ```

### 6. **Função de Callback MQTT**
- **Objetivo**: Processar mensagens recebidas do broker MQTT.
- **O que fazer**:
    - Crie uma função `callback()` para lidar com mensagens:
    ```cpp
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
    ```

### 7. **Criar cabeçalhos de função no início do arquivo (após definições)**
- **Objetivo**: Permitir que as funções criadas possam ser acessadas de qualquer parte do código.
- **O que fazer**:
    - Copie as declarações das funções criadas 
        ```cpp 
        void callback(char *subscribedTopic, byte *payload, int length);
        void connectToMQTT();
        void connectToWiFi();
        ```
    - Cole-as logo abaixo das importações de bibliotecas com `;` ao final

### 8. **Função `loop()`**
- **Objetivo**: Manter o dispositivo em funcionamento contínuo.
- **O que fazer**:
    - Verifique a conexão MQTT e processe mensagens:
    ```cpp
    void loop() {
        if (!WiFi.isConnected()) {
            Serial.println("Conexão com WiFi perdida!");
            connectToWiFi();
        }
        if (WiFi.isConnected() && !mqttClient.connected()) {
            Serial.println("Conexão com Broker MQTT perdida!");
            connectToMQTT();
        } else {
            mqttClient.loop();

            String mensagem = "algumaCoisa";
            mqttClient.publish("Topico", mensagem.c_str());
        }
    }
    ```

### 9. **Testes**
- **Objetivo**: Garantir que o código funcione corretamente.

- **O que fazer**:
    - Compile e envie o código para o ESP32.
    - Verifique a saída no monitor serial para confirmar a conexão Wi-Fi e MQTT.

### 🔐 **Boas práticas de segurança**
Em códigos de produção, é estritamente proibido _commitar_ informações sensíveis (como senhas e chaves de acesso) para repositórios públicos, como o GitHub.

#### O que fazer
Para proteger essas informações, siga os seguintes passos:

1. **Crie um arquivo de configuração**

    Crie um arquivo chamado `secrets.h` ou `env.h`

2. **Adicione o arquivo ao `.gitignore`** 

    Inclua o nome deste arquivo (`secrets.h` ou `env.h`) no arquivo `.gitignore`

3. **Inclua o arquivo de configuração no seu código**

    Importe este arquivo no seu código 
```cpp
//Bibliotecas já instaladas no framework PlatformIO
#include <Arduino.h>
#include <WiFiClientSecure.h>

//Bibliotecas baixadas do GitHub e adicionadas ao projeto
#include <PubSubClient.h>

//Arquivo que contém as chaves de acesso
#include "nomeDoArquivo.h"
```
4. **Defina as credenciais dentro do arquivo de configuração**

    Utilize _include guards_ para evitar múltiplas inclusões e defina suas chaves **sem utilizar ponto-e-vírgula** (;):
```cpp
//Cria as definições caso não existam
#ifndef SECRETS_H
#define SECRETS_H
//       ou
//  #ifndef ENV_H
//  #define ENV_H

//Define os valores
#define SSID "SEU_WIFI"
#define PASSWORD "SUA_SENHA"

#define BROKER "BROKER"
#define PORT 8883

#define MQTT_CLIENT_ID "ID_UNICO_DA_PLACA"
#define MQTT_USER "SEU_USUARIO"
#define MQTT_PASSWORD "SUA_SENHA"

#endif
```
5. **Utilize as variáveis no seu código**

    No código você utilizara as definições importadas, por exemplo:

```cpp
void setup() {
    WiFi.begin(SSID, PASSWORD);  // Acesso seguro às credenciais
    
    // Configuração MQTT segura
    client.setServer(BROKER, PORT);
    client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);
}
```


Com este guia, você terá um código funcional, seguro e bem estruturado para o seu projeto IoT. Boa sorte!
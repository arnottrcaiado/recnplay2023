//
// Projeto Rec N Play 2023
// Desfile Maker 2
//
// Codigo para ler dados de API e mostrar na tela
//
// Data: 09/10/2023


#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h> // Inclua esta biblioteca


ESP8266WiFiMulti WiFiMulti;

// Struct para armazenar informações de rede
struct NetworkConfig {
  const char* ssid;
  const char* password;
};

// Lista de redes pré-cadastradas
NetworkConfig networks[] = {
  {"AP1501", "ARBBBE11"},
  {"Vivo-Internet-E532", "Vivo-Internet-E532"},
  {"iPhone de Arnott", "arbbbe11"},
  {"Vivo-Internet-E532", "Vivo-Internet-E532"},
  {"SENAC-Mesh", "09080706"}
};

void connectToWiFi() {
  // Loop para tentar se conectar a cada rede na lista
  for (int i = 0; i < sizeof(networks) / sizeof(networks[0]); i++) {
    WiFiMulti.addAP(networks[i].ssid, networks[i].password);
    Serial.printf("Tentando conectar a %s...\n", networks[i].ssid);

    // Aguarda a conexão por até 10 segundos
    for (int j = 0; j < 10; j++) {
      if (WiFiMulti.run() == WL_CONNECTED) {
        Serial.printf("Conectado a %s\n", networks[i].ssid);
        return; // Conexão bem-sucedida, saia da função
      }
      delay(1000);
    }
    delay(1000);
    Serial.printf("Falha na conexão a %s\n", networks[i].ssid);
//    WiFiMulti.removeAP(networks[i].ssid); // Remove a rede da lista para tentar a próxima
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
//  WiFi.mode(WIFI_STA);
//  WiFiMulti.addAP("Vivo-Internet-E532", "Vivo-Internet-E532");
}

void loop() {
  connectToWiFi();
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");

    if (http.begin(client, "http://recnplay2023.pythonanywhere.com/resultados")) {
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);

          // Defina o tamanho máximo do documento JSON com base no tamanho esperado do JSON
          const size_t capacity = JSON_OBJECT_SIZE(3); // 3 é o número de chaves no seu JSON
          StaticJsonDocument<200> doc;

          // Parse JSON
          DeserializationError error = deserializeJson(doc, payload);

          if (!error) {
            // Acessar os valores com as chaves "vermelho", "azul" e "verde"
            int vermelho = doc["vermelho"];
            int azul = doc["azul"];
            int verde = doc["verde"];

            // Mostrar os dados separados
            Serial.printf("Vermelho: %d\n", vermelho);
            Serial.printf("Azul: %d\n", azul);
            Serial.printf("Verde: %d\n", verde);
          } else {
            Serial.println("Falha no parse JSON");
          }
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }
  }
  delay(100);
}

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

WiFiMulti WiFiMulti;

struct NetworkConfig {
  const char* ssid;
  const char* password;
};

NetworkConfig networks[] = {
  {"AP1501", "ARBBBE11"},
  {"Vivo-Internet-E532", "Vivo-Internet-E532"},
  {"iPhone de Arnott", "arbbbe11"},
  {"Vivo-Internet-E532", "Vivo-Internet-E532"},
  {"SENAC-Mesh", "09080706"}
};

void connectToWiFi() {
  for (int i = 0; i < sizeof(networks) / sizeof(networks[0]); i++) {
    WiFiMulti.addAP(networks[i].ssid, networks[i].password);
    Serial.printf("Tentando conectar a %s...\n", networks[i].ssid);

    for (int j = 0; j < 10; j++) {
      if (WiFiMulti.run() == WL_CONNECTED) {
        Serial.printf("Conectado a %s\n", networks[i].ssid);
        return;
      }
      delay(1000);
    }
    delay(1000);
    Serial.printf("Falha na conexão a %s\n", networks[i].ssid);
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

          const size_t capacity = JSON_OBJECT_SIZE(3);
          StaticJsonDocument<200> doc;

          DeserializationError error = deserializeJson(doc, payload);

          if (!error) {
            int vermelho = doc["vermelho"];
            int azul = doc["azul"];
            int verde = doc["verde"];

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

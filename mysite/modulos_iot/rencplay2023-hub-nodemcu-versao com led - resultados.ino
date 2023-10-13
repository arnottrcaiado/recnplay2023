#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <IRremoteESP8266.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN_R D3 // red
#define PIN_G D2 // green
#define PIN_B D1 // blue
#define NUM_LEDS 1


#define BT_UM     0xFF30CF
#define BT_DOIS   0xFF18E7
#define BT_TRES   0xFF7A85
#define BT_QUATRO 0xFF10EF
#define BT_CINCO  0xFF38C7
#define BT_NOVE   0xFF52AD

ESP8266WiFiMulti WiFiMulti;

IRsend irsend(4); //an IR emitter led is connected to GPIO pin 4

StaticJsonDocument<200> doc; // <- a little more than 200 bytes in the stack
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_R, NEO_GRB + NEO_KHZ800);


void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_G, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println();
  strip.begin();
  strip.show();  // Inicializa todas as cores (off)
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Vivo-Internet-E532", "6EFC366C");
}

void loop() {
  static float vermelho, azul, verde, total;
  
 
   // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://recnplay2023.pythonanywhere.com/resultados")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          http.useHTTP10(true);

          // Parse response
          deserializeJson(doc, payload);

          // Read values
          vermelho = doc["vermelho"];
          azul = doc["azul"];
          verde = doc["verde"];
          String ultimo = doc["ultimo"];
          total= vermelho + azul + verde;
          verde = (verde / total) * 100;
          azul = (azul / total) * 100;
          vermelho = (vermelho / total) * 100;

          // chamar funcao que dispara comando para os Arduinos - usando I2C
          

          Serial.println(vermelho);
          Serial.println(azul);
          Serial.println(verde);

          if ( strcmp(ultimo.c_str(),"vermelho") == 0 )
            digitalWrite( PIN_R, HIGH );

          if (strcmp(ultimo.c_str(),"azul") == 0 )
            digitalWrite( PIN_B, HIGH );

          if (strcmp(ultimo.c_str(),"verde") == 0)
            digitalWrite( PIN_G, HIGH );

          delay(1000);
          digitalWrite( PIN_R, LOW);
          digitalWrite( PIN_B, LOW);
          digitalWrite( PIN_G, LOW);
          

          // Define a cor do LED RGB
          digitalWrite( PIN_R, HIGH );
          // strip.setPixelColor(0, strip.Color(255,0,0));
          // strip.show();  // Atualiza o LED com a nova cor
          delay(vermelho * 20);
          digitalWrite( PIN_R, LOW );
          delay(vermelho *10);
          digitalWrite( PIN_G, HIGH );
          delay(verde * 20);
          digitalWrite( PIN_G, LOW );
          delay(verde * 10);
          digitalWrite( PIN_B, HIGH );
          delay(azul * 20);
          digitalWrite( PIN_B, LOW );
          delay(azul * 10);

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
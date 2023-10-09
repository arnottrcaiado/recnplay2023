// Arduino e Lilypad Controle Remoto IR - receptor
// le estado do IR e acende LEDs
// https://blog.eletrogate.com/guia-completo-do-controle-remoto-ir-receptor-ir-para-arduino
// Baseado em https://hetpro-store.com/TUTORIALES/control-ir-con-arduino/
#include <IRremote.h>                       // Biblioteca IRemote

#define REPETE    0xFFFFFFFF
#define BT_UM     0xFF30CF
#define BT_DOIS   0xFF18E7
#define BT_TRES   0xFF7A85
#define BT_QUATRO 0xFF10EF
#define BT_CINCO  0xFF38C7
#define BT_NOVE   0xFF52AD

// pesquisar quais são os pinos do Lilypad que funcionam como PWM
//
#define LED_VERMELHO 5  // porta 5 PWM
#define LED_AZUL 9      // porta 9 PWM
#define LED_VERDE 6     // porta 6 pwm
#define LED_TODOS 0

#define MAXTEMPO 70
#define MINTEMPO 5

#define RECV_PIN 4                          // Lilypar - pino 4 conectado no Receptor IR

#include <math.h>

IRrecv irrecv(RECV_PIN);                    // criando a instância
decode_results results;                     // declarando os resultados

int ledAtual = LED_VERDE;                   // cor de partida
int tempo = 20;                             // intervalo de tempo

void setup()
{
  Serial.begin(9600);                       // Setando monitor serial para velocidade 9600 bps
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  apagaTudo();
  irrecv.enableIRIn();                      // Inicializa a recepção de códigos
}



void loop()
{
  if (irrecv.decode(&results))              // se algum código for recebido
  {
    if ( results.value != REPETE ) {
      // Serial.println(results.value, HEX);     // imprime o HEX Code
      switch ( results.value ) {
        case BT_UM :                           // botao um - acende VERMELHO
          acendeVermelho();
          delay(1);
          break;

        case BT_DOIS :                      // botao dois - acende AZUL
          acendeAzul();
          delay(1);
          break;

        case BT_TRES :                      // botao tres - acende VERDE
          acendeVerde();
          delay(1);
          break;

        case BT_QUATRO:                     // botao quatro - reduzindo tempo
          tempo = tempo - 10;
          if ( tempo < MINTEMPO )
            tempo = MINTEMPO;
          break;

        case BT_CINCO:                    // botao cinco - aumentando tempo
          tempo = tempo + 10;
          if ( tempo > MAXTEMPO )
            tempo = MAXTEMPO;
          break;

        case BT_NOVE :                   // acende todos os LEDs em sequencia
          acendeTodos();
          delay(1);
          break;

        default:
          break;
      }
      apagaTudo();
    }
    irrecv.resume();                        // reinicializa o receptor
    delay(5);                              // atraso de 10 ms
  }

  if ( ledAtual != LED_TODOS ) {
    fadeIn( ledAtual, tempo );
    fadeOut( ledAtual, tempo );
  }
  else {
    apagaTudo();
    fadeIn( LED_VERDE, tempo/2 );
    apagaTudo();
    fadeIn( LED_VERMELHO, tempo/2 );
    apagaTudo();
    fadeIn( LED_AZUL, tempo/2 );
  }

}

void fadeIn( int led, int tempo ) {
  for (int i = 0; i <= 60; i++) {
    setRGB(logFade(i), led);
    delay(tempo); // Espere por alguns ms
  }
}
void fadeOut( int led, int tempo ) {
  for (int i = 60; i >= 0; i--) {
    setRGB(logFade(i), led);
    delay(tempo); // Espere por alguns ms
  }
}


void acendeTodos() {
  ledAtual = LED_TODOS;
}

// funcoes para acender leds
void acendeVermelho() {
  ledAtual = LED_VERMELHO;
}

void acendeAzul() {
  ledAtual = LED_AZUL ;
}

void acendeVerde() {
  ledAtual = LED_VERDE ;
}

void apagaTudo() {
  analogWrite( LED_VERMELHO, LOW );
  analogWrite( LED_AZUL, LOW );
  analogWrite ( LED_VERDE, LOW );
}



// Função para definir a intensidade dos LEDs RGB
void setRGB(int intensity, int led) {
  analogWrite(led, intensity);
  // analogWrite(greenPin, intensity);
  //analogWrite(bluePin, intensity);
}

// Função para calcular a intensidade com uma escala logarítmica
int logFade(double value) {
  return (int)(255.0 * (log10(1 + 9 * (value / 100.0))));
}
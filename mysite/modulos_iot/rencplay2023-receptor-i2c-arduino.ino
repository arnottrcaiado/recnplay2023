//
// Projeto REC N PLAY - Desfile MAKER
// Codigo para os Arduinos receptores
// Utilizando I2C
//
// previsao de comando de SERVO MOTORES
// e LEDS
// a definição de quais LEDs e Motores deve ser realizado nos HUBS com NodeMCU
//


#include <Wire.h>
#include <Servo.h>

Servo myservo[5];     // Cria objetos servo para controlar até 5 servos
struct ServoList {
  int address;        // Endereço do servo
  int pos_servo;      // Posição no array de servos
};
ServoList servoList[5];

const int SLAVE_ADDRESS = 0x08;  // Endereço I2C do dispositivo escravo
byte slaveCommand = 0x01;        // Comando enviado pelo escravo ao ligar

bool receive_new_command = false;

// Estrutura para armazenar as informações do mestre
struct MasterInfo {
  int address;                // Endereço do dispositivo mestre
  int gpioValue;              // Valor do pino GPIO associado
  int function;               // Função (a ser definida)
  int valueOn;                // Valor para ligar
  int valueOff;               // Valor para desligar
  int timeOn;                 // Tempo para ligar (ms)
  int timeOff;                // Tempo para desligar (ms)
  int timeIddle;              // Tempo de espera entre as operações (ms)
  unsigned long timeProcess;  // Tempo de início do processo
  int st_process;             // Estado do processo (0 - desligado, 1 - ligando, 2 - desligando)
};

byte command_receive[14]; // Array de bytes para armazenar os dados do mestre
                          // byte 0 e 1 - peripheral
                          // byte 2 e 3 - function
                          // byte 4 e 5 - valueOn
                          // byte 6 e 7 - valueOff
                          // byte 8 e 9 - timeOn
                          // byte 10 e 11 - timeOff
                          // byte 12 e 13 - timeIddle

const int MAX_MASTER_INFO = 10;
MasterInfo masterInfoList[MAX_MASTER_INFO];

void setup() {
  Serial.begin(9600);
  
  Wire.begin(SLAVE_ADDRESS);

  // Envia para o master que este device foi ligado
  send_to_master_begin();

  // Aguarda o master enviar a informação do setup do slave
  wait_response_master();

  // Coleta o dado enviado pelo master e atualiza a lista masterInfoList
  get_response_master();

  // Inicializa GPIO e envia comando ao mestre
  setup_gpio_slave();

  // Configura para aguardar dados do master
  //Wire.onEvent(receiveEvent); // Para ESP 32
  Wire.onReceive(receiveEvent); // Para Arduino
}

void loop() {
  // Verifica se há novos comandos do mestre
  if (receive_new_command)
  {
    update_gpio_slave();
    receive_new_command = false;
  }
}

void send_to_master_begin() {
  Wire.beginTransmission(0x00); // Endereço do mestre (ajuste conforme necessário)
  Wire.write(slaveCommand);     // Envie o comando do escravo ao mestre
  Wire.endTransmission();
}

void wait_response_master() {
  // Aguarde até receber 20 bytes do mestre
  while (Wire.available() < MAX_MASTER_INFO * 2 ) {
    delay(10);  // Aguarda 10 milissegundos e verifica novamente
  }

  // Você pode adicionar lógica adicional para verificar se os bytes recebidos
  // correspondem à resposta esperada do mestre, se necessário.
}

/*
 * 0 ... 9 -> Servo_motor
 * 10 ... 39 -> LEDs RGB
 * 40 ... 46 -> LEDS especiais
 */
void get_response_master() {
  int index = 0; // Índice para percorrer a lista masterInfoList
  int n_servo = 0; // indice da quantidade de servos

  while (Wire.available() >= 4 && index < MAX_MASTER_INFO) {
    MasterInfo info;

    byte data_received[4];
    data_received[0] = Wire.read();
    data_received[1] = Wire.read();
    data_received[2] = Wire.read();
    data_received[3] = Wire.read();
    

    // Leitura das informações do mestre
    info.address     = (data_received[0] << 8) | data_received[1];
    info.gpioValue   = (data_received[2] << 8) | data_received[3];
    info.function    = 0;
    info.valueOn     = 0;
    info.valueOff    = 0;
    info.timeOn      = 0;
    info.timeOff     = 0;
    info.timeIddle   = 0;
    info.timeProcess = 0;
    info.st_process  = 0;

    pinMode( info.gpioValue , OUTPUT);

    if (info.address < 9)
    {
      myservo[n_servo].attach( info.gpioValue );  // anexa o servo ao pino GPIO
      servoList[n_servo].address = info.address;
      servoList[n_servo].pos_servo = n_servo;
      n_servo++;
    }
    
    // Adiciona as informações na lista
    masterInfoList[index] = info;

    index++; // Avança para o próximo elemento da lista
  }
}

void setup_gpio_slave() {
  // Inicializa os GPIOs como Output com base nas informações no masterInfoList
  // está sendo feito em get_response_master, depois passar para cá
}

void receiveEvent(int byteCount)
{
  //Wire.readBytes(command_receive, byteCount );
  process_master_info(command_receive);
  receive_new_command = true;
}

void process_master_info(byte* receivedByte) {
  Wire.readBytes(command_receive, 14);

  // Converter bytes para inteiros
  int index = 0;
  MasterInfo info;
  info.address   = (command_receive[index]   << 8) | command_receive[index++];
  info.function  = (command_receive[index++] << 8) | command_receive[index++];
  info.valueOn   = (command_receive[index++] << 8) | command_receive[index++];
  info.valueOff  = (command_receive[index++] << 8) | command_receive[index++];
  info.timeOn    = (command_receive[index++] << 8) | command_receive[index++];
  info.timeOff   = (command_receive[index++] << 8) | command_receive[index++];
  info.timeIddle = (command_receive[index++] << 8) | command_receive[index];

  // Imprimir informações recebidas do mestre na serial
  Serial.print("Received: Address=");
  Serial.print(info.address);
  Serial.print(", Function=");
  Serial.print(info.function);
  Serial.print(", ValueOn=");
  Serial.print(info.valueOn);
  Serial.print(", ValueOff=");
  Serial.print(info.valueOff);
  Serial.print(", TimeOn=");
  Serial.print(info.timeOn);
  Serial.print(", TimeOff=");
  Serial.print(info.timeOff);
  Serial.print(", TimeIddle=");
  Serial.println(info.timeIddle);

  // Atualizar as informações na lista masterInfoList
  for (index = 0 ; index < MAX_MASTER_INFO ; index++)
  {
    if (masterInfoList[index].address == info.address)
    {
      masterInfoList[index].function  = info.function;
      masterInfoList[index].valueOn   = info.valueOn;
      masterInfoList[index].valueOff  = info.valueOff;
      masterInfoList[index].timeOn    = info.timeOn;
      masterInfoList[index].timeOff   = info.timeOff;
      masterInfoList[index].timeIddle = info.timeIddle;
    }
  }
}

/*
 * 0 ... 9 -> Servo_motor
 * 10 ... 39 -> LEDs RGB
 * 40 ... 46 -> LEDS especiais
 */
void update_gpio_slave() {
  // Implementação para atualizar os GPIOs com base nas informações do mestre
  int index = 0;
  
  for ( ; index < MAX_MASTER_INFO ; index++ )
  {
    // Change Iddle to On
    if (masterInfoList[index].st_process == 2 and masterInfoList[index].timeProcess - millis() > masterInfoList[index].timeIddle)
    {
      if (masterInfoList[index].address < 9)
      {
        for (int index_servo = 0 ; index_servo < 5 ; index_servo++ )
        {
          if (servoList[index_servo].address == masterInfoList[index].address)
          {
            myservo[index_servo].write(masterInfoList[index].valueOn);
            masterInfoList[index].st_process  = 0;
            masterInfoList[index].timeProcess = millis();
          }
        }
      }
      else
      {
        analogWrite( masterInfoList[index].gpioValue , masterInfoList[index].valueOn);
        masterInfoList[index].st_process  = 0;
        masterInfoList[index].timeProcess = millis();
      }
    }
    
    // Change Off to On
    if (masterInfoList[index].st_process == 1 and masterInfoList[index].timeProcess - millis() > masterInfoList[index].timeOff)
    {
      if (masterInfoList[index].address < 9)
      {
        for (int index_servo = 0 ; index_servo < 5 ; index_servo++ )
        {
          if (servoList[index_servo].address == masterInfoList[index].address)
          {
            masterInfoList[index].st_process  = 2;
            masterInfoList[index].timeProcess = millis();
          }
        }
      }
      else
      {
        analogWrite( masterInfoList[index].gpioValue , 0);
        masterInfoList[index].st_process  = 1;
        masterInfoList[index].timeProcess = millis();
      }
    }

    // Change On to Off
    if (masterInfoList[index].st_process == 0 and masterInfoList[index].timeProcess - millis() > masterInfoList[index].timeOn)
    {
      if (masterInfoList[index].address < 9)
      {
        for (int index_servo = 0 ; index_servo < 5 ; index_servo++ )
        {
          if (servoList[index_servo].address == masterInfoList[index].address)
          {
            myservo[index_servo].write(masterInfoList[index].valueOff);
            masterInfoList[index].st_process  = 1;
            masterInfoList[index].timeProcess = millis();
          }
        }
      }
      else
      {
        analogWrite( masterInfoList[index].gpioValue , masterInfoList[index].valueOff);
        masterInfoList[index].st_process  = 1;
        masterInfoList[index].timeProcess = millis();
      }
    }
    
  }
  
}

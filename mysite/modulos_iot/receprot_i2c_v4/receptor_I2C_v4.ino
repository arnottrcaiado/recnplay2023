#include <Wire.h>
#include "I2CCommunication.h"
#include <Servo.h>
#include "config.h"

const int I2C_master_address     = I2C_ADDRESS_MASTER;    // Endereço I2C do dispositivo mestre
const int I2C_slave_address      = I2C_ADDRESS_SLAVE_01;  // Endereço I2C do dispositivo escravo
//const int I2C_slave_address      = I2C_ADDRESS_SLAVE_02;  // Endereço I2C do dispositivo escravo
//const int I2C_slave_address      = I2C_ADDRESS_SLAVE_03;  // Endereço I2C do dispositivo escravo
//const int I2C_slave_address      = I2C_ADDRESS_SLAVE_04;  // Endereço I2C do dispositivo escravo

I2CCommunication i2cCommunication( I2C_slave_address ); // Substitua pelo seu endereço escravo

Servo myservo[N_SERVO_TO_SLAVE];     // Cria objetos servo para controlar até 5 servos
struct ServoList {
  int address;        // Endereço do servo
  int pos_servo;      // Posição no array de servos
};
ServoList servoList[N_SERVO_TO_SLAVE];

// Estrutura para armazenar as informações do mestre
struct PeripheralParams {
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

/*
	Command sent from Master
	byte peripheral = 0X2E;
	byte function = 0X02;
	byte max_value = 0X03;
	byte min_value = 0X04;
	byte on_time = 0x05;
	byte off_time = 0x06;
	byte idle_time = 0x07;
	byte unused =0x08;
	byte command[8] = {peripheral, function, max_value, min_value, on_time, off_time, idle_time, unused};
*/

const int MAX_PERIPHERAL_PER_SLAVE = N_PERIPHERAL_TO_SLAVE;
PeripheralParams listPeripheralParams[MAX_PERIPHERAL_PER_SLAVE];

byte command_receive[30]; // Array de bytes para armazenar os dados do mestre
                          // byte 0 e 1 - peripheral
                          // byte 2 e 3 - function
                          // byte 4 e 5 - valueOn
                          // byte 6 e 7 - valueOff
                          // byte 8 e 9 - timeOn
                          // byte 10 e 11 - timeOff
                          // byte 12 e 13 - timeIddle

bool set_slave_true = false;
bool receive_new_command = false;
byte dataReceived[50];

void setup() {
  Serial.begin(9600);
  
  Serial.println("PROJETO IOT SENAC");
  Serial.println("DESFILE SENAC");
  Serial.println("SLAVE I2C ON");

  delay(5000);

  // Inicializa a comunicação I2C
  Serial.println("Inicializando a comunicação I2C...");
  i2cCommunication.begin();

  // Configura para aguardar dados do master
  Serial.println("Configurando para receber comandos do mestre...");
  i2cCommunication.onReceive();
  //Wire.onRequest(requestEvent);                 // Configura uma função de callback para enviar dados
  i2cCommunication.onRequest();

  Serial.println("SLAVE I2C START");
}

void loop() {
  static unsigned long time_print_alive = millis();
  //
  if (millis() - time_print_alive > TIME_PRINT_ALIVE_MS)
  {
    Serial.println("   Still alive"); // Envia um sinal de vida
    time_print_alive = millis();
  }

  // Verifica se há novos comandos do mestre
  int numberBytesReceived = i2cCommunication.getDataReceived( dataReceived );
  if ( numberBytesReceived > 0 )
  {
    Serial.println("Recebido novos comandos do mestre");
    Serial.print("Quantidade de bytes recebidos: ");
    Serial.println(numberBytesReceived);
    delay(500);
    processDataReceived( numberBytesReceived );
  }

  if (!set_slave_true)
    return;
  
  // Verifica se há novos comandos do mestre
  if (receive_new_command)
  {
    //
    Serial.println("Recebendo novos comandos do mestre...");
    update_gpio_slave();
    receive_new_command = false;
  }
}

#define N_BYTES_COMMAND_GPIO 8
void processDataReceived(int numberBytesReceived)
{
	// SPECIAL COMMANDS
	if ( dataReceived[0] == 0xE0 )
	{
		// Verificar se a quantidade de dados é maior ou igual do que o pacote de configuração
		if ( numberBytesReceived >= MAX_PERIPHERAL_PER_SLAVE * 2 + 1 )
		{
			// Configuracao Slave
			/*
			 * ADDRESS: 0 ... 9 -> Servo_motor
			 *          10 ... 39 -> LEDs RGB
			 *          40 ... 46 -> LEDS especiais
			 */
			int index = 0; // Índice para percorrer a lista listPeripheralParams
			int n_servo = 0; // indice da quantidade de servos
			while (index < MAX_PERIPHERAL_PER_SLAVE * 2 )
			{
				PeripheralParams peripheralParams;
				peripheralParams.address     = (int)dataReceived[2*index];
				peripheralParams.gpioValue   = (int)dataReceived[2*index+1];
				peripheralParams.function    = 0;
				peripheralParams.valueOn     = 0;
				peripheralParams.valueOff    = 0;
				peripheralParams.timeOn      = 0;
				peripheralParams.timeOff     = 0;
				peripheralParams.timeIddle   = 0;
				peripheralParams.timeProcess = 0;
				peripheralParams.st_process  = 0;
				/*
				// Considerando que foram recebidos dois bytes por valor
				peripheralParams.address     = (dataReceived[4*index] << 8) | dataReceived[4*index +1];
				peripheralParams.gpioValue   = (dataReceived[4*index+2] << 8) | dataReceived[4*index+3];
				*/
				
				pinMode( peripheralParams.gpioValue , OUTPUT);
				
				if ( peripheralParams.address < 9 )
				{
				  //
				  Serial.println("...");
				  myservo[n_servo].attach( peripheralParams.gpioValue );  // anexa o servo ao pino GPIO
				  servoList[n_servo].address = peripheralParams.address;
				  servoList[n_servo].pos_servo = n_servo;
				  n_servo++;
				}
				
				// Adiciona as informações na lista
				listPeripheralParams[index] = peripheralParams;

				index++; // Avança para o próximo elemento da lista
			}
			set_slave_true = true;
			
			int bytesTotal = 0;
			if ( numberBytesReceived > MAX_PERIPHERAL_PER_SLAVE * 2 + 1 )
			{
				int i = 0;
				for ( ; index + i < numberBytesReceived ; i++ )
				{
					dataReceived[i] = dataReceived[index + i];
				}
				dataReceived[i] = '\0';
				processDataReceived( i );
			}
			return;
		}
		else
		{
			dataReceived[0] = '\0';
			return;
		}
	}
	
	// NORMAL COMMANDS
	if ( dataReceived[0] < ADDRESS_LED_LARGE_07 )	// Endereço do último periférico
	{
		// Verificar se a quantidade de dados é maior ou igual do que o pacote de configuração
		if ( numberBytesReceived >= N_BYTES_COMMAND_GPIO )
		{
      // Atualizar as informações na lista listPeripheralParams
      int index = 0;
      for ( ; index < MAX_PERIPHERAL_PER_SLAVE ; index++)
      {
        if (listPeripheralParams[index].address == dataReceived[index*8])
        {
          // Faça o processamento do comando para o periférico
          Serial.print("Comando recebido para o periférico 0x");
          Serial.println(listPeripheralParams[index].address, HEX);
      
          listPeripheralParams[index].function  = (int)dataReceived[index*8+1];
          listPeripheralParams[index].valueOn   = (int)dataReceived[index*8+2];
          listPeripheralParams[index].valueOff  = (int)dataReceived[index*8+3];
          listPeripheralParams[index].timeOn    = (int)dataReceived[index*8+4];
          listPeripheralParams[index].timeOff   = (int)dataReceived[index*8+5];
          listPeripheralParams[index].timeIddle = (int)dataReceived[index*8+6];
          //listPeripheralParams[index].???     = (int)dataReceived[index*8+7];
          
          // Imprimir informações recebidas do mestre na serial
          Serial.print("Received: Address=");
          Serial.print(listPeripheralParams[index].address);
          Serial.print(", Function=");
          Serial.print(listPeripheralParams[index].function);
          Serial.print(", ValueOn=");
          Serial.print(listPeripheralParams[index].valueOn);
          Serial.print(", ValueOff=");
          Serial.print(listPeripheralParams[index].valueOff);
          Serial.print(", TimeOn=");
          Serial.print(listPeripheralParams[index].timeOn);
          Serial.print(", TimeOff=");
          Serial.print(listPeripheralParams[index].timeOff);
          Serial.print(", TimeIddle=");
          Serial.println(listPeripheralParams[index].timeIddle);

        }
			}
			int bytesTotal = 0;
			if ( numberBytesReceived > N_BYTES_COMMAND_GPIO )
			{
				int i = 0;
				for ( ; index + i < numberBytesReceived ; i++ )
				{
					dataReceived[i] = dataReceived[index + i];
				}
				dataReceived[i] = '\0';
				processDataReceived( i );
			}
			return;
			
		}
		else
		{
			dataReceived[0] = '\0';
			return;
		}
	}
}

void setup_gpio_slave() {
  // Inicializa os GPIOs como saída com base nas informações em listPeripheralParams
  Serial.println("...");

  for (int i = 0; i < MAX_PERIPHERAL_PER_SLAVE ; i++)
  {
    pinMode(listPeripheralParams[i].gpioValue, OUTPUT);
  }

  Serial.println("...");
}

/*
 * 0 ... 9 -> Servo_motor
 * 10 ... 39 -> LEDs RGB
 * 40 ... 46 -> LEDS especiais
 */
void update_gpio_slave() {
  // Implementação para atualizar os GPIOs com base nas informações do mestre
  int index = 0;
  
  //
  Serial.println("...");
  for ( ; index < MAX_PERIPHERAL_PER_SLAVE ; index++ )
  {
    //
    Serial.println("...");

    // Change Iddle to On
    if (listPeripheralParams[index].st_process == 2 and listPeripheralParams[index].timeProcess - millis() > listPeripheralParams[index].timeIddle)
    {
      //
      Serial.println("...");
      if (listPeripheralParams[index].address < 9)
      {
        for (int index_servo = 0 ; index_servo < 5 ; index_servo++ )
        {
          if (servoList[index_servo].address == listPeripheralParams[index].address)
          {
            //
            Serial.println("...");
            myservo[index_servo].write(listPeripheralParams[index].valueOn);
            listPeripheralParams[index].st_process  = 0;
            listPeripheralParams[index].timeProcess = millis();
          }
        }
      }
      else
      {
        //
        Serial.println("...");
        analogWrite( listPeripheralParams[index].gpioValue , listPeripheralParams[index].valueOn);
        listPeripheralParams[index].st_process  = 0;
        listPeripheralParams[index].timeProcess = millis();
      }
    }
    
    // Change Off to On
    if (listPeripheralParams[index].st_process == 1 and listPeripheralParams[index].timeProcess - millis() > listPeripheralParams[index].timeOff)
    {
      //
      Serial.println("...");
      if (listPeripheralParams[index].address < 9)
      {
        for (int index_servo = 0 ; index_servo < 5 ; index_servo++ )
        {
          if (servoList[index_servo].address == listPeripheralParams[index].address)
          {
            //
            Serial.println("...");
            listPeripheralParams[index].st_process  = 2;
            listPeripheralParams[index].timeProcess = millis();
          }
        }
      }
      else
      {
        //
        Serial.println("...");
        analogWrite( listPeripheralParams[index].gpioValue , 0);
        listPeripheralParams[index].st_process  = 1;
        listPeripheralParams[index].timeProcess = millis();
      }
    }

    // Change On to Off
    if (listPeripheralParams[index].st_process == 0 and listPeripheralParams[index].timeProcess - millis() > listPeripheralParams[index].timeOn)
    {
      //
      Serial.println("...");
      if (listPeripheralParams[index].address < 9)
      {
        for (int index_servo = 0 ; index_servo < 5 ; index_servo++ )
        {
          if (servoList[index_servo].address == listPeripheralParams[index].address)
          {
            //
            Serial.println("...");
            myservo[index_servo].write(listPeripheralParams[index].valueOff);
            listPeripheralParams[index].st_process  = 1;
            listPeripheralParams[index].timeProcess = millis();
          }
        }
      }
      else
      {
        //
        Serial.println("...");
        analogWrite( listPeripheralParams[index].gpioValue , listPeripheralParams[index].valueOff);
        listPeripheralParams[index].st_process  = 1;
        listPeripheralParams[index].timeProcess = millis();
      }
    }
    
  }
  
}

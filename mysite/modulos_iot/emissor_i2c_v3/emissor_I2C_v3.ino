#include <Wire.h>
#include "I2CCommunication.h"
#include "config.h"

const int I2C_master_address     = I2C_ADDRESS_MASTER;    // Endereço I2C do dispositivo mestre
const int I2C_slave_address_01   = I2C_ADDRESS_SLAVE_01;  // Endereço I2C do dispositivo escravo
const int I2C_slave_address_02   = I2C_ADDRESS_SLAVE_02;  // Endereço I2C do dispositivo escravo
const int I2C_slave_address_03   = I2C_ADDRESS_SLAVE_03;  // Endereço I2C do dispositivo escravo
const int I2C_slave_address_04   = I2C_ADDRESS_SLAVE_04;  // Endereço I2C do dispositivo escravo

I2CCommunication i2cCommunication( I2C_master_address ); // Substitua pelo seu endereço mestre

bool set_slave_01_true = false;
bool set_slave_02_true = false;
bool set_slave_03_true = false;
bool set_slave_04_true = false;

// Defina a lista de dispositivos(Address e GPIO)
const int numDevices = N_PERIPHERAL_TO_SLAVE;
int device_slave_01[numDevices][2] = {
  { ADDRESS_SERVO_01         , GPIO_SERVO_01         },
  { ADDRESS_SERVO_02         , GPIO_SERVO_02         },
  { ADDRESS_SERVO_03         , GPIO_SERVO_03         },
  { ADDRESS_SERVO_04         , GPIO_SERVO_04         },
  { ADDRESS_LED_RGB_01_RED   , GPIO_LED_RGB_01_RED   },
  { ADDRESS_LED_RGB_01_GREEN , GPIO_LED_RGB_01_GREEN },
  { ADDRESS_LED_RGB_01_BLUE  , GPIO_LED_RGB_01_BLUE  },
  { ADDRESS_LED_RGB_02_RED   , GPIO_LED_RGB_02_RED   },
  { ADDRESS_LED_RGB_02_GREEN , GPIO_LED_RGB_02_GREEN },
  { ADDRESS_LED_RGB_02_BLUE  , GPIO_LED_RGB_02_BLUE  },
};

int device_slave_02[numDevices][2] = {
  { ADDRESS_SERVO_05         , GPIO_SERVO_05         },
  { ADDRESS_SERVO_06         , GPIO_SERVO_06         },
  { ADDRESS_SERVO_07         , GPIO_SERVO_07         },
  { ADDRESS_SERVO_08         , GPIO_SERVO_08         },
  { ADDRESS_LED_RGB_03_RED   , GPIO_LED_RGB_03_RED   },
  { ADDRESS_LED_RGB_03_GREEN , GPIO_LED_RGB_03_GREEN },
  { ADDRESS_LED_RGB_03_BLUE  , GPIO_LED_RGB_03_BLUE  },
  { ADDRESS_LED_RGB_04_RED   , GPIO_LED_RGB_04_RED   },
  { ADDRESS_LED_RGB_04_GREEN , GPIO_LED_RGB_04_GREEN },
  { ADDRESS_LED_RGB_04_BLUE  , GPIO_LED_RGB_04_BLUE  },
};

int device_slave_03[numDevices][2] = {
  { ADDRESS_SERVO_09         , GPIO_SERVO_09         },
  { ADDRESS_LED_RGB_05_RED   , GPIO_LED_RGB_05_RED   },
  { ADDRESS_LED_RGB_05_GREEN , GPIO_LED_RGB_05_GREEN },
  { ADDRESS_LED_RGB_05_BLUE  , GPIO_LED_RGB_05_BLUE  },
  { ADDRESS_LED_RGB_06_RED   , GPIO_LED_RGB_06_RED   },
  { ADDRESS_LED_RGB_06_GREEN , GPIO_LED_RGB_06_GREEN },
  { ADDRESS_LED_RGB_06_BLUE  , GPIO_LED_RGB_06_BLUE  },
  { ADDRESS_LED_RGB_07_RED   , GPIO_LED_RGB_07_RED   },
  { ADDRESS_LED_RGB_07_GREEN , GPIO_LED_RGB_07_GREEN },
  { ADDRESS_LED_RGB_07_BLUE  , GPIO_LED_RGB_07_BLUE  },
};

int device_slave_04[numDevices][2] = {
  { ADDRESS_SERVO_10         , GPIO_SERVO_09         },
  { ADDRESS_LED_RGB_08_RED   , GPIO_LED_RGB_08_RED   },
  { ADDRESS_LED_RGB_08_GREEN , GPIO_LED_RGB_08_GREEN },
  { ADDRESS_LED_RGB_08_BLUE  , GPIO_LED_RGB_08_BLUE  },
  { ADDRESS_LED_RGB_09_RED   , GPIO_LED_RGB_09_RED   },
  { ADDRESS_LED_RGB_09_GREEN , GPIO_LED_RGB_09_GREEN },
  { ADDRESS_LED_RGB_09_BLUE  , GPIO_LED_RGB_09_BLUE  },
  { ADDRESS_LED_RGB_10_RED   , GPIO_LED_RGB_10_RED   },
  { ADDRESS_LED_RGB_10_GREEN , GPIO_LED_RGB_10_GREEN },
  { ADDRESS_LED_RGB_10_BLUE  , GPIO_LED_RGB_10_BLUE  },
};

void setup() {
  Serial.begin(9600);

  Serial.println("PROJETO IOT SENAC");
  Serial.println("DESFILE SENAC");
  Serial.println("MASTER I2C ON");

  delay(5000);

 
  i2cCommunication.begin();

}

static unsigned long timeCheckSlave01 = 0;
static unsigned long timeCheckSlave02 = 0;
static unsigned long timeCheckSlave03 = 0;
static unsigned long timeCheckSlave04 = 0;

void loop() {
  //Serial.println("Start loop");
  set_slave_01_true = setSlave( 1 , set_slave_01_true , I2C_slave_address_01 , device_slave_01 );
  set_slave_02_true = setSlave( 2 , set_slave_02_true , I2C_slave_address_02 , device_slave_02 );
  set_slave_03_true = setSlave( 3 , set_slave_03_true , I2C_slave_address_03 , device_slave_03 );
  set_slave_04_true = setSlave( 4 , set_slave_04_true , I2C_slave_address_04 , device_slave_04 );

  return;

  // adicionar logica para enviar comandos
  static unsigned long timeSendCommand = millis();
  if ( millis() - timeSendCommand > 10000 )
  {
    Serial.println("Start SendCommand");
    sendCommand();
    /*byte peripheral = 0X2E;
    byte function = 0X02;
    byte max_value = 0X03;
    byte min_value = 0X04;
    byte on_time = 0x05;
    byte off_time = 0x06;
    byte idle_time = 0x07;
    byte unused =0x08;
    byte command[8] = {peripheral, function, max_value, min_value, on_time, off_time, idle_time, unused};

    int result = sendCommandSlave( peripheral , command );

    peripheral = 0X05;
    result = sendCommandSlave( peripheral , command );

    peripheral = 0X07;
    result = sendCommandSlave( peripheral , command );

    peripheral = 0X12;
    result = sendCommandSlave( peripheral , command );

    peripheral = 0X23;
    result = sendCommandSlave( peripheral , command );

    peripheral = 0X37;
    result = sendCommandSlave( peripheral , command );

    peripheral = 0X0A;
    result = sendCommandSlave( peripheral , command );

    peripheral = 0XDC;
    result = sendCommandSlave( peripheral , command );*/
    
    timeSendCommand = millis();
    Serial.println("End SendCommand");
  }
  delay(50);
  //Serial.println("End loop");
}

void sendCommand()
{
  byte peripheral = 0X2E;
  byte function = 0X02;
  byte max_value = 0X03;
  byte min_value = 0X04;
  byte on_time = 0x05;
  byte off_time = 0x06;
  byte idle_time = 0x07;
  byte unused =0x08;
  byte command[8] = {peripheral, function, max_value, min_value, on_time, off_time, idle_time, unused};

  int result = sendCommandSlave( peripheral , command );

  peripheral = 0X05;
  result = sendCommandSlave( peripheral , command );

  peripheral = 0X07;
  result = sendCommandSlave( peripheral , command );

  peripheral = 0X12;
  result = sendCommandSlave( peripheral , command );

  peripheral = 0X23;
  result = sendCommandSlave( peripheral , command );

  peripheral = 0X37;
  result = sendCommandSlave( peripheral , command );

  peripheral = 0X0A;
  result = sendCommandSlave( peripheral , command );

  peripheral = 0XDC;
  result = sendCommandSlave( peripheral , command );
}

int sendCommandSlave( byte peripheral , byte* commandToSend )
{
  int slave = checkPeripheralToSlave( peripheral );
  //int slave = 2;
  int set_slave_true = false;
  byte I2C_slave_address;
  if (slave == 1)
  {
    set_slave_true = set_slave_01_true;
    I2C_slave_address = I2C_slave_address_01;
  }
  if (slave == 2)
  {
    set_slave_true = set_slave_02_true;
    I2C_slave_address = I2C_slave_address_02;
  }
  if (slave == 3)
  {
    set_slave_true = set_slave_03_true;
    I2C_slave_address = I2C_slave_address_03;
  }
  if (slave == 4)
  {
    set_slave_true = set_slave_04_true;
    I2C_slave_address = I2C_slave_address_04;
  }
  if (slave == 0)
  {
    Serial.println("   Slave isn't known");
    return 0;
  }
  if ( set_slave_true )
  {
    Serial.print("   Send command to control GPIO, device is ");
    Serial.println(I2C_slave_address,HEX);
    i2cCommunication.sendControlGpioSlave( I2C_slave_address , 8 , commandToSend );
  }
  else
  {
    Serial.print("   Slave isn't configured, device is ");
    Serial.println(I2C_slave_address,HEX);
  }
  if (slave == 1) return 1;
  if (slave == 2) return 2;
  if (slave == 3) return 3;
  if (slave == 4) return 4;
}

int checkPeripheralToSlave( byte address_peripheral )
{
  int index = 0;
  for ( ; index < numDevices ; index++)
  {
    if ( address_peripheral == device_slave_01[index][0] )
      return 1;
  }
  index = 0;
  for ( ; index < numDevices ; index++)
  {
    if ( address_peripheral == device_slave_02[index][0] )
      return 2;
  }
  index = 0;
  for ( ; index < numDevices ; index++)
  {
    if ( address_peripheral == device_slave_03[index][0] )
      return 3;
  }
  index = 0;
  for ( ; index < numDevices ; index++)
  {
    if ( address_peripheral == device_slave_04[index][0] )
      return 4;
  }
  return 0;
}

#define TIME_SET_SLAVE_AGAIN  5000

bool setSlave( int slave , int set_slave_true , int slave_address , int addressAndGpio[][2] )
{
  if (!set_slave_true)
  {
    // checa se faz mais de 10 segundos que tentou se conectar ao slave
    if (slave == 1)
      if ( millis() - timeCheckSlave01 < TIME_SET_SLAVE_AGAIN and timeCheckSlave01 != 0 )
        return false;
    if (slave == 2)
      if ( millis() - timeCheckSlave02 < TIME_SET_SLAVE_AGAIN and timeCheckSlave02 != 0 )
        return false;
    if (slave == 3)
      if ( millis() - timeCheckSlave03 < TIME_SET_SLAVE_AGAIN and timeCheckSlave03 != 0 )
        return false;
    if (slave == 4)
      if ( millis() - timeCheckSlave04 < TIME_SET_SLAVE_AGAIN and timeCheckSlave04 != 0 )
        return false;

    if (slave == 1) timeCheckSlave01 = millis();
    if (slave == 2) timeCheckSlave02 = millis();
    if (slave == 3) timeCheckSlave03 = millis();
    if (slave == 4) timeCheckSlave04 = millis();
    

    Serial.print("Sending device configurations to ");
    if (slave == 1) Serial.println("slave_01");
    if (slave == 2) Serial.println("slave_02");
    if (slave == 3) Serial.println("slave_03");
    if (slave == 4) Serial.println("slave_04");
    // Envia dados para o Slave
    int result = i2cCommunication.setSlave( slave_address , numDevices , addressAndGpio );
    if (result == 0)
      Serial.println("Sent device configurations to slave");
    else
    {
      if ( slave == 1 ) 
        Serial.println("Erro ao Configurar slave 01");
      if ( slave == 2 ) 
        Serial.println("Erro ao Configurar slave 02");
      if ( slave == 3 ) 
        Serial.println("Erro ao Configurar slave 03");
      if ( slave == 4 ) 
        Serial.println("Erro ao Configurar slave 04");
      return false;
    }

    //delay(7000);

    if (i2cCommunication.getConfirmSlave( slave_address , 2 ))
    {
      if ( slave == 1 ) 
      {
        Serial.println("Configuração slave 01 OK");
        timeCheckSlave01 = millis();
      }
      if ( slave == 2 ) 
      {
        Serial.println("Configuração slave 02 OK");
        timeCheckSlave02 = millis();
      }
      if ( slave == 3 ) 
      {
        Serial.println("Configuração slave 03 OK");
        timeCheckSlave03 = millis();
      }
      if ( slave == 4 ) 
      {
        Serial.println("Configuração slave 04 OK");
        timeCheckSlave04 = millis();
      }
      return true;
    }
  }
  if (set_slave_true)
  {
    if ( slave == 1 )
      if ( millis() - timeCheckSlave01 > 60000 )
        return false;
    if ( slave == 2 )
      if ( millis() - timeCheckSlave02 > 60000 )
        return false;
    if ( slave == 3 )
      if ( millis() - timeCheckSlave03 > 60000 )
        return false;
    if ( slave == 4 )
      if ( millis() - timeCheckSlave04 > 60000 )
        return false;
    return true;
  }
  return false;
}
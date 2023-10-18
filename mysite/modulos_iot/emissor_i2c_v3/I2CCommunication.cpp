#include "I2CCommunication.h"
#include <arduino.h>

#define _START_PROCOL_ "AA"
#define _END_PROCOL_ "AA"
#define _CONFIRM_SLAVE_ "OK"

byte _dataReceived_[_BUFFER_DATA_RECEIVED_] = {};
int _numberBytesReveived = 0;
bool _bufferFull = false;
int _ERROR_ = 0;
/*
                  -1 ERRO DE PROCOLO
*/

// Master and Slave
I2CCommunication::I2CCommunication(int ownAddress) {
  this->ownAddress = ownAddress;
}

// Master and Slave
void I2CCommunication::begin() {
  Wire.begin(this->ownAddress);
}

// Master
int I2CCommunication::setSlave( int slaveAddress , int numDevicesConnected , int addressAndGpio[][2] ) {
  // Inicia a transmissão para o Slave com endereço slaveAddress
  Wire.beginTransmission( slaveAddress );
  // Envia os endereços e gpios do perifericos
  Wire.write( _START_PROCOL_ );
  for (int i = 0; i < numDevicesConnected ; i++) {
    Wire.write( addressAndGpio[i][0]); // Endereço do dispositivo
    Wire.write( addressAndGpio[i][1]); // GPIO associado
  }
  Wire.write( _END_PROCOL_ );
  // Finaliza a transmissão
  int _result = Wire.endTransmission();
  delay(100);
  return _result;
}

// Master
bool I2CCommunication::getConfirmSlave( int slaveAddress , int numBytesConfirm ) {
  int numberBytes = Wire.requestFrom( slaveAddress , numBytesConfirm ); // Solicita n bytes do Slave

  if (numberBytes != numBytesConfirm)
  {
    Serial.print("Erro ao solicitar dado do dispositivo com endereço 0x");
    Serial.println(slaveAddress, HEX);
    return false;
  }

  char* confirmSlave = _CONFIRM_SLAVE_;

  delay(100);

  char receivedChar[numBytesConfirm];
  int i = 0;
  while (Wire.available())
  {
    receivedChar[i] = Wire.read(); // Lê os dados recebidos do Slave
    i++;
    if (i >= numBytesConfirm)
      break;
  }

  Serial.print("Received: ");
  for ( i = 0 ; i < numBytesConfirm ; i++ )
  {
    Serial.print(receivedChar[i]);
  }
  Serial.println("");

  if (receivedChar[0] == confirmSlave[0] && receivedChar[1] == confirmSlave[1])
  {
    return true;
  }
  return false;
}

// Master
void I2CCommunication::sendControlGpioSlave( int slaveAddress , int numberBytesCommand , byte* command ) {
  Wire.beginTransmission( slaveAddress );
  Wire.write( _START_PROCOL_ );
  Wire.write( command, numberBytesCommand );
  Wire.write( _END_PROCOL_ );
  int result = Wire.endTransmission();

  if (result == 0) {
    Serial.print("   Comando enviado para o dispositivo com endereço 0x");
    Serial.println( slaveAddress , HEX);
  }
  else
  {
    Serial.print("   Erro ao enviar o comando para o dispositivo com endereço 0x");
    Serial.println( slaveAddress , HEX);
  }
}

// Slave
char responseMaster_[2] = "EF";
void receiveEvent(int byteCount)
{
  byte __dataReceived__[_BUFFER_DATA_RECEIVED_];
  byte _startPackage_[2] = _START_PROCOL_;
  byte _endPackage_[2] = _END_PROCOL_;
  int index = 0;
  while (Wire.available())
  {
    __dataReceived__[index] = Wire.read(); // Lê os dados recebidos do Master
    // TESTE
    Serial.print("Received from Master [");
    Serial.print(index);
    Serial.print("]: ");
    Serial.print(__dataReceived__[index]);
    Serial.print("(Dec) , ");
    Serial.print(__dataReceived__[index], HEX);
    Serial.println("(Hex) ");
    // TESTE
    index++;
    delay(10);
  }
  if ( __dataReceived__[0]         != _startPackage_[0] or 
       __dataReceived__[1]         != _startPackage_[1] or
       __dataReceived__[index - 2] != _endPackage_[0]   or
       __dataReceived__[index - 1] != _endPackage_[1])
  {
    _ERROR_ = -1;
    responseMaster_[0] = 'D';
    responseMaster_[0] = 'D';
    return;
  }
  responseMaster_[0] = 'O';
  responseMaster_[0] = 'K';
  int i = 2;
  while ( i < index - 2)
  {
    _dataReceived_[_numberBytesReveived] =__dataReceived__[index]; // Atualiza vetor de dados

    _numberBytesReveived++;

    if (_numberBytesReveived >= _BUFFER_DATA_RECEIVED_)
    {
      _numberBytesReveived = 0;
      _bufferFull = true;
    }
  }
}

// Slave
void I2CCommunication::onReceive() {
  //Wire.onEvent(receiveEvent); // Para ESP 32  // Configura uma função de callback para receber dados
  Wire.onReceive(receiveEvent); // Para Arduino // Configura uma função de callback para receber dados
}

// Slave
void requestEvent() {
  Wire.write(responseMaster_); // Envia a string "OK" de volta para o Master
  Serial.print("Enviado resposta ao Master: ");
  Serial.println(responseMaster_);
  responseMaster_[0] = 'E';
  responseMaster_[1] = 'F';
}

// Slave
void I2CCommunication::onRequest() {
  Wire.onRequest(requestEvent); // Configura uma função de callback para enviar dados
}

int I2CCommunication::getDataReceived( byte* dataReceived ) {
  int index = 0;
  int tailData = 0;
  if ( _bufferFull == false )
    if ( _numberBytesReveived == 0 )
      return 0;
  if ( _bufferFull == true )
  {
    tailData = _numberBytesReveived;
    for ( ; index < 50 ; index++ )
    {
      dataReceived[ index ] = _dataReceived_[ tailData + index ];
      if ( tailData + index + 1 >= 50 )
        tailData = - index - 1 ;
    }
  }
  else
  {
    for ( ; index < _numberBytesReveived ; index++ )
    {
      dataReceived[ index ] = _dataReceived_[ index ];
    }
  }
  _bufferFull = false;
  _numberBytesReveived = 0;
}



/*
void I2CCommunication::send(int slaveAddress, byte data[], int dataSize) {
  Wire.beginTransmission(slaveAddress);
  Wire.write(data, dataSize);
  Wire.endTransmission();
}

int I2CCommunication::receive(byte data[], int dataSize) {
  int bytesRead = 0;
  if (Wire.available() >= dataSize) {
    bytesRead = Wire.readBytes(data, dataSize);
  }
  return bytesRead;
}*/
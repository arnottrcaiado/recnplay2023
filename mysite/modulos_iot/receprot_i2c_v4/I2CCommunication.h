#ifndef I2C_COMMUNICATION_H
#define I2C_COMMUNICATION_H

#include <Wire.h>
#include <arduino.h>

#define _BUFFER_DATA_RECEIVED_ 50

class I2CCommunication {
public:
  I2CCommunication(int ownAddress);

  void begin();

  int setSlave( int slaveAddress , int numDevicesConnected , int addressAndGpio[][2] );

  bool getConfirmSlave( int slaveAddress , int numBytesConfirm );

  void sendControlGpioSlave( int slaveAddress , int numberBytesCommand , byte* command );

  void onReceive();

  void onRequest();

  int getDataReceived( byte* dataReceived );

  //void send(int slaveAddress, byte data[], int dataSize);
  //int receive(byte data[], int dataSize);

private:
  int ownAddress;
  //byte _dataReceived_[_BUFFER_DATA_REVEIVE_] = {};
  //int _numberBytesReveived = 0;
  //bool _bufferFull = false;
};

#endif
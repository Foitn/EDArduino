#ifndef _EMBEDDED_DEBBUGER_H_
#define _EMBEDDED_DEBBUGER_H_

#if ARDUINO>=100
  #include <Arduino.h>
#endif
#include <HardwareSerial.h>  // Arduino AVR
#define SERIAL_CLASS HardwareSerial
#include "msg.h"

class ArduinoHardware{
public:
  //ArduinoHardware(SERIAL_CLASS* iostream, long baudRate=192000){
  //  this->iostream=iostream;
  //  this->baudRate=baudRate;
  //}
  ArduinoHardware(){
    iostream = &Serial;
    baudRate=9600;
  }
  void setBaudRate(long baudRate){
    this->baudRate = baudRate;
  }
  long getBaudRate(){return baudRate;}
  void init(){iostream->begin(baudRate);};
  int read(){return iostream->read();};
  void write(uint8_t* data, int length){
    for(int i=0; i<length; i++)
      iostream->write(data[i]);
  }
protected:
  SERIAL_CLASS* iostream;
  long baudRate;
};

namespace ed{
  class EmbeddedDebugger{
  private:
    uint8_t message_out[512];
    ArduinoHardware hardware;
  public:
    EmbeddedDebugger(){}

    void init(){
      hardware.init();
    }
    void send(const Msg *msg){
      int length = msg->serialize(message_out);
      hardware.write(message_out, length);
    }
    void spinOnce(){}
  };
}
#endif

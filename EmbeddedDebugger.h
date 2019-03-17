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
ArduinoHardware(SERIAL_CLASS* iostream, long baudRate=192000){
  this->iostream=iostream;
  this->baudRate=baudRate;
}
void setBaudRate(long baudRate){
  this->baudRate = baudRate;
}
long getBaudRate(){return baudRate;}
void init(){iostream->begin(baudRate);};
int read(){return iostream->read();};
protected:
  SERIAL_CLASS* iostream;
  long baudRate;
};

namespace ed{
  class EmbeddedDebugger{
  public:
    EmbeddedDebugger(){}

    void init();
    void send();
    void spinOnce();
  protected:

  private:

  };
}
#endif

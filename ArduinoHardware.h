#include <Arduino.h>
#include <HardwareSerial.h>  // Arduino AVR
#define SERIAL_CLASS HardwareSerial
namespace ed{
  class ArduinoHardware{
  public:
    ArduinoHardware(){
      iostream = &Serial;
      baudRate = 9600;
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
}

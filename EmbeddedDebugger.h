#ifndef _EMBEDDED_DEBBUGER_H_
#define _EMBEDDED_DEBBUGER_H_

#include "msg.h"
#if ARDUINO>=100
  #include "ArduinoHardware.h"
#endif

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

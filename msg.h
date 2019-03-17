#ifndef _EMBEDDED_DEBUGGER_MSG_H_
#define _EMBEDDED_DEBUGGER_MSG_H_

#include <stdint.h>
#include <stddef.h>
#define STX 0x55
#define ETX 0xAA
#define ESC 0x66

namespace ed
{
  enum Command{
    GetInfo = 'I',
    GetVersion = 'V',
    WriteRegister = 'W',
    QueryRegister = 'Q',
    ConfigChannel = 'C',
    Decimation = 'D',
    ResetTime = 'T',
    ReadChannelData = 'R',
    DebugString = 'S'
  };

  class Msg
  {
  public:
    Msg(){}

    int serialize(unsigned char *outbuffer) const
    {
      unsigned char *writePointer = *outbuffer;

      return 0;
    }
    int deserialize(unsigned char *inbuffer)
    {
      if(*inbuffer != STX) return 0;
      unsigned char *counterPointer = *inbuffer;
      uint32_t size = 0;
      do {
        size++;
      } while(counterPointer++ != ETX);
      nodeID = *inbuffer + 1;
      msgID = *inbuffer + 2;
      cmd = *inbuffer + 3;
      memcpy(cmdData, inbuffer+4, size-7);
      crc = *inbuffer + (size-2);
      return 0;
    }
  protected:
    uint8_t nodeID;
    uint8_t msgID;
    Command cmd;
    uint8_t cmdData[];
    uint8_t crc;
  };
}

#endif

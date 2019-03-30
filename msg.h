#ifndef _EMBEDDED_DEBUGGER_MSG_H_
#define _EMBEDDED_DEBUGGER_MSG_H_

#include <stdint.h>
#include <stddef.h>
#define STX 0x55
#define ETX 0xAA
#define ESC 0x66

namespace ed
{
  const uint8_t crcTable[ ] =
{
     0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
   157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
    35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
   190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
    70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
   219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
   101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
   248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
   140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
    17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
   175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
    50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
   202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
    87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
   233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
   116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};
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
    Msg(uint8_t nodeID, uint8_t msgID, uint8_t cmd, uint8_t cmdData[], uint8_t cmdDataSize){
      this->nodeID = nodeID;
      this->msgID = msgID;
      this->cmd = cmd;
      memcpy(this->cmdData, cmdData, cmdDataSize);
      this->cmdDataSize = cmdDataSize;
    }

    unsigned char AddToCRC(uint8_t crc, uint8_t charToAdd){
      return crcTable[crc ^ charToAdd];
    }

    int serialize(uint8_t *outbuffer) const
    {
      uint8_t *writePointer = outbuffer;
      uint8_t crc = 0;
      *writePointer++ = STX;
      *writePointer++ = nodeID;
      crc = AddToCRC(crc, nodeID);
      *writePointer++ = msgID;
      crc = AddToCRC(crc, msgID);
      *writePointer++ = cmd;
      crc = AddToCRC(crc, cmd);
      for(int i = 0; i < cmdDataSize; i++){
        crc = AddToCRC(crc, cmdData[i]);
        if(cmdData[i] == STX || cmdData[i] == ETX ||  cmdData[i] == ESC){
          *writePointer++ = ESC;
          *writePointer++ = cmdData[i] ^ ESC;
        } else {
          *writePointer++ = cmdData[i];
        }
      }
      if(crc == STX || crc == ETX || crc == ESC){
        *writePointer++ = crc ^ ESC;
      } else {
        *writePointer++ = crc;
      }
      *writePointer = ETX;
      return cmdDataSize + 7;
    }

    // input = char array
    // output = msg
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
      cmd = (Command)*inbuffer + 3;
      memcpy(cmdData, inbuffer+4, size-7);
      crc = *inbuffer + (size-2);
      return 0;
    }
  protected:
    uint8_t nodeID;
    uint8_t msgID;
    uint8_t cmd;
    uint8_t crc;
    uint8_t cmdData[256];
  private:
    uint8_t cmdDataSize;
  };


}

#endif

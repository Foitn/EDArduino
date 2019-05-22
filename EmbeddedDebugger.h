#ifndef _EMBEDDED_DEBBUGER_H_
#define _EMBEDDED_DEBBUGER_H_

#define REC_SEP 0x33
#include "msg.h"
#if ARDUINO>=100
  #include "ArduinoHardware.h"
#endif

namespace ed{
  class EmbeddedDebugger{
    private:
      uint8_t messageOut[255];
      uint8_t messageIn[255];
      ArduinoHardware hardware;
      Msg *incomingMsg;
      Msg *outgoingMsg;
    public:
      EmbeddedDebugger(){}

      void init(){
        hardware.init();
      }

      void send(const Msg *msg){
        int length = msg->serialize(messageOut);
        hardware.write(messageOut, length);
      }

      void spinOnce(){
        bool addToMessage = false;
        uint8_t messageInIndex = 0;
        while(true){
          int data = hardware.read();
          if(data < 0) break;
          if(data == STX){
            if(addToMessage){
              messageInIndex = 0;
            }
            addToMessage = true;
          }
          if(addToMessage) messageIn[messageInIndex++] = (uint8_t)data;
          if (data == ETX){
            if(!addToMessage) continue;
            incomingMsg = new Msg();
            if(incomingMsg->deserialize(messageIn) == 0){
              dispatchMessage(incomingMsg);
            } else {
              Serial.println("Error deserializing");
            }
            delete(incomingMsg);
            messageInIndex = 0;
          }
        }
      }

      void dispatchMessage(Msg *msg){
        switch(msg->getCmd()){
          case GetVersion:
          {
            uint8_t cmdData[] = {0,0,7,0,0,0,1,0,5,'F','o','i','t','n',1,'1'};
            outgoingMsg = new Msg(0,0,(uint8_t)ed::GetVersion, cmdData, sizeof(cmdData));
            //Serial.println("Sending Version message");
            send(outgoingMsg);
            delete outgoingMsg;
            break;
          }
          case GetInfo:
          {
            uint8_t cmdData[] = {
              0x00,0x01,REC_SEP,
              0x01,sizeof(void*),REC_SEP,
              0x02,sizeof(bool),REC_SEP,
              0x03,sizeof(uint8_t),REC_SEP,
              0x04,sizeof(uint16_t),REC_SEP,
              0x05,sizeof(uint32_t),REC_SEP,
              0x06,sizeof(uint64_t),REC_SEP,
              0x07,sizeof(float),REC_SEP,
              0x08,sizeof(double),REC_SEP,
              0x09,sizeof(double),REC_SEP,
              0x0A,0xE8, 0x03, 0x00, 0x00
            };
            outgoingMsg = new Msg(0,0,(uint8_t)ed::GetInfo, cmdData, sizeof(cmdData));
            //Serial.println("Sending Version message");
            send(outgoingMsg);
            delete outgoingMsg;
            break;
          }
          case QueryRegister:
          {
            uint8_t cmdData[] = {
              0x00, 0x00, 0x00, 0x00, 0x70, 0x01, 0xA1
            };
            outgoingMsg = new Msg(0,0,(uint8_t)ed::QueryRegister, cmdData, sizeof(cmdData));
            send(outgoingMsg);
            delete outgoingMsg;
            break;
          }
          case WriteRegister:
          {
            uint32_t address = incomingMsg->getCmdData()[0]
              | (incomingMsg->getCmdData()[1] << 8)
              | (incomingMsg->getCmdData()[2] << 16)
              | (incomingMsg->getCmdData()[3] << 24);
            memcpy(address, &incomingMsg->getCmdData()[6], msg->getCmdData()[5]);
            uint8_t cmdData[] = {
              0x00
            };
            outgoingMsg = new Msg(0,0,(uint8_t)ed::WriteRegister, cmdData, sizeof(cmdData));
            send(outgoingMsg);
            delete outgoingMsg;
          }
          default:
          {
            //Serial.println("Going to send ack");
            outgoingMsg = new Msg(0,msg->getMsgID(),msg->getCmd());
            //Serial.println("Sending ack");
            send(outgoingMsg);
            delete outgoingMsg;
            break;
          }
        }
      }
  };
}
#endif

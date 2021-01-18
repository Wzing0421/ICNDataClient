#include "MsgReceiver.h"

MsgReceiver::MsgReceiver(unsigned short port, string _GlobalName){
    bindport = port;
    udpMsgReceiver.create(bindport);
    GlobalName = _GlobalName;
    distributer = Distributer::GetInstance(51002);
}

MsgReceiver::~MsgReceiver(){}

void MsgReceiver::ProcMsgReceiver(){
    char recvMsgBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    cout << "Msg Thread start! port is: " << bindport << endl;
    while(true){
        lenrecv = udpMsgReceiver.recvbuf(recvMsgBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpMsgReceiver recv error" << endl;
            break;
        }
        DataPackage datapack;
        memcpy(&datapack, recvMsgBuf, sizeof(datapack));

        if(datapack.end == 1) break;
        // get local time
        time_t now = time(0);
        char* dt = ctime(&now);
        // print short message
        cout << "[Info] Time is: " << dt << "ContentName is : " << datapack.contentName <<  " Message is: " << datapack.data << endl;
        cout << endl;
    }
    distributer->deleteGlobalName(GlobalName);
    distributer->retreivePort(bindport);
    udpMsgReceiver.Close();
    cout << "[Info] Msg Thread Receiver ends!" << endl;
}

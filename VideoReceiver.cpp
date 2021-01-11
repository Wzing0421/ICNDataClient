#include "VideoReceiver.h"

VideoReceiver::VideoReceiver(unsigned short port, string _GlobalName){
    bindport = port;
    udpVideoReceiver.create(bindport);
    GlobalName = _GlobalName;
    distributer = Distributer::GetInstance(51002);
}

VideoReceiver::~VideoReceiver(){}

void VideoReceiver::ProcVideoReceiver(){
    char recvVideoBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    while(true){
        lenrecv = udpVideoReceiver.recvbuf(recvVideoBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpVideoReceiver recv error" << endl;
            break;
        }
        char* contentName = new char[10];
        memcpy(contentName, recvVideoBuf, 10);
        string name = contentName;
        if(name == "Stop Video"){
            delete[] contentName;
            break;
        }
        udpVideoReceiver.sendbuf(recvVideoBuf + 50, lenrecv - 50, "127.0.0.1", 53000);
        delete [] contentName;
    }
    distributer->deleteGlobalName(GlobalName);
    distributer->retreivePort(bindport);
    udpVideoReceiver.Close();
}

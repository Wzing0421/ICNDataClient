#include "Distributer.h"
Distributer::Distributer(unsigned short _port, unsigned short _StartPortForDistribution){
    StartPortForDistribution = _StartPortForDistribution;
    udpSocket.create(_port);
}

Distributer::~Distributer(){
    ContentName2Port.clear();
    udpSocket.Close();
}

Distributer* Distributer::distributerInstance = NULL;
std::mutex distributermtx;

Distributer* Distributer::GetInstance(unsigned short _port){
    if(distributerInstance == NULL){
        std::lock_guard<mutex> lck(distributermtx);
        if(distributerInstance == NULL){
            // default start allocate port is 52000
            distributerInstance = new Distributer(_port, 52000);
        }
    }
    return distributerInstance;
}

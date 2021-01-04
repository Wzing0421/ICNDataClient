#include "Distributer.h"
Distributer::Distributer(unsigned short _port, unsigned short _StartPortForDistribution){
    StartPortForDistribution = _StartPortForDistribution;
    udpSocket.create(_port);
}

Distributer::~Distributer(){
    ContentName2Port.clear();
    udpSocket.Close();
    delete distributerInstance;
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

unsigned short Distributer::allocatePort(){
    std::lock_guard<mutex> allocatemtx(distributermtx);

    // allocate a port. starts from StartPortForDistribution
    unsigned short retport = StartPortForDistribution;

    set<unsigned int>::iterator iter = portSet.begin();
    while(iter != portSet.end()){
        if(retport != *iter){
            portSet.insert(retport);
            return retport;
        }
        retport++;
        iter++;
    }
    portSet.insert(retport);
    return retport;
}

void Distributer::retreivePort(unsigned short _port){
    std::lock_guard<mutex> retreive(distributermtx);
    portSet.erase(_port);
}

void Distributer::distributeProc(){
    char recvDataBuf[1500];
    string srcip_;
    unsigned short sport_;

    while(true){
        int lenrecv = udpSocket.recvbuf(recvDataBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpDataSocket recv error" << endl;
            break;
        }
        DataPackage dataPackage;
        memcpy(&dataPackage, recvDataBuf, sizeof(DataPackage));
        string upperName = getUpperName(dataPackage.contentName);
        if(upperName == ""){
            cout << "[Error]: Invalid contentName! contentName is: " << dataPackage.contentName << endl;
            continue;
        }

        //transfer to receiving process
        unsigned short port;
        if( (port = getPortByContentName(upperName)) != 0){
            udpSocket.sendbuf(recvDataBuf, 1500, "127.0.0.1", port);
        }
        else{
            cout << "[Error]: No UpperName in ContentName2Port! UpperName is: " << upperName << endl;
        }

    }
    udpSocket.Close();
}

string Distributer::getUpperName(string name){
    string upperName = "";
    size_t postion = name.find("segment");
    if(postion == string::npos){
        return upperName;
    }
    return name.substr(0, postion - 1);
}

unsigned short Distributer::getPortByContentName(string name){
    auto iter = ContentName2Port.find(name);
    if(iter != ContentName2Port.end()) return iter->second;
    return 0;
}

bool Distributer::isTaskRunning(string name){
    return ContentName2Port.find(name) != ContentName2Port.end();
}

void Distributer::InsertGlobalName(string GlobalName, unsigned short _port){
    std::lock_guard<mutex> Insertlck(distributermtx);
    ContentName2Port.insert(make_pair(GlobalName, _port));
}

void Distributer::deleteGlobalName(string GlobalName){
    std::lock_guard<mutex> Deletelck(distributermtx);
    ContentName2Port.erase(GlobalName);
}
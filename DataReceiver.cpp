#include "DataReceiver.h"

DataReceiver::DataReceiver(unsigned short port, string _GlobalName){
    bindport = port;
    udpReceiver.create(bindport);
    GlobalName = _GlobalName;
    FileName = getFileName(GlobalName);
    cout << "GlobalName: " << GlobalName << " FileName: "  << FileName << endl;
    if(FileName == ""){
        cout << "[Error] Invalid FileName! FileName is: " << FileName << endl;
        udpReceiver.Close();
        exit(1);
    }
    outfile.open(FileName);
    distributer = Distributer::GetInstance(51002);
}

DataReceiver::~DataReceiver(){}

string DataReceiver::getFileName(string GlobalName){
    string FileName = "";
    vector<string> v;
    SplitString(GlobalName, v, "/");
    if(v.size() < 1){
        cout << "[Error]: Invalid GlobalName! GlobalName is: " << GlobalName << endl; 
        return FileName;
    }
    // 默认倒数第1个是文件名字 eg: pku/eecs/file/test1.txt
    return v[v.size() - 1];
}

void DataReceiver::SplitString(string& s, vector<string>& v, const string& c){
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
 
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

void DataReceiver::ProcTextReceiver(){

    char recvDataBuf[1500];
    string srcip_;
    unsigned short sport_;
    while(true){
        int lenrecv = udpReceiver.recvbuf(recvDataBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpReceiver recv error" << endl;
            break;
        }
        DataPackage dataPackage;
        memcpy(&dataPackage, recvDataBuf, sizeof(DataPackage));
        

        //save to the disk
        //1. save binary file
        //outfile.write(dataPackage.data, dataPackage.datasize);
        
        //2. save Text file
        cout << dataPackage.contentName << endl;
        outfile << dataPackage.data << endl;
        if(dataPackage.end == 1) break;

    }
    
    sleep(1);
    udpReceiver.Close();
    outfile.close();
    distributer->deleteGlobalName(GlobalName);
    distributer->retreivePort(bindport);

    cout << "[Info]: " << GlobalName << " thread receive end. File Name is: " << FileName << endl; 
    cout << "请输入全局订阅事件名称" << endl;
}

void DataReceiver::ProcBinReceiver(){
    char recvDataBuf[1500];
    string srcip_;
    unsigned short sport_;
    while(true){
        int lenrecv = udpReceiver.recvbuf(recvDataBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpReceiver recv error" << endl;
            break;
        }
        DataPackage dataPackage;
        memcpy(&dataPackage, recvDataBuf, sizeof(DataPackage));
        

        //save to the disk
        //1. save binary file
        outfile.write(dataPackage.data, dataPackage.datasize);
        
        //2. save Text file
        //outfile << dataPackage.data << endl;
        if(dataPackage.end == 1) break;

    }
    sleep(1);
    udpReceiver.Close();
    outfile.close();
    distributer->deleteGlobalName(GlobalName);
    distributer->retreivePort(bindport);

    cout << "[Info]: " << GlobalName << " thread receive end. File Name is: " << FileName << endl; 
    cout << "请输入全局订阅事件名称" << endl;
}

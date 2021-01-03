#include "DataReceiver.h"

DataReceiver::DataReceiver(unsigned short port, string _GlobalName){
    bindport = port;
    udpReceiver.create(bindport);
    GlobalName = _GlobalName;
    FileName = getFileName(GlobalName);
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

void DataReceiver::ProcReceiver(){

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
        outfile << dataPackage.data;
        if(dataPackage.end) break;

    }
    udpReceiver.Close();
    outfile.close();
    distributer->deleteGlobalName(GlobalName);
    distributer->retreivePort(bindport);
}

string DataReceiver::getUpperName(string name){
    string upperName = "";
    size_t postion = name.find("segment");
    if(postion == string::npos){
        return upperName;
    }
    return name.substr(0, postion - 1);
}
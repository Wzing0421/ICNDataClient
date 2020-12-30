#include "DataReceiver.h"

DataReceiver::DataReceiver(unsigned short port, string contentname){
    ContentName = contentname;
    udpReceiver.create(port);
    FileName = getFileName(ContentName);
}

DataReceiver::~DataReceiver(){
    udpReceiver.Close();
}

string DataReceiver::getFileName(string ContentName){
    string FileName = "";
    vector<string> v;
    SplitString(ContentName, v, "/");
    if(v.size() <= 2){
        cout << "[Error]: Invalid ContentName! ContentName is: " << ContentName << endl; 
    }
    // 默认倒数第二个是文件名字
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

}

#include "CMD.h"

void SplitString(string& s, vector<string>& v, const string& c);
string getFileName(string GlobalName);
bool judgeBinOrText(string GlobalName);

void *thread_startDataReceiver(void* arg){
    ARGS *p = (ARGS*)arg;
    DataReceiver dataReceiver(p->port, p->GlobalName);
    
    cout << "Create Receiving thread, port is: " << p->port << endl;
    if(judgeBinOrText(p->GlobalName)){
        // a Text file
        dataReceiver.ProcTextReceiver();
    }
    else{
        // a Binary file
        dataReceiver.ProcBinReceiver();
    }
}

/*
 *@Description: judge if a file is a binary file or text file
 *@Author: ZiningWang
 *@Date: 2021-01-04 16:48:47
 *@Return: True if the file is a text file
           False if the file is a binary file
*/
bool judgeBinOrText(string GlobalName){
    string FileName = getFileName(GlobalName);
    if(FileName.find("txt") !=  FileName.npos) return true;
    return false;
}

string getFileName(string GlobalName){
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

void SplitString(string& s, vector<string>& v, const string& c){
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


CMD::CMD(){
    Init();
}

CMD::~CMD(){}

void CMD::Init(){
    Json::Value root;
    Json::Reader reader;

    ifstream ifs("Settings.json");
    if(!reader.parse(ifs, root)){
        printf("[Error] Fail to parse Settings.json");
        return;
    }
    
    //parse Settings.json
    InterestPort = (unsigned short)root["InterestPort"].asUInt();
    DataPort = (unsigned short)root["DataPort"].asUInt();    
    ICNDstIp = root["ICNDstIP"].asString();

    // 端口任意指定
    udpclient.create(20100);
    
    distributer = Distributer::GetInstance(51002);
}

void CMD::SendInterestPackage(string GlobalName){	
    InterestPackage package(GlobalName.c_str());
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);
}



void CMD::processInerestInput(){
        cout << "ICN Data Client Starts!" << endl;
        // 循环获得订阅事件名称
        while (true)
        {
            string GlobalName;
            cout << "请输入全局订阅事件名称" << endl;
            cin >> GlobalName;
            cout << GlobalName << endl;
            //need to first judge if the name container in map:
            //if exists, that means it's just a duplicate task and remind the user
            if(distributer->isTaskRunning(GlobalName)){
                cout << "[Warning] The task of this GlobalName (" << GlobalName <<  " ) is running!" << endl;
                continue;
            }
            //allocate a new port for the new receiving process
            unsigned short port = distributer->allocatePort();
            distributer->InsertGlobalName(GlobalName, port);

            // allocate a port for new process
            pthread_t thid;            
            ARGS arg(GlobalName, port);
            if(pthread_create(&thid, NULL, thread_startDataReceiver, (void*)&arg) != 0){
                cout << "Thread " << thid << "create error" << endl;
                continue;
            }
            sleep(1);
            SendInterestPackage(GlobalName);
            //pthread_join(thid, NULL);
            
        }
        udpclient.Close();
}

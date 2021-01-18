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

void *thread_startVideoReceiver(void *arg){
    ARGS *p = (ARGS*) arg;
    VideoReceiver videoReceiver(p->port, p->GlobalName);
    
    cout << "Create Video Receiving thread, port is: " << p->port << endl;
    videoReceiver.ProcVideoReceiver();
}

void *thread_startMsgReceiver(void *arg){
    ARGS *p = (ARGS*) arg;
    MsgReceiver msgReceiver(p->port, p->GlobalName);

    cout << "Create Msg Receiving thread, port is: " << p->port << endl;
    msgReceiver.ProcMsgReceiver();
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

void CMD::SendSubscribeInterestPackage(string GlobalName){	
    InterestPackage package(GlobalName.c_str(), 1, 0);
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);
}

void CMD::SendUnSubscribeInterestPackage(string GlobalName){
    // 1. notify ICN node
    InterestPackage package(GlobalName.c_str(), 0, 0);
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);

    // 2. notify local receiving thread
    DataPackage dataPacakge;
    dataPacakge.segmentNum = -1;
    unsigned short port = distributer->getPortByContentName(GlobalName);
    if(port != 0){
        char sendbuffer1[1500];
        memset(sendbuffer1, 0, sizeof(sendbuffer1));
        memcpy(sendbuffer1, &dataPacakge, sizeof(dataPacakge));
        udpclient.sendbuf(sendbuffer1, sizeof(sendbuffer1), "127.0.0.1", port);
    }
}

void CMD::SendVideoSubscribeInterestPackage(string GlobalName){
    InterestPackage package(GlobalName.c_str(), 1, 1);
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);
}

void CMD::SendVideoUnSubscribeInterestPackage(string GlobalName){
    // 1. notify ICN node
    InterestPackage package(GlobalName.c_str(), 0, 1);
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);

    // 2. notify local receiving thread
    char sendbuf[10];
    string stopSignal = "Stop Video";
    memcpy(sendbuf, stopSignal.c_str(), stopSignal.size());
    //unsigned short port = distributer->getPortByContentName(GlobalName);
    unsigned short port = 51005;
    if(port != 0){
        udpclient.sendbuf(sendbuf, sizeof(sendbuf), "127.0.0.1", port);
    }
}

void CMD::SendMsgSubscribeInterestPackage(string GlobalName){
    InterestPackage package(GlobalName.c_str(), 1, 2);
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);
}

void CMD::SendMsgUnSubscribeInterestPackage(string GlobalName){
    // 1. notify ICN node
    InterestPackage package(GlobalName.c_str(), 0, 2);
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), ICNDstIp, InterestPort);

    // 2. notify local receiving thread
    DataPackage dataPacakge;
    dataPacakge.end = 1;
    unsigned short port = distributer->getPortByContentName(GlobalName);
    if(port != 0){
        char sendbuffer1[1500];
        memset(sendbuffer1, 0, sizeof(sendbuffer1));
        memcpy(sendbuffer1, &dataPacakge, sizeof(dataPacakge));
        udpclient.sendbuf(sendbuffer1, sizeof(sendbuffer1), "127.0.0.1", port);
    }
}

bool CMD::judgeVideo(string GlobalName){
    return GlobalName.find("video") != GlobalName.npos;
}

bool CMD::judgeFile(string GlobalName){
    return GlobalName.find("file") != GlobalName.npos;
}

void CMD::processInerestInput(){
        cout << "ICN Data Client Starts!" << endl;
        // 循环获得订阅事件名称
        while (true)
        {
            string operation;
            cout << "请选择操作：　如订阅请输入: 1; 如取消订阅请输入: 0" << endl;
            cin >> operation;
            if(operation != "0" && operation != "1"){
                cout << "操作输入有误，请重新输入" << endl;
                continue;
            }
            if(operation == "1"){
                // 订阅操作
                string GlobalName;
                cout << "请输入全局订阅事件名称" << endl;
                cin >> GlobalName;
                
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
                if(judgeVideo(GlobalName)){
                    // video process
                    pthread_t thid;

                    //JUST FOR TEST!!!!            
                    //ARGS arg(GlobalName, port);
                    ARGS arg(GlobalName, 51005);
                    if(pthread_create(&thid, NULL, thread_startVideoReceiver, (void*)&arg) != 0){
                        cout << "Video Thread " << thid << "create error" << endl;
                        continue;
                    }
                    sleep(1);
                    SendVideoSubscribeInterestPackage(GlobalName);
                }
                else if(judgeFile(GlobalName)){
                    // file prcoess
                    pthread_t thid;            
                    ARGS arg(GlobalName, port);
                    if(pthread_create(&thid, NULL, thread_startDataReceiver, (void*)&arg) != 0){
                        cout << "Thread " << thid << "create error" << endl;
                        continue;
                    }
                    sleep(1);
                    SendSubscribeInterestPackage(GlobalName);
                    //pthread_join(thid, NULL);
                }
                else{
                    // msg process
                    pthread_t thid;
                    ARGS arg(GlobalName, port);
                    if(pthread_create(&thid, NULL, thread_startMsgReceiver, (void*)&arg) != 0){
                        cout << "Thread " << thid << "create error" << endl;
                        continue;
                    }
                    sleep(1);
                    SendMsgSubscribeInterestPackage(GlobalName);
                }
                
            }
            else{
                //取消订阅
                string GlobalName;
                cout << "请输入取消订阅事件名称" << endl;
                cin >> GlobalName;

                // only if the task is still working before we can stop it
                if(distributer->isTaskRunning(GlobalName)){
                    if(judgeVideo(GlobalName)){
                        SendVideoUnSubscribeInterestPackage(GlobalName);
                    }
                    else if(judgeFile(GlobalName)){
                        SendUnSubscribeInterestPackage(GlobalName);
                    }
                    else{
                        SendMsgUnSubscribeInterestPackage(GlobalName);
                    }
                    
                }
                else{
                    cout << "当前无此任务!" << endl;
                }
            }
        }
        udpclient.Close();
}

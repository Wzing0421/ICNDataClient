#include "CMD.h"

void *thread_startDataReceiver(void* arg){
    ARGS *p = (ARGS*)arg;
    DataReceiver dataReceiver(p->port, p->GlobalName);
    dataReceiver.ProcReceiver();
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

void CMD::InterestUnitTest_findLocalContent(string str){	
    InterestPackage package(str.c_str());
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
            pthread_join(thid, NULL);
            
        }
        udpclient.Close();
}

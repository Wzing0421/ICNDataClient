#include "UDPSocket.h"
#include "Package.h"
#include "CMD.h"
#include "Distributer.h"
#include "MultipleUDPSocket.h"

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <jsoncpp/json/json.h>

using namespace std;

void *distributeProc(void*){
    Distributer *distributer = Distributer::GetInstance(51002);
    distributer->distributeProc();
}

void *inputProc(void*){
    CMD cmd;
    cmd.processInerestInput();
}
//receive mulcast test
void *receiveMulAviationMsg(void*){
    MultipleUDPSocket muludpsocket;
    // 本地端口任意，组播端口全局分配即可
    muludpsocket.create("225.0.0.1", 51010, 11010);

    char recvMsgBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    cout << "Msg Multicast Aviation Thread start! "  << endl;
    while(true){
        lenrecv = muludpsocket.recvbuf(recvMsgBuf, 1500, srcip_, sport_);
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
    muludpsocket.Close();
}

//receive mulcast test
void *receiveMulMetologicMsg(void*){
    MultipleUDPSocket muludpsocket;
    // 本地端口任意，组播端口全局分配即可
    muludpsocket.create("225.0.0.2", 51011, 11011);

    char recvMsgBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    cout << "Msg Multicast Metrologic Thread start! "  << endl;
    while(true){
        lenrecv = muludpsocket.recvbuf(recvMsgBuf, 1500, srcip_, sport_);
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
    muludpsocket.Close();
}

//receive mulcast test
void *receiveMulAeroVideo(void*){
    MultipleUDPSocket muludpsocket;
    // 本地端口任意，组播端口全局分配即可
    muludpsocket.create("225.0.0.3", 51012, 11012);

    char recvVideoBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    cout << "Msg Multicast Aero Video Thread start! "  << endl;
    while(true){
        lenrecv = muludpsocket.recvbuf(recvVideoBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] Multiple Video udpMsgReceiver recv error" << endl;
            break;
        }
        char* contentName = new char[50];
        memcpy(contentName, recvVideoBuf, 50);
        muludpsocket.sendbuf(recvVideoBuf + 50, lenrecv - 50, "127.0.0.1", 53001);
        // get local time
        time_t now = time(0);
        char* dt = ctime(&now);
        // print short message
        //cout << "[Info] Receive Multiple video, Time is: " << dt << "ContentName is : " << contentName << endl;
        delete contentName;
    }
    muludpsocket.Close();
}

void fileCopy(char *file1, char *file2)  
{  
    // 最好对file1和file2进行判断  
      
    ifstream in(file1);  
    ofstream out(file2);  
    string filename;  
    string line;  
     
    while (getline (in, line))  
    {   
        char buf[1400];
        int i;
        for(i = 0; i < line.size(); i++){
            buf[i] = line[i];
        }
        buf[i] = '\0';
        if(in.eof()){
            cout << "End" << endl;
        }
        else{
            cout << "Not End" << endl;
        }
        out << buf << endl;
          
    }
}  

void binfileCopy(char* file1, char* file2){
    ifstream fin(file1, ios::binary);
    ofstream fout(file2, ios::binary);

    if(!fin){
        cout << "File open error" << endl;
        return;
    }
    char buf[1400];
    while (!fin.eof())
    {
        /* code */
        fin.read(buf, 1400);
        fout.write(buf, fin.gcount());
    }
    fin.close();
    fout.close();
}

void receiveBinFile(char* filename){
    ofstream outfile(filename, ios::binary);
    UDPSocket udpsocket;
    udpsocket.create(51002);
    
    char recvBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    while (true)
    {
        lenrecv = udpsocket.recvbuf(recvBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpSocket recv error" << endl;
            break;
        }
        DataPackage datapackage;
        memcpy(&datapackage, recvBuf, sizeof(DataPackage));
        outfile.write(datapackage.data, datapackage.datasize);
        if(datapackage.end == 1) break;
    }
    udpsocket.Close();
    outfile.close();
}

void receiveTextFile(char* filename){
    ofstream outfile(filename);
    UDPSocket udpsocket;
    udpsocket.create(51002);
    
    char recvBuf[1500];
    string srcip_;
    unsigned short sport_;
    int lenrecv;
    while (true)
    {
        lenrecv = udpsocket.recvbuf(recvBuf, 1500, srcip_, sport_);
        if(lenrecv < 0){
            cout << "[Error] udpSocket recv error" << endl;
            break;
        }
        DataPackage datapackage;
        memcpy(&datapackage, recvBuf, sizeof(DataPackage));
        
        outfile << datapackage.data << endl;
        if(datapackage.end == 1) break;
    }
    udpsocket.Close();
    outfile.close();
}

int main(){
    
    pthread_t thid1, thid2;
    pthread_t thid_mulcast1, thid_mulcast2, thid_mulcast3;
    if(pthread_create(&thid1, NULL, distributeProc, NULL) != 0){
        cout << "distribute process create error!" << endl;
        return -1;
    }
    if(pthread_create(&thid2, NULL, inputProc, NULL) != 0){
        cout << "input process create error!" << endl;
        return -1;
    }
    if(pthread_create(&thid_mulcast1, NULL, receiveMulAviationMsg, NULL) != 0){
        cout << "multiplecast Aviation process create error!" << endl;
        return -1;
    }
    if(pthread_create(&thid_mulcast2, NULL, receiveMulMetologicMsg, NULL) != 0){
        cout << "multiplecast Metrologic process create error!" << endl;
        return -1;
    }
    if(pthread_create(&thid_mulcast3, NULL, receiveMulAeroVideo, NULL) != 0){
        cout << "multiplecast Video Aero process create error!" << endl;
        return -1;
    }

    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);
    pthread_join(thid_mulcast1, NULL);
    pthread_join(thid_mulcast2, NULL);
    pthread_join(thid_mulcast3, NULL);
    // pku/eecs/file/test2.txt
    return 0;
}
                                                                                                                                                          

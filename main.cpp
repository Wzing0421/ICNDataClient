#include "UDPSocket.h"
#include "Package.h"
#include "CMD.h"
#include "Distributer.h"

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;


/*
void DataUnitTest(){
    DataPackage package1("pku/eecs/file/test3.txt", "this is a test", 14, 0, 1);   
    char sendbuf[1470];
    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &package1, sizeof(package1));
    udpclient.sendbuf(sendbuf, sizeof(sendbuf), ICNDstIp, DataPort);
}
*/
void *distributeProc(void*){
    Distributer *distributer = Distributer::GetInstance(51002);
    distributer->distributeProc();
}

void *inputProc(void*){
    CMD cmd;
    cmd.processInerestInput();
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
    /*
    pthread_t thid1, thid2;
    if(pthread_create(&thid1, NULL, distributeProc, NULL) != 0){
        cout << "distribute process create error!" << endl;
        return -1;
    }
    if(pthread_create(&thid2, NULL, inputProc, NULL) != 0){
        cout << "input process create error!" << endl;
        return -1;
    }
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);
    */
    
    //receiveTextFile("test2.txt");
    receiveBinFile("11583.mp3");
    return 0;
}
                                                                                                                                                          

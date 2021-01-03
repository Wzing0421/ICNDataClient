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
    fileCopy("1.txt", "3.txt");
    //binfileCopy("test1.jpg", "test3.jpg");
    return 0;
}
                                                                                                                                                          

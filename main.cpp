#include "UDPSocket.h"
#include "Package.h"

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

using namespace std;

UDPSocket udpclient;
string srcip = "162.105.85.247";
unsigned short srcInterestPort = 51001; 
unsigned short srcDataPort = 51002; 

/**
 * 对于一个订阅事件，需要分配一个对应的端口号进行数据接收．
 * 此map存放订阅事件名称和端口对应关系
 */
unordered_map<string, unsigned short> ContentName2Port;

void InterestUnitTest_findLocalContent(){	
    InterestPackage package("pku/eecs/file/test7.txt");
    char sendbuffer[100];
    memset(sendbuffer, 0, sizeof(sendbuffer));
    memcpy(sendbuffer, &package, sizeof(package));
    udpclient.sendbuf(sendbuffer, sizeof(sendbuffer), srcip, srcInterestPort);
}

void DataUnitTest(){
    DataPackage package1("pku/eecs/file/test1.txt", "this is a test", 14, 0, 1);   
    char sendbuf[1470];
    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &package1, sizeof(package1));
    udpclient.sendbuf(sendbuf, sizeof(sendbuf), srcip, srcDataPort);
}
int main(){
    cout << "ICN Data Client Starts!" << endl;
    // 循环获得订阅事件名称
    while (true)
    {
        string eventName;
        cout << "请输入订阅事件名称" << endl;
        cin >> eventName;
        //allocate a port for new process
    }
    
    udpclient.create(20100);
    InterestUnitTest_findLocalContent();	
    //sleep(3);
    //DataUnitTest();
    udpclient.Close();
    return 0;
}
                                                                                                                                                          

#include "UDPSocket.h"
#include "Package.h"
#include "CMD.h"

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

/**
 * 对于一个订阅事件，需要分配一个对应的端口号进行数据接收．
 * 此map存放订阅事件名称和端口对应关系
 */
unordered_map<string, unsigned short> ContentName2Port;


/*
void DataUnitTest(){
    DataPackage package1("pku/eecs/file/test3.txt", "this is a test", 14, 0, 1);   
    char sendbuf[1470];
    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &package1, sizeof(package1));
    udpclient.sendbuf(sendbuf, sizeof(sendbuf), ICNDstIp, DataPort);
}
*/

int main(){
    CMD cmd;
    cmd.processInerestInput();    
    return 0;
}
                                                                                                                                                          

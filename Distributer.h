#ifndef __DISTRIBUTER_H__
#define __DISTRIBUTER_H__

#include <unordered_map>
#include <set>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <algorithm>

#include "UDPSocket.h"
#include "Package.h"

using namespace std;

/*
 *@Description: Singleton  
 1. get date packages and transfer to different recieveing process
 2. distribute a port for new recieveing process
 3. retrieve ports for receiving process when ends

 *@Author: ZiningWang
 *@Date: 2020-12-31 12:35:53
*/
class Distributer{

private:

    /**
    * 对于一个订阅事件，需要分配一个对应的端口号进行数据接收．
    * 此map存放订阅事件名称和端口对应关系
    */
    unordered_map<string, unsigned short> ContentName2Port;    
    
    /**
     * 存放当前已经分配出去的port
    */
    set<unsigned int> portSet;

    UDPSocket udpSocket;

    unsigned short StartPortForDistribution;
    /*
     *@Description: private construction method
     *@Author: ZiningWang
     *@Date: 2020-12-31 13:06:13
     *@param port: udpclient bind port
     *@param StartPortForDistribution: the starting port number that can be allocated
    */
    Distributer(unsigned short _port, unsigned short _StartPortForDistribution);
    static Distributer* distributerInstance;

    string getUpperName(string name);

    unsigned short getPortByContentName(string name);

public:
    ~Distributer();

    Distributer(Distributer &other) = delete;
    void operator=(const Distributer &) = delete;

    static Distributer *GetInstance(unsigned short _port = 51002);

    unsigned short allocatePort();

    void retreivePort(unsigned short _port);

    /*
     *@Description: check if a contentName exist in ContentName2Port
     *@Author: ZiningWang
     *@Date: 2021-01-01 22:38:34
     *@return: true if name exist. DO NOT execute the task because of duplication.
    */
    bool isTaskRunning(string name);
    
    /*
     *@Description: main process for distributing.
     1. get data packages and transfer to relating port
     *@Author: ZiningWang
     *@Date: 2021-01-01 15:22:23
    */
    void distributeProc();
    
};
#endif
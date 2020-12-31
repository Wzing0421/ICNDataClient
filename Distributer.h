#ifndef __DISTRIBUTER_H__
#define __DISTRIBUTER_H__

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <algorithm>

#include "UDPSocket.h"

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
    
    UDPSocket udpSocket;

    unsigned short StartPortForDistribution;
    /*
     *@Description: private construction method
     *@Author: ZiningWang
     *@Date: 2020-12-31 13:06:13
     *@param port:
     *@param StartPortForDistribution:
    */
    Distributer(unsigned short _port, unsigned short _StartPortForDistribution);
    static Distributer* distributerInstance;


public:
    ~Distributer();

    Distributer(Distributer &other) = delete;
    void operator=(const Distributer &) = delete;

    static Distributer *GetInstance(unsigned short _port = 51002);
    
};
#endif
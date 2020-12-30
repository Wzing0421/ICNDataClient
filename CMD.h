#ifndef __CMD_H__
#define __CMD_H__

#include "UDPSocket.h"
#include "Package.h"
#include "DataReceiver.h"

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
 *@Description: class for CMD Input
 *@Author: ZiningWang
 *@Date: 2020-12-30 11:28:00
*/
class CMD
{
private:
    UDPSocket udpclient;
    unsigned short InterestPort; 
    unsigned short DataPort; 
    string ICNDstIp;

    /*
     *@Description: Init method for CMD constructor
     *@Author: ZiningWang
     *@Date: 2020-12-30 18:04:51
    */
    void Init();

public:
    CMD();
    ~CMD();

    /*
     *@Description: 1. Get user's Interest input and send it to ICN
                    2. Allocates a receiver process for the following data package 
     *@Author: ZiningWang
     *@Date: 2020-12-30 18:09:37
    */
    void processInerestInput();

    void InterestUnitTest_findLocalContent(string str);
};

#endif
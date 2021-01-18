#ifndef __MSGRECEIVER_H__
#define __MSGRECEIVER_H__

#include "UDPSocket.h"
#include "Package.h"
#include "Distributer.h"

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <ctime>
#include <jsoncpp/json/json.h>

using namespace std;

class MsgReceiver
{
private:

    UDPSocket udpMsgReceiver;

    unsigned short bindport;

    /**
     * Overall name for the name without segment
     * eg: pku/eecs/Hangzhen/msg/aeroplane
     */
    string GlobalName;

    Distributer *distributer;

public:
    MsgReceiver(unsigned short port, string _GlobalName);
    ~MsgReceiver();

    void ProcMsgReceiver();
};
#endif
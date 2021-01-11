#ifndef __VIDEORECEIVER_H__
#define __VIDEORECEIVER_H__

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
#include <jsoncpp/json/json.h>

using namespace std;

class VideoReceiver
{
private:

    UDPSocket udpVideoReceiver;

    unsigned short bindport;

    /**
     * Overall name for the name without segment
     * eg: pku/eecs/file/test1.txt
     */
    string GlobalName;

    Distributer *distributer;

public:
    VideoReceiver(unsigned short port, string _GlobalName);
    ~VideoReceiver();

    void ProcVideoReceiver();
};
#endif
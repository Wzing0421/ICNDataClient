#ifndef __DATARECEIVER_H__
#define __DATARECEIVER_H__

#include "UDPSocket.h"
#include "Package.h"

#include <unordered_map>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

class DataReceiver
{
private:

    UDPSocket udpReceiver;

    /**
     * ContentName for ICN, and can be related to filename
     */    
    string ContentName;

    /**
     * FileName for storing data stream
     */
    string FileName;


    /*
     *@Description: Transfer ContentName to FileName
     *@Author: ZiningWang
     *@Date: 2020-12-30 20:12:55
    */
    string getFileName(string ContentName);

    void SplitString(string& s, vector<string>& v, const string& c);

    /*
     *@Description: main process for receiving data package, sorting, writing to file AND Noticing user.
     *@Author: ZiningWang
     *@Date: 2020-12-30 20:42:04
    */
    void ProcReceiver();


public:
    DataReceiver(unsigned short port, string contentname);
    ~DataReceiver();
};
#endif
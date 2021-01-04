#ifndef __DATARECEIVER_H__
#define __DATARECEIVER_H__

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

class DataReceiver
{
private:

    UDPSocket udpReceiver;

    unsigned short bindport;

    /**
     * FileName for storing data stream
     * eg: test1.txt
     */
    string FileName;

    /**
     * Overall name for the name without segment
     * eg: pku/eecs/file/test1.txt
     */
    string GlobalName;

    ofstream outfile;

    Distributer *distributer;

    /*
     *@Description: Transfer ContentName to FileName
     *@Author: ZiningWang
     *@Date: 2020-12-30 20:12:55
    */
    string getFileName(string GlobalName);

    void SplitString(string& s, vector<string>& v, const string& c);

public:
    DataReceiver(unsigned short port, string _GlobalName);
    ~DataReceiver();

    /*
     *@Description: main process for receiving TEXT data package, sorting, writing to file AND Noticing user.
     *@Author: ZiningWang
     *@Date: 2020-12-30 20:42:04
    */
    void ProcTextReceiver();

    /*
     *@Description: main process for receiving BINARY data package, sorting, writing to file AND Noticing user.
     *@Author: ZiningWang
     *@Date: 2021-01-04 16:35:10
    */
    void ProcBinReceiver();
};
#endif
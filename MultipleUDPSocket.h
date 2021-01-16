#pragma once
#ifndef MULTIPLEUDPSOCKET_H
#define MULTIPLEUDPSOCKET_H
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <iostream>
using std::string;
class MultipleUDPSocket
{
     public:
            MultipleUDPSocket();
           ~MultipleUDPSocket();
            int create(string mcastip,int mcastport, unsigned short localport);
            int sendbuf(char *buf,int buflen,string destip,unsigned short destport);
            int recvbuf(char *buf,int buflen,string&srcip,unsigned short &srcport);
            int Close();
            //int sock;
            //int m_sock;
            int socket_fd;
            struct sockaddr_in group_addr;  //group address
            struct sockaddr_in local_addr;//local address
 
};
#endif

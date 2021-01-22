#include "MultipleUDPSocket.h"
#include <iostream>
MultipleUDPSocket::MultipleUDPSocket()
{

}
MultipleUDPSocket::~MultipleUDPSocket()
{
	close(socket_fd);
}

/*int UDPSocket::create(unsigned short port)
{
	//struct sockaddr_in localaddr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		std::cout << "socket initialize error" << std::endl;
		close(sock);
		return -1;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	//int addr_len = sizeof(addr);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
	int ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1)
	{
		std::cout << "socket bind error" << std::endl;
		close(sock);
		return -1;
	}
     return sock;
}*/

int MultipleUDPSocket::create(string mcastip,int mcastport, unsigned short localport)
{
    struct ip_mreq mreq;
    socklen_t addr_len = sizeof(group_addr);
    u_int yes;


    socket_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(socket_fd < 0)
    {
        perror("socket multicast!");
        exit(1);
    }

    //allow multiple sockets to use the same PORT number 
    if (setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
    {
        perror("Reusing ADDR failed");
        exit(1);
    }

    // set up the destination address
    memset(&group_addr,0,sizeof(struct sockaddr_in));
    
    group_addr.sin_family = AF_INET;
    group_addr.sin_port = htons(mcastport);
    group_addr.sin_addr.s_addr = inet_addr(mcastip.c_str());

    // set up the local address
    memset(&local_addr,0,sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(mcastport);              //this port must be the group port
    // bind local address
    if(bind(socket_fd,(struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)
    {
        perror("Binding the multicast!");
        exit(1);
    }
    //use the setsocketopt() to request joining the multicast group
    mreq.imr_multiaddr.s_addr=inet_addr(mcastip.c_str());
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(socket_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
    {
        perror("setsockopt multicast!");
        exit(1);
    }
    return socket_fd;
}

int MultipleUDPSocket::sendbuf(char *buf,int buflen,string destip,unsigned short destport)
{
	struct sockaddr_in dstaddr;
	memset(&dstaddr, 0, sizeof(dstaddr));
	dstaddr.sin_family = AF_INET;
	dstaddr.sin_addr.s_addr = inet_addr(destip.c_str());
	dstaddr.sin_port = htons(destport);
	return sendto(socket_fd, buf, buflen, 0, (struct sockaddr *)&dstaddr, sizeof(dstaddr));
}
int MultipleUDPSocket::recvbuf(char *buf, int buflen, string &srcip, unsigned short  &srcport)
{
	int recvsize;
    socklen_t addr_len = sizeof(group_addr); 
	recvsize = recvfrom(socket_fd,buf,buflen,0,(struct sockaddr *)&group_addr,(socklen_t *)&addr_len);
	if (recvsize > 0)
	{
		srcip = inet_ntoa(group_addr.sin_addr);
		srcport = ntohs(group_addr.sin_port);
	}
	return recvsize;
}
int MultipleUDPSocket::Close()
{
	close(socket_fd);
	return 0;
}


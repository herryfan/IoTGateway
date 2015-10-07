/*
 *  This file implement multicast 
 */

#include <stdlib.h>  
#include <stdio.h>
#include <limits.h>  
#include <memory.h>  
#include <signal.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <netinet/in.h>
#include "DgramMulticast.h"

// implement INET_Address class 

INET_Address::INET_Address(unsigned short port, const char* address, int flag) 
{
    ip_flag_ = flag;

    // check the address is ipv6 or not, 1 means ipv6
    if (flag == 1) {
    
    } else {  
        // FIXME: not to check ip string is OK or not
        bzero(&inet_addr_.in4_, sizeof(inet_addr_.in4_));
        inet_addr_.in4_.sin_family = AF_INET;
        inet_addr_.in4_.sin_port = htons(port);
        inet_addr_.in4_.sin_addr.s_addr = inet_addr(address);
    }

}

INET_Address::~INET_Address()
{

}

bool INET_Address::IfIpV6()
{
    return ip_flag_ == 1;
}

const struct sockaddr_in& INET_Address::ipv4_addr() const
{
    return inet_addr_.in4_;
}

// implement Dgram_Multicast

Dgram_Multicast::Dgram_Multicast(unsigned short port, const char* address, int flag)
:multicast_addr_(port, address, flag)
{
}

Dgram_Multicast::~Dgram_Multicast()
{
}

INET_Address& Dgram_Multicast::GetMulticastAddress()
{
    return multicast_addr_;
}

int Dgram_Multicast::join(int socketfd)
{
    // check address is ipv6 or not
    if (multicast_addr_.IfIpV6()  == 1) 
    {

    }
    else 
    {
    
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = multicast_addr_.ipv4_addr().sin_addr.s_addr;
        mreq.imr_interface.s_addr = htons(INADDR_ANY);
        
        //bind multicast address
        if (bind(socketfd, (struct
                        sockaddr*)&multicast_addr_.ipv4_addr(),sizeof(multicast_addr_.ipv4_addr()))
                < 0)
        {
            printf("Failed to bind the socket\n");
            return -1;
        }

        // add socketfd into multicast group
        if (setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&mreq,
                    sizeof(mreq)) < 0 )
        {
            printf("Failed to setsockopt, cannt join socketfd into group\n");
            return -1;
        }
    }

    return 0;
}








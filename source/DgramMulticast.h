/*
 * This file defines Multicast class
 */

#ifndef __DGRAM_MULTI_CAST_H__
#define __DGRAM_MULTI_CAST_H__

#include <sys/socket.h>
#include <netinet/in.h>


/*
 * Defines wrapper class of adress for IP4 and IP6
 */

class INET_Address {
public:
	INET_Address(unsigned short port, const char* address, int flag=0);
	~INET_Address();

    bool IfIpV6();
    const struct sockaddr_in& ipv4_addr() const ;

private:
	union {
	sockaddr_in in4_;
	sockaddr_in6 in6_;
	} inet_addr_;
	
	int ip_flag_;
};

/*
 *  Defines class of Dgram multicast 
 */
class Dgram_Multicast {
public:
	
    Dgram_Multicast(unsigned short port, const char* address, int flag=0);
	~Dgram_Multicast();
    
    INET_Address& GetMulticastAddress(); 
    int join(int socketfd);


private:
	INET_Address multicast_addr_;
	int option_;
};




#endif /*__DGRAM_MULTI_CAST_H__*/



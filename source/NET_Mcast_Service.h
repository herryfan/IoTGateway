/*
* This file defines multicast service
*/

#ifndef __NET_MCAST_SERVICE_H__
#define __NET_MCAST_SERVICE_H__


#include "Config.h"

class NetMcastService
{
public:

    NetMcastService();
    ~NetMcastService();
    
    int Start();
    void SetMcast(ACE_HANDLE handle);
    
private:
    
    ACE_SOCK_Dgram_Mcast mcast_;
};



#endif //__NET_MCAST_SERVICE_H__





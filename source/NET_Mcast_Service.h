/*
* This file defines multicast service
*/

#ifndef __NET_MCAST_SERVICE_H__
#define __NET_MCAST_SERVICE_H__


#include "Config.h"

class CfgService;

class NetMcastService
{
public:

    NetMcastService();
    ~NetMcastService();

    int join(ACE_HANDLE default_handle = 0);
    void SetConf(CfgService *conf);
    
private:
    
    ACE_SOCK_Dgram_Mcast mcast_;
    CfgService *conf_;
};



#endif //__NET_MCAST_SERVICE_H__





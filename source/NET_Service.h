/*
* This file defines Net service feature for Gateway
*/

#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

#include "Config.h"
#include "NET_Mcast_Service.h"

class NetService
{
public:
    
    NetService();
    ~NetService();
    
    int Init();
    int Start();
    int Close();
    int Run();
    
private:
    NetMcastService mcast_svc_;
    ACE_Reactor reactor_;
    
};

typedef ACE_Singleton<NetService, ACE_Mutex> Net_Service;


#endif /*__NET_SERVICE_H__ */







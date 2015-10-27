/*
* This file implement Multicast service
*/

#include "Config.h"
#include "CfgService.h"
#include "NET_Mcast_Service.h"


NetMcastService::NetMcastService()
{
}

NetMcastService::~NetMcastService()
{
}

int NetMcastService::join(ACE_HANDLE default_handle)
{    
    if (default_handle >= 0)
    {
        mcast_.set_handle(default_handle);
    }

    ACE_INET_Addr mcast;
    mcast.set(conf_->mcast_addr_port_, conf_->mcast_addr_.c_str());
    
    if (mcast_.join(mcast) == -1)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to join into multicast group\n"));

        return -1;
    }

    return 0;
}

void NetMcastService::SetConf(CfgService *conf)
{
    conf_ = conf;
}



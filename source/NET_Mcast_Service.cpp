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

int NetMcastService::Start()
{    
    int code = mcast_.join(Cfg_Service::instance()->GetMcastAddr());

    if (code == -1)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to join into multicast group\n"));

        return -1;
    }

    return 0;
}

void NetMcastService::SetMcast(ACE_HANDLE handle)
{
    mcast_.set_handle(handle);
}













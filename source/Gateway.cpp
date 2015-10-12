/*
*  This file implement Gateway
*/

#include "Config.h"
#include "Gateway.h"
#include "CfgService.h"
#include "NET_Service.h"


int Gateway::Initialize()
{
    cfgsvc_ = new CfgService("gateway.conf");

    if (cfgsvc_ == 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate CfgService object\n"));

        return -1;
    }

    if (cfgsvc_->Init() == -1)
    {
        return -1;
    }

    netsvc_ = new NetService();

    if (netsvc_ == 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate NetService object\n"));

        return -1;
    }

    if (netsvc_->Init() == -1 )
    {
        return -1;
    }
    
    return 0;
}

int Gateway::Start()
{
    return 0;
}

int Gateway::Stop()
{
    return 0;
}

int Gateway::Run()
{
    return 0;
}







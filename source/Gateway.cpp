/*
*  This file implement Gateway
*/

#include "Config.h"
#include "Gateway.h"
#include "CfgService.h"
#include "NET_Service.h"
#include "CoAP_Service.h"


int Gateway::Init()
{
    int code = Cfg_Service::instance()->Init("gateway.conf");
    
    if (code == -1 )
    {
        return -1;
    }

    code = Net_Service::instance()->Init();
    
    if (code == -1 )
    {
        return -1;
    }

    code = CoAP_Service::instance()->Init();

    if (code == -1 )
    {
        return -1;
    }

    Net_Service::instance()->SetMcast(
                            CoAP_Service::instance()->GetMcastHandle());

    Net_Service::instance()->RegHandler(CoAP_Service::instance()->GetEventHandler(),
                                      ACE_Event_Handler::READ_MASK);

    Net_Service::instance()->RegEventHook(CoAPService::EventHook, 
                                (void*)(CoAP_Service::instance()));
                                
    return 0;
}

int Gateway::Start()
{
    int code = Net_Service::instance()->Start();

    if (code == -1)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to start net service \n"));
        return -1;
    }

    return 0;
}

int Gateway::Stop()
{
    Net_Service::instance()->Close();
    
    return 0;
}

int Gateway::Run()
{
    int code = Net_Service::instance()->Run();

    if (code == -1 )
        return -1;

    
    Net_Service::instance()->wait();
    
    return 0;
}







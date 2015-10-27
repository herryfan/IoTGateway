/*
* This file implement Net service
*/

#include "Config.h"
#include "NET_Service.h"

static bool break_netsvc_thread_flag = false;

NetService::NetService()
:reactor_(new ACE_Select_Reactor(), true)
{
}

NetService::~NetService()
{
}

int NetService::Init()
{
    return 0;
}

int NetService::Close()
{
    // break the net service thread,
    // then close the net service
    
    break_netsvc_thread_flag = true;

    reactor_.end_reactor_event_loop();
    reactor_.close();

    // wait for net service thread to be exited
    wait();
    
    return 0;
}

int NetService::Start()
{
    return Run();
}


int NetService::Run()
{
    if (activate() == -1 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate Net Service thread\n"));
        return -1;
    }

    wait();
    
    return 0;
}

int NetService::svc()
{
    ACE_DEBUG((LM_DEBUG,
                "Create Net Service thread\n"));

    int code = 0;    
    reactor_.owner (ACE_OS::thr_self ());

    ACE_Time_Value timeout;
    timeout.sec(3);

    for (;;)
    {        
        code = reactor_.handle_events(timeout);

        if (break_netsvc_thread_flag)
        {
            break;
        }
    }
            
    ACE_DEBUG((LM_DEBUG,
                "Return from Net Service thread[%d]\n",code));

    return code;
}

int NetService::RegHandler(ACE_Event_Handler *event_handler,
               ACE_Reactor_Mask mask)
{
    return reactor_.register_handler(event_handler, mask);
}

long NetService::schedule_timer(ACE_Event_Handler *event_handler,
                           const void *arg,
                           const ACE_Time_Value &delay)
{
    return reactor_.schedule_timer(event_handler, arg, delay);
}

void NetService::SetConf(CfgService *conf)
{
    svc_conf_ = conf;
    mcast_svc_.SetConf(svc_conf_);
}

int NetService::join(ACE_HANDLE handle)
{
    if (svc_conf_ == 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Please set config service first\n"));

        return -1;
    }

    return mcast_svc_.join(handle);
}

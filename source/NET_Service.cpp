/*
* This file implement Net service
*/

#include "Config.h"
#include "NET_Service.h"

static bool break_netsvc_thread_flag = false;

NetService::NetService()
:reactor_(new ACE_Select_Reactor(), true),
hook_(0),
hook_argv_(0)
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
    
    return 0;
}

int NetService::Start()
{
    if (mcast_svc_.Start() == -1 )
    {
        return -1;
    }
    
    return 0;
}


int NetService::Run()
{

    int code = activate();

    if (code == -1 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate Net Service thread\n"));
        return -1;
    }
    
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
        if (hook_ != 0)
        {
          code = (*hook_)(hook_argv_, timeout);
        }
        
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

void NetService::SetMcast(ACE_HANDLE handle)
{
    mcast_svc_.SetMcast(handle);
}

int NetService::RegHandler(ACE_Event_Handler *event_handler,
               ACE_Reactor_Mask mask)
{
    return reactor_.register_handler(event_handler, mask);
}

int NetService::RegEventHook(EXT_EVENT_HOOK hook, void* argv)
{
    hook_ = hook;
    hook_argv_ = argv;

    return 0;
}


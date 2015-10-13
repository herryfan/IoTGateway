/*
* This file implement Net service
*/

#include "Config.h"
#include "NET_Service.h"

static bool break_netsvc_thread_flag = false;

// Net Service thread.
// do event loop
static void net_service_thread(void* argv)
{
    ACE_DEBUG((LM_DEBUG,
                "Create Net Service thread\n"));

    int code = 0;

    ACE_Reactor* reactor = (ACE_Reactor*)(argv);
    reactor->owner (ACE_OS::thr_self ());
    
    ACE_Time_Value timeout;
    timeout.sec(3);

    for (;;)
    {
        code = reactor->handle_events(timeout);

        if (break_netsvc_thread_flag)
        {
            break;
        }
    }
            
    ACE_DEBUG((LM_DEBUG,
                "Return from Net Service thread[%d]\n",code));
    
}

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
    
    return 0;
}

int NetService::Start()
{
    if (mcast_svc_.Start() == -1 )
    {
        return -1;
    }

    // registe self into Reactor
    int code = reactor_.register_handler(&mcast_svc_, 
                                      ACE_Event_Handler::READ_MASK);

    if (code == -1 )
        return -1;
    
    return 0;
}


int NetService::Run()
{

    int code = ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)net_service_thread, (void*)&reactor_);

    if (code == -1 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate Net Service thread\n"));
        return -1;
    }
    
    return 0;
}



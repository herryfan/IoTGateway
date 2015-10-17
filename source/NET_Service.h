/*
* This file defines Net service feature for Gateway
*/

#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

#include "Config.h"
#include "NET_Mcast_Service.h"

typedef int (*EXT_EVENT_HOOK)(void* argv, ACE_Time_Value& value);

class NetService : public ACE_Task_Base
{
public:
    
    NetService();
    ~NetService();
    
    int Init();
    int Start();
    int Close();
    int Run();

    void SetMcast(ACE_HANDLE handle);
    int RegHandler(ACE_Event_Handler *event_handler,
                   ACE_Reactor_Mask mask);

    int RegEventHook(EXT_EVENT_HOOK hook, void* argv);
    virtual int svc (void);
    
private:
    
    NetMcastService mcast_svc_;
    ACE_Reactor reactor_;
    EXT_EVENT_HOOK hook_;
    void* hook_argv_;
    
    
};

typedef ACE_Singleton<NetService, ACE_Mutex> Net_Service;


#endif /*__NET_SERVICE_H__ */







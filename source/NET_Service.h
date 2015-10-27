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

    void SetConf(CfgService *conf);
    int join(ACE_HANDLE handle);
    int RegHandler(ACE_Event_Handler *event_handler,
                   ACE_Reactor_Mask mask);
    long schedule_timer(ACE_Event_Handler *event_handler,
                               const void *arg,
                               const ACE_Time_Value &delay);    
    virtual int svc (void);
    
private:

    int Run();

    CfgService *svc_conf_;
    NetMcastService mcast_svc_;
    ACE_Reactor reactor_;
    
};

typedef ACE_Singleton<NetService, ACE_Mutex> Net_Service;


#endif /*__NET_SERVICE_H__ */







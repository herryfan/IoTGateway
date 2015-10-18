/*
* This file defines CoAP Service
*/

#ifndef __COAP_SERVICE_H__
#define __COAP_SERVICE_H__

#include "Config.h"

class RDService;
class CoAPEventHandler;
struct coap_context_t;



class CoAPService
{
public:

    CoAPService();
    ~CoAPService();

    static int EventHook(void* argv, ACE_Time_Value& tm);
    
    int Init();
    int Start();
    int Close();
    int Run();
    
    ACE_HANDLE GetMcastHandle();
    ACE_Event_Handler* GetEventHandler();
    int EventHookHandler(ACE_Time_Value& tm);
    
private:

    RDService* rd_service_;
    CoAPEventHandler* evt_handler_;
    struct coap_context_t* coap_ctx_;
    
};

typedef ACE_Singleton<CoAPService, ACE_Mutex> CoAP_Service;


#endif /* __COAP_SERVICE_H__*/







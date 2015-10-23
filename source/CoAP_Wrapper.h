/*
* This file defines libcoap wrapper
*/

#ifndef __COAP_WRAPPER_H__
#define __COAP_WRAPPER_H__

#include "Config.h"
#include "CoAP_Resource.h"


class ACE_Thread_Mutex;
struct coap_context_t;
class ACE_Time_Value;


class CoAPWrapper
{
public:
    CoAPWrapper();
    ~CoAPWrapper();

    int Create(const char *node, const char *port, int debug_level);
    int DoEventDispatch();
    ACE_HANDLE GetMcastHandle();
    int EventHookHandler(ACE_Time_Value& tm);
    int CreateResource(std::string uri,
                        CoAP_Attr& attr,
                        CoAPResource::method_handler_t& method_handler);
    


private:

    struct coap_context_t* coap_ctx_;
    ACE_Thread_Mutex* coap_mutex_;
    
};


#endif //__COAP_WRAPPER_H__






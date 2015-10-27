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

    int Create(ACE_TString &node, int port, int debug_level);
    int handle_event();
    ACE_HANDLE get_handle();
    int time_out(ACE_Time_Value& tm);
    int CreateResource(std::string uri,
                        CoAP_Attr& attr,
                        CoAPResource::method_handler_t& method_handler);
    


private:

    struct coap_context_t *coap_ctx_;
    ACE_Thread_Mutex *coap_mutex_;
    
};


#endif //__COAP_WRAPPER_H__






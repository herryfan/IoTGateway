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

typedef struct CoAPCallback
{
    void *context_;
    void *resource_;
    void *peer_;
    void *request_;
    void *token;
    void *response_;
    
} CoAPCallback;

class CoAPWrapper
{
public:
    CoAPWrapper();
    ~CoAPWrapper();

    int Create(ACE_TString &node, int port, int debug_level);
    int enable_proxy();
    int handle_event();
    ACE_HANDLE get_handle();
    int time_out(ACE_Time_Value& tm);
    void default_handle_request(CoAPCallback &callback);
    void bad_request(CoAPCallback &callback);
    void changed_request(CoAPCallback &callback);
    void ok_request(CoAPCallback &callback);
    void internal_err_request(CoAPCallback &callback);
    
    void get_default_con(CoAPCallback &callback, std::string &con);
    void get_payload(CoAPCallback &callback, std::string &payload);
    
    void get_query_value(CoAPCallback &callback, 
                         std::string key,
                         std::string &value);
    void get_query_value(CoAPCallback &callback, 
                         std::string key,
                         int &value);
                         
    void get_domain_and_endpointIdentifier(CoAPCallback &callback,
                                           std::string &domain,
                                           std::string &ep);
    void* CreateResource(std::string uri,
                        CoAP_Attr& attr,
                        CoAPResource::method_handler_t& method_handler,
                        CoAPResource *app_context);
    


private:

    struct coap_context_t *coap_ctx_;
    ACE_Thread_Mutex *coap_mutex_;
    
};

#endif //__COAP_WRAPPER_H__






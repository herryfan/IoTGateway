/*
* This file implements Forward Proxy service for coap
*/

#include "CfgService.h"
#include "NET_Service.h"
#include "FW_Proxy_Service.h"
#include "CoAP_Wrapper.h"


FWProxyService::FWProxyService(CfgService *conf, NetService *net)
:conf_(conf),
net_(net),
coap_wrapper_(0)
{
}

FWProxyService::~FWProxyService()
{
}

int FWProxyService::Init()
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    if ((coap_wrapper_ = new CoAPWrapper()) == 0)
    {
        ACE_DEBUG((LM_DEBUG, "Failed to allocate CoAPWrapper in Proxy\n"));
        return -1;
    }

    if (coap_wrapper_->Create(conf_->proxy_addr_, 
                              conf_->proxy_addr_port_,
                              conf_->coap_debug_level_
                              ) < 0)
    {

        ACE_DEBUG((LM_DEBUG,"Failed to create coap\n"));
        return -1;
    }

    coap_wrapper_->enable_proxy();
    
    net_->RegHandler(this, ACE_Event_Handler::READ_MASK);
    net_->schedule_timer(this, 0, timeout);
    
    return 0;

}

int FWProxyService::Close()
{
    if (coap_wrapper_)
    {
        delete coap_wrapper_;
    }
       
    return 0;
}

int FWProxyService::handle_input (ACE_HANDLE fd)
{
    ACE_DEBUG((LM_DEBUG, "recv FWProxyService handle_input message\n "));

    if (coap_wrapper_)
        coap_wrapper_->handle_event();
        
    return 0;   
}

int FWProxyService::handle_timeout (const ACE_Time_Value &tv,
                                    const void *arg)
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    coap_wrapper_->time_out(timeout);
    net_->schedule_timer(this, 0, timeout);
    
    return 0;
}

ACE_HANDLE FWProxyService::get_handle (void) const
{
    ACE_DEBUG((LM_DEBUG,
               "call FWProxyService get_handle\n"));
               
    if (coap_wrapper_)
    {
        return coap_wrapper_->get_handle();
    }

    return -1;    
}




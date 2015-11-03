/*
*  This file implement Resource directory service
*/

#include "RD_Service.h"
#include "CoAP_Wrapper.h"
#include "CoAP_RD_Resource.h"
#include "CoAP_RD_Lookup_Resource.h"
#include "CfgService.h"
#include "NET_Service.h"


RDService::RDService(CfgService *conf, NetService *net)
:conf_(conf),
net_(net),
coap_wrapper_(0),
rd_resource_(0),
lookup_resource_(0)
{
}

RDService::~RDService()
{   
}

int RDService::Init()
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    if ((coap_wrapper_ = new CoAPWrapper()) == 0)
    {
        ACE_DEBUG((LM_DEBUG, "Failed to allocate CoAPWrapper in RD\n"));
        return -1;
    }

    if (coap_wrapper_->Create(conf_->rd_addr_, 
                              conf_->rd_addr_port_,
                              conf_->coap_debug_level_
                              ) < 0)
    {

        ACE_DEBUG((LM_DEBUG,"Failed to create coap\n"));
        return -1;
    }

    if ((rd_resource_ = new CoAP_RD_Resource(coap_wrapper_)) == 0)
    {
        ACE_DEBUG((LM_DEBUG, "Failed to allocate rd resource in RD\n"));
        return -1;
    }

    if ((rd_resource_->Create()) == 0)
    {
        ACE_DEBUG((LM_DEBUG,"Failed to craete rd resource\n"));
        return -1;
    }
    
    if ((lookup_resource_ = new CoAPRDLookUpResource(coap_wrapper_)) == 0)
    {
        ACE_DEBUG((LM_DEBUG, "Failed to allocate lookup resource in RD\n"));
        return -1;
    }

    if ((lookup_resource_->Create()) == 0)
    {
        ACE_DEBUG((LM_DEBUG,"Failed to craete lookup resource\n"));
        return -1;
    }

    net_->join(coap_wrapper_->get_handle());
    net_->RegHandler(this, ACE_Event_Handler::READ_MASK);
    net_->schedule_timer(this, 0, timeout);
    
    return 0;
}

int RDService::Close()
{
    if (coap_wrapper_)
    {
        delete coap_wrapper_;
    }

    if (rd_resource_ )
    {
        delete rd_resource_;
    }

    if (lookup_resource_)
    {
        delete lookup_resource_;
    }
        
    return 0;
}

int RDService::handle_input (ACE_HANDLE fd)
{
    if (coap_wrapper_)
        coap_wrapper_->handle_event();
        
    return 0;
}

int RDService::handle_timeout (const ACE_Time_Value &tv,
                            const void *arg)
{
    ACE_Time_Value timeout;
    timeout.sec(5);
    
    coap_wrapper_->time_out(timeout);
    net_->schedule_timer(this, 0, timeout);
    
    return 0;
}

ACE_HANDLE RDService::get_handle (void) const
{
    ACE_DEBUG((LM_DEBUG,
               "call RDService get_handle\n"));
               
    if (coap_wrapper_)
    {
        return coap_wrapper_->get_handle();
    }

    return -1;    
}


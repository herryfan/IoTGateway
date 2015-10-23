/*
* This file implement CoAP Service
*/

#include "Config.h"
#include "CoAP_Service.h"
#include "CoAP_Event_Handler.h"
#include "CfgService.h"
#include "RD_Service.h"
#include "CoAP_Wrapper.h"

CoAPService::CoAPService()
:coap_ctx_(0)
{
}


CoAPService::~CoAPService()
{
}


int CoAPService::Init()
{
    ACE_TString svc_ip;
    char svc_port[10];
    int coap_debug_level;

    ACE_INET_Addr& svc_addr = Cfg_Service::instance()->GetServerAddr();
    coap_debug_level = Cfg_Service::instance()->GetCoapDebugLevel();

    svc_ip = svc_addr.get_host_addr();
    sprintf(svc_port,"%d",svc_addr.get_port_number());

    
    coap_ctx_ = new CoAPWrapper(); 

    if (coap_ctx_ == 0 || coap_ctx_->Create(svc_ip.c_str(), svc_port, 
                                                    coap_debug_level) < 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate CoAP context\n"));

        return -1;
    }

    evt_handler_ = new CoAPEventHandler(coap_ctx_);
    ACE_ASSERT(evt_handler_ != 0);

    rd_service_ = new RDService(coap_ctx_);
    ACE_ASSERT(rd_service_);
    
    rd_service_->Init();
  
    return 0;
}


int CoAPService::Start()
{
    return 0;
}


int CoAPService::Close()
{
    // FIXME-be careful, maybe net service uses evt_handler,
    // so, before deleting evt handler, net service must unregiste it first.
    if ( evt_handler_ )
        delete evt_handler_;

    if ( rd_service_ )
    {
        rd_service_->Close();
        delete rd_service_;
    }

    if ( coap_ctx_ )
    {
        delete coap_ctx_;
    }
        
    return 0;
}

int CoAPService::Run()
{
    return 0;
}

ACE_HANDLE CoAPService::GetMcastHandle()
{
    if ( coap_ctx_)
        return coap_ctx_->GetMcastHandle();
        
    return -1;
}

ACE_Event_Handler* CoAPService::GetEventHandler()
{
    if (evt_handler_)
    {
        return evt_handler_;
    }

    return 0;
}

int CoAPService::EventHook(void* argv, ACE_Time_Value& tm)
{
    CoAPService* coap_svc = (CoAPService*)argv;

    return coap_svc->EventHookHandler(tm);
}

int CoAPService::EventHookHandler(ACE_Time_Value& tm)
{
    if (coap_ctx_)
        coap_ctx_->EventHookHandler(tm);
        
    return 0;
}







/*
* This file implement CoAP Service
*/

#include "Config.h"
#include "coap.h"
#include "CoAP_Service.h"
#include "CoAP_Event_Handler.h"
#include "CfgService.h"
#include "RD_Service.h"

static coap_context_t *
get_context(const char *node, const char *port)
{
    coap_context_t *ctx = NULL;
    int s;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Coap uses UDP */
    //hints.ai_flags = AI_PASSIVE;

    ACE_DEBUG((LM_DEBUG,
                "gataddrinfo: %s-%s\n", node, port));

    s = getaddrinfo("::", "5683", &hints, &result);
    if ( s != 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                   "getaddrinfo: %s\n",gai_strerror(s)));
                   
        return NULL;
    }

    /* iterate through results until success */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        coap_address_t addr;

        if (rp->ai_addrlen <= sizeof(addr.addr))
        {
            coap_address_init(&addr);
            addr.size = rp->ai_addrlen;
            memcpy(&addr.addr, rp->ai_addr, rp->ai_addrlen);

            ctx = coap_new_context(&addr);
            if (ctx)
            {
                /* TODO: output address:port for successful binding */
                goto finish;
            }
        }
    }

    ACE_DEBUG((LM_DEBUG,
                "no context available for interface '%s'\n", node));
                
finish:
    freeaddrinfo(result);
    return ctx;
}

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

    ACE_INET_Addr& svc_addr = Cfg_Service::instance()->GetServerAddr();

    svc_ip = svc_addr.get_host_addr();
    sprintf(svc_port,"%d",svc_addr.get_port_number());

    coap_set_log_level(LOG_WARNING);
    
    coap_ctx_ = get_context(svc_ip.c_str(), svc_port); 

    if (coap_ctx_ == 0)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to allocate CoAP context\n"));

        return -1;
    }

    evt_handler_ = new CoAPEventHandler(coap_ctx_);
    assert(evt_handler_ != 0);

    rd_service_ = new RDService(coap_ctx_);
    assert(rd_service_);
    
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
        coap_free_context(coap_ctx_);
        
    return 0;
}

int CoAPService::Run()
{
    return 0;
}

ACE_HANDLE CoAPService::GetMcastHandle()
{
    if (coap_ctx_)
    {
        return coap_ctx_->sockfd;
    }

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
    struct timeval tv, *timeout;
    int result;
    coap_tick_t now;
    coap_queue_t *nextpdu;
    coap_context_t  *ctx;
    
    if (coap_ctx_ == 0 )
        return 0;

    ctx = coap_ctx_;
    nextpdu = coap_peek_next( ctx );

    coap_ticks(&now);
    while ( nextpdu && nextpdu->t <= now )
    {
        coap_retransmit( ctx, coap_pop_next( ctx ) );
        nextpdu = coap_peek_next( ctx );
    }

    if ( nextpdu && nextpdu->t <= now + COAP_RESOURCE_CHECK_TIME )
    {
        /* set timeout if there is a pdu to send before our automatic timeout occurs */
        tv.tv_usec = ((nextpdu->t - now) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
        tv.tv_sec = (nextpdu->t - now) / COAP_TICKS_PER_SECOND;
        timeout = &tv;
    }
    else
    {
        tv.tv_usec = 0;
        tv.tv_sec = COAP_RESOURCE_CHECK_TIME;
        timeout = &tv;
    }

    tm.set(*timeout);

    return 0;
}







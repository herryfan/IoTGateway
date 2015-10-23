/*
* This file implement libcoap wrapper
*/

#include "Config.h"
#include "coap.h"
#include "CoAP_Wrapper.h"

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


CoAPWrapper::CoAPWrapper()
:coap_mutex_(new ACE_Thread_Mutex()),
coap_ctx_(0)
{
    ACE_ASSERT(coap_mutex_ != 0);
}

CoAPWrapper::~CoAPWrapper()
{
}

int CoAPWrapper::Create(const char *node, const char *port, int debug_level)
{
    ACE_Guard<ACE_Thread_Mutex> guard(*coap_mutex_);
    
    coap_set_log_level(debug_level);

    coap_ctx_ = get_context(node, port);  
    
    if (coap_ctx_ == 0)
    {
        ACE_DEBUG((LM_DEBUG,
                "Failed to allocate CoAP context\n"));

        return -1;
    }
    
    return 0;
}

int CoAPWrapper::DoEventDispatch()
{
    // FIXME: becareful, dont use ACE_Guard at here
    // maybe cause thread died locked
     
    if ( coap_ctx_ )
    {
        coap_read( coap_ctx_ );   /* read received data */
        coap_dispatch( coap_ctx_ );   /* and dispatch PDUs from receivequeue */
    }

    return 0;
}

ACE_HANDLE CoAPWrapper::GetMcastHandle()
{
    ACE_Guard<ACE_Thread_Mutex> guard(*coap_mutex_);
    
    if (coap_ctx_)
    {
        return coap_ctx_->sockfd;
    }

    return -1;
}

int CoAPWrapper::EventHookHandler(ACE_Time_Value& tm)
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

int CoAPWrapper::CreateResource(std::string uri,
                    CoAP_Attr& attr,
                    CoAPResource::method_handler_t& method_handler)
{        
    coap_resource_t *r;

    r = coap_resource_init((const unsigned char*)uri.c_str(), uri.length(), 0);

    if (r == 0 )
    {
        ACE_DEBUG((LM_DEBUG,
        "Failed to allocate %s resource\n", uri.c_str()));
        return -1;
    }
        
    coap_register_handler(r, COAP_REQUEST_GET, (coap_method_handler_t)method_handler.handler_get);
    coap_register_handler(r, COAP_REQUEST_POST, (coap_method_handler_t)method_handler.handler_post);
    coap_register_handler(r, COAP_REQUEST_PUT, (coap_method_handler_t)method_handler.handler_put);
    coap_register_handler(r, COAP_REQUEST_DELETE, (coap_method_handler_t)method_handler.handler_delete);

    CoAP_Attr::iterator e = attr.begin();

    for (; e != attr.end(); ++e)
    {
        std::string key = e->first;
        std::string value = e->second;
        
        coap_add_attr(r, (unsigned char *)key.c_str(), key.length() , 
                         (unsigned char *)value.c_str(), value.length(), 0);                        
    }
    
    coap_add_resource(coap_ctx_, r);
    
}













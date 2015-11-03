/*
* This file implement libcoap wrapper
*/

#include "Config.h"
#include "coap.h"
#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"

//-----------------------------------------------------------------------

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

    s = getaddrinfo(node, port, &hints, &result);
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

static int
parse_param(unsigned char *search, size_t search_len,
            unsigned char *data, size_t data_len, str *result)
{

    if (result)
        memset(result, 0, sizeof(str));

    if (!search_len)
        return 0;

    while (search_len <= data_len)
    {

        /* handle parameter if found */
        if (memcmp(search, data, search_len) == 0)
        {
            data += search_len;
            data_len -= search_len;

            /* key is only valid if we are at end of string or delimiter follows */
            if (!data_len || *data == '=' || *data == '&')
            {
                while (data_len && *data != '=')
                {
                    ++data;
                    --data_len;
                }

                if (data_len > 1 && result)
                {
                    /* value begins after '=' */

                    result->s = ++data;
                    while (--data_len && *data != '&')
                    {
                        ++data;
                        result->length++;
                    }
                }

                return 1;
            }
        }

        /* otherwise proceed to next */
        while (--data_len && *data++ != '&')
            ;
    }

    return 0;
}

static void query_value(coap_pdu_t *pdu, 
                            std::string &key, 
                            std::string &value)
{
    coap_opt_iterator_t opt_iter;
    coap_opt_t *query;
    str v = {0, NULL};

    query = coap_check_option(pdu, COAP_OPTION_URI_QUERY, &opt_iter);

    if (query) 
    {   
        parse_param((unsigned char *)key.c_str(), key.length(), 
		    COAP_OPT_VALUE(query), COAP_OPT_LENGTH(query), &v);
    }
    
    if (v.length > 0)
        value.assign((const char*)v.s, v.length);
}

static void find_domain_ep_from_query(coap_pdu_t *pdu, 
                                      std::string &domain, 
                                      std::string &ep)
{
    std::string domain_key = "d";
    std::string ep_key = "ep";

    query_value(pdu, domain_key, domain);
    query_value(pdu, ep_key, ep);   
}

static void
make_bad_request(CoAPCallback &callback, int code)
{
    unsigned char buf[3];
    coap_pdu_t *response = (coap_pdu_t*)callback.response_;

    response->hdr->code = COAP_RESPONSE_CODE(code);

    coap_add_option(response, COAP_OPTION_CONTENT_TYPE,
                    coap_encode_var_bytes(buf, COAP_MEDIATYPE_APPLICATION_LINK_FORMAT), buf);

    coap_add_option(response, COAP_OPTION_MAXAGE,
                    coap_encode_var_bytes(buf, 0x2ffff), buf);   

}

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

static size_t
address_to_string(const struct coap_address_t *addr, unsigned char *buf, size_t len)
{
#ifdef HAVE_ARPA_INET_H
    const void *addrptr = NULL;
    in_port_t port;
    unsigned char *p = buf;

    switch (addr->addr.sa.sa_family)
    {
    case AF_INET:
        addrptr = &addr->addr.sin.sin_addr;
        port = ntohs(addr->addr.sin.sin_port);
        break;
    case AF_INET6:
        if (len < 7) /* do not proceed if buffer is even too short for [::]:0 */
            return 0;

        addrptr = &addr->addr.sin6.sin6_addr;
        port = ntohs(addr->addr.sin6.sin6_port);

        break;
    default:
        memcpy(buf, "(unknown address type)", min(22, len));
        return min(22, len);
    }

    if (inet_ntop(addr->addr.sa.sa_family, addrptr, (char *)p, len) == 0)
    {
        perror("coap_print_addr");
        return 0;
    }

    p += strnlen((char *)p, len);

    if (addr->addr.sa.sa_family == AF_INET6)
    {
        if (p < buf + len)
        {
        }
        else
            return 0;
    }

    p += snprintf((char *)p, buf + len - p + 1, ":%d", port);

    return buf + len - p;
#else /* HAVE_ARPA_INET_H */
# if WITH_CONTIKI
    unsigned char *p = buf;
    uint8_t i;
#  if WITH_UIP6
    const unsigned char hex[] = "0123456789ABCDEF";

    if (len < 41)
        return 0;

    *p++ = '[';

    for (i=0; i < 16; i += 2)
    {
        if (i)
        {
            *p++ = ':';
        }
        *p++ = hex[(addr->addr.u8[i] & 0xf0) >> 4];
        *p++ = hex[(addr->addr.u8[i] & 0x0f)];
        *p++ = hex[(addr->addr.u8[i+1] & 0xf0) >> 4];
        *p++ = hex[(addr->addr.u8[i+1] & 0x0f)];
    }
    *p++ = ']';
#  else /* WITH_UIP6 */
#   warning "IPv4 network addresses will not be included in debug output"

    if (len < 21)
        return 0;
#  endif /* WITH_UIP6 */
    if (buf + len - p < 6)
        return 0;

#ifdef HAVE_SNPRINTF
    p += snprintf((char *)p, buf + len - p + 1, ":%d", uip_htons(addr->port));
#else /* HAVE_SNPRINTF */
    /* @todo manual conversion of port number */
#endif /* HAVE_SNPRINTF */

    return p - buf;
# else /* WITH_CONTIKI */
    /* TODO: output addresses manually */
#   warning "inet_ntop() not available, network addresses will not be included in debug output"
# endif /* WITH_CONTIKI */
    return 0;
#endif
}

//-----------------------------------------------------------------------

CoAPWrapper::CoAPWrapper()
:coap_mutex_(new ACE_Thread_Mutex()),
coap_ctx_(0)
{
    ACE_ASSERT(coap_mutex_ != 0);
}

CoAPWrapper::~CoAPWrapper()
{
}

int CoAPWrapper::Create(ACE_TString &node, int port, int debug_level)
{
    char port_buf[20];

    sprintf(port_buf, "%d", port);
    port_buf[19] = '\0';
    
    ACE_Guard<ACE_Thread_Mutex> guard(*coap_mutex_);
    
    coap_set_log_level(debug_level);

    coap_ctx_ = get_context(node.c_str(), port_buf);  
    
    if (coap_ctx_ == 0)
    {
        ACE_DEBUG((LM_DEBUG,
                "Failed to allocate CoAP context\n"));

        return -1;
    }
    
    return 0;
}

int CoAPWrapper::handle_event()
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

ACE_HANDLE CoAPWrapper::get_handle()
{
    ACE_Guard<ACE_Thread_Mutex> guard(*coap_mutex_);
    
    if (coap_ctx_)
    {
        return coap_ctx_->sockfd;
    }

    return -1;
}

int CoAPWrapper::time_out(ACE_Time_Value& tm)
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

void* CoAPWrapper::CreateResource(std::string uri,
                    CoAP_Attr& attr,
                    CoAPResource::method_handler_t& method_handler,
                    CoAPResource *app_context)
{        
    coap_resource_t *r;
    str copy = {0, NULL};

    copy.s = (unsigned char*)coap_malloc(uri.length());
    copy.length = uri.length();

    memcpy(copy.s, uri.c_str(), copy.length);
    

    r = coap_resource_init_with_app_context(copy.s, 
                                            copy.length, 
                                            COAP_ATTR_FLAGS_RELEASE_NAME,
                                            (void*)app_context);

    if (r == 0 )
    {
        ACE_DEBUG((LM_DEBUG,
        "Failed to allocate %s resource\n", uri.c_str()));
        return 0;
    }
        
    coap_register_handler(r, COAP_REQUEST_GET, (coap_method_handler_t)method_handler.handler_get);
    coap_register_handler(r, COAP_REQUEST_POST, (coap_method_handler_t)method_handler.handler_post);
    coap_register_handler(r, COAP_REQUEST_PUT, (coap_method_handler_t)method_handler.handler_put);
    coap_register_handler(r, COAP_REQUEST_DELETE, (coap_method_handler_t)method_handler.handler_delete);

    CoAP_Attr::iterator e = attr.begin();

    for (; e != attr.end(); ++e)
    {
        str k = {0, NULL}, v={0, NULL};
        std::string key = e->first;
        std::string value = e->second;
        
        k.s = (unsigned char*)coap_malloc(key.length());
        k.length = key.length();
        v.s = (unsigned char*)coap_malloc(value.length());
        v.length = value.length();
        
        memcpy(k.s, key.c_str(), k.length);
        memcpy(v.s, value.c_str(), v.length);
        
        coap_add_attr(r, k.s, k.length, 
                         v.s, v.length, COAP_ATTR_FLAGS_RELEASE_NAME);                        
    }
    
    coap_add_resource(coap_ctx_, r);

    return r;
    
}

int CoAPWrapper::enable_proxy()
{
    coap_set_proxy_flag(coap_ctx_, 1);

    return 0;
}


void CoAPWrapper::default_handle_request(CoAPCallback &callback)
{
    make_bad_request(callback, 501);
}

void CoAPWrapper::get_domain_and_endpointIdentifier(CoAPCallback &callback,
                                                    std::string &domain,
                                                    std::string &ep)
{
    coap_pdu_t *pdu = (coap_pdu_t*)callback.request_;
    
    find_domain_ep_from_query(pdu, domain, ep);
}

void CoAPWrapper::bad_request(CoAPCallback &callback)
{
    make_bad_request(callback,400);    
}

void CoAPWrapper::changed_request(CoAPCallback &callback)
{
    make_bad_request(callback, 200);    

}

void CoAPWrapper::ok_request(CoAPCallback &callback)
{
    make_bad_request(callback, 201);    

}

void CoAPWrapper::internal_err_request(CoAPCallback &callback)
{
    make_bad_request(callback, 500);    

}

void CoAPWrapper::get_query_value(CoAPCallback &callback, 
                     std::string key,
                     std::string &value)
{
    coap_pdu_t *pdu = (coap_pdu_t*)(callback.request_);

    if ( pdu != 0)
        query_value(pdu, key, value);
}

void CoAPWrapper::get_query_value(CoAPCallback &callback, 
                     std::string key,
                     int &value)
{
    std::string tmp;
    coap_pdu_t *pdu = (coap_pdu_t*)(callback.request_);

    if ( pdu != 0)
        query_value(pdu, key, tmp);

    if( !tmp.empty())
    {
        value = ACE_OS::atoi(tmp.c_str());
    }
}

void CoAPWrapper::get_default_con(CoAPCallback &callback, std::string &con)
{
#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 40
#endif
    
    unsigned char addr[INET6_ADDRSTRLEN+8];
    coap_address_t *remote = (coap_address_t *)callback.peer_;

    memset(addr, 0, sizeof(addr));
    address_to_string(remote, addr, INET6_ADDRSTRLEN+8);

    con = std::string("coap://") + std::string((char*)addr);
}


void CoAPWrapper::get_payload(CoAPCallback &callback, std::string &payload)
{
    str load= {0, NULL};
    
    coap_pdu_t *pdu = (coap_pdu_t *) (callback.request_);
    
    if (coap_get_data(pdu, &load.length, &load.s)) 
    {
        payload.assign((char*)load.s, load.length);
    }
}


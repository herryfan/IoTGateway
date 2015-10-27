/*
 * This file defines coap forward proxy
 */

#ifndef __COAP_PROXY_H__
#define __COAP_PROXY_H__

#include "coap.h"

typedef struct coap_exchange_info_t
{
    coap_hdr_t *hdr;
    coap_address_t *source;
}coap_exchange_info_t; 

typedef struct coap_proxy_map_t
{  
    coap_key_t id_key;	/**< the actual key bytes for this resource */
    coap_key_t token_key;	/**< the actual key bytes for this resource */
    
#ifndef WITH_CONTIKI
#ifdef COAP_RESOURCES_NOHASH
    struct coap_proxy_map_t *next;
#else
    UT_hash_handle hh;
#endif

    struct coap_exchange_info_t *from_client;
    struct coap_exchange_info_t *to_server;

#endif /* WITH_CONTIKI */
 
} coap_proxy_map_t;

void handle_proxy(coap_context_t *context, coap_queue_t *sent, coap_queue_t *rcvd);


#endif /*__COAP_PROXY_H__*/




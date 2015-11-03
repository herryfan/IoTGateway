/*
* This file implement CoAP RD Resource
*/
#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"
#include "CoAP_RD_Resource.h"
#include "CoAP_RD_Node_Resource.h"
#include "coap.h"

CoAP_RD_Resource::CoAP_RD_Resource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{

}

CoAP_RD_Resource::~CoAP_RD_Resource()
{
}


void* CoAP_RD_Resource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    rd_resource_ = Create_i();
    SetCoAPResource(rd_resource_);

    return rd_resource_;
}

void CoAP_RD_Resource::response_ok(CoAPResource *node, CoAPCallback &callback)
{
#define LOCSIZE 68

    coap_pdu_t *response = (coap_pdu_t*)callback.response_;
    
    /* create response */
    response->hdr->code = COAP_RESPONSE_CODE(201);

    {
        /* split path into segments and add Location-Path options */
        unsigned char _b[LOCSIZE];
        unsigned char *b = _b;
        size_t buflen = sizeof(_b);
        int nseg;

        nseg = coap_split_path((unsigned char*)node->uri().c_str(), node->uri().length(), b, &buflen);
        while (nseg--)
        {
            coap_add_option(response, COAP_OPTION_LOCATION_PATH,
                            COAP_OPT_LENGTH(b), COAP_OPT_VALUE(b));
            b += COAP_OPT_SIZE(b);
        }
    }
    
}

void CoAP_RD_Resource::handler_post(CoAPCallback &callback)
{
    std::string ep_identifier = "";
    std::string ep_domain = "hisilicion";

    if (get_wrapper() != 0)
        get_wrapper()->get_domain_and_endpointIdentifier(callback, ep_domain, ep_identifier);

    if (ep_identifier.empty())
    {
        get_wrapper()->bad_request(callback);

        return;
    }

    CoAPResource *r = find_resource_from_cache_by_ep(ep_domain, ep_identifier);

    if (r)
    {
        r->update(callback);
        get_wrapper()->changed_request(callback);
    }
    else
    {
        r = create_node_resource(callback, ep_domain, ep_identifier);

        if (r)
        {
            void *rd;
            std::string payload;
            
            add_node_to_cache(r);
            
            r->update(callback);
            rd = r->Create();
            r->SetCoAPResource(rd);

            get_wrapper()->get_payload(callback, payload);
            r->set_payload(payload);

            response_ok(r, callback);
        }
        else
        {
            get_wrapper()->internal_err_request(callback);
        }
    }
        
}

std::string CoAP_RD_Resource::create_node_uri(std::string base)
{
#define LOCSIZE 68
    char loc[LOCSIZE];
    size_t loc_size=0;
    unsigned int now;

    ACE_OS::srand((unsigned)ACE_OS::time( NULL ));

    loc_size +=
        snprintf((char *)(loc + loc_size), LOCSIZE - loc_size - 1,
                 "%x", ACE_OS::rand());

    loc[loc_size] = '\0' ;            
                 
    return base + "/" + std::string(loc);
}

CoAPResource *CoAP_RD_Resource::create_node_resource(CoAPCallback &callback, 
                                                 std::string &domain, 
                                                 std::string &ep)
{
    CoAPRDNodeResource *node = new CoAPRDNodeResource(get_wrapper());

    if (node == 0)
        return 0;

    std::string tmp = create_node_uri(uri());
    
    node->SetURI(tmp);

    CoAP_Attr attr;

    attr.insert(std::make_pair("d",domain.c_str()));
    attr.insert(std::make_pair("ep",ep.c_str()));
    
    node->SetAttr(attr);
    
    return node;
}












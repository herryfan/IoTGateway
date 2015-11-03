/*
* This file implement CoAP Resource
*/

#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"
#include "coap.h"

//-----------------------------------------------------------------


static void make_coap_callback(coap_context_t  *ctx, struct coap_resource_t *resource,
           coap_address_t *peer, coap_pdu_t *request, str *token,
           coap_pdu_t *response,
           CoAPCallback &callback)
{
    callback.context_ = ctx;
    callback.resource_ = resource;
    callback.peer_ = peer;
    callback.request_ = request;
    callback.token = token;
    callback.response_ = response;
}

static void
method_handler_get(coap_context_t  *ctx, struct coap_resource_t *resource,
           coap_address_t *peer, coap_pdu_t *request, str *token,
           coap_pdu_t *response)
 
{   
    if (resource != 0 && resource->app_contex != 0)
    {    
       CoAPResource *resource_self = (CoAPResource*)(resource->app_contex);

       CoAPCallback tmp_callback;
       make_coap_callback(ctx, resource, peer, request, token, response, tmp_callback);

       resource_self->handler_get(tmp_callback);
    }
}

static void
method_handler_put(coap_context_t  *ctx, struct coap_resource_t *resource,
           coap_address_t *peer, coap_pdu_t *request, str *token,
           coap_pdu_t *response)
 
{
    if (resource != 0 && resource->app_contex != 0)
    {
        CoAPResource *resource_self = (CoAPResource*)(resource->app_contex);
        
        CoAPCallback tmp_callback;
        make_coap_callback(ctx, resource, peer, request, token, response, tmp_callback);
        
        resource_self->handler_put(tmp_callback);
    }
}

static void
method_handler_post(coap_context_t  *ctx, struct coap_resource_t *resource,
           coap_address_t *peer, coap_pdu_t *request, str *token,
           coap_pdu_t *response)
 
{
    if (resource != 0 && resource->app_contex != 0)
    {
        CoAPResource *resource_self = (CoAPResource*)(resource->app_contex);
        
        CoAPCallback tmp_callback;
        make_coap_callback(ctx, resource, peer, request, token, response, tmp_callback);
        
        resource_self->handler_post(tmp_callback);
    }

}

static void
method_handler_delete(coap_context_t  *ctx, struct coap_resource_t *resource,
           coap_address_t *peer, coap_pdu_t *request, str *token,
           coap_pdu_t *response)
 
{
    if (resource != 0 && resource->app_contex != 0)
    {
        CoAPResource *resource_self = (CoAPResource*)(resource->app_contex);
        
        CoAPCallback tmp_callback;
        make_coap_callback(ctx, resource, peer, request, token, response, tmp_callback);
        
        resource_self->handler_delete(tmp_callback);
    }
}

static CoAPResource::method_handler_t method_handler_resource = 
{
(void*)method_handler_get,
(void*)method_handler_post,
(void*)method_handler_put,
(void*)method_handler_delete
};

//-----------------------------------------------------------------

CoAPResource::coap_resource_cache_t CoAPResource::ep_cache_;

CoAPResource::CoAPResource(CoAPWrapper* ctx)
:coap_ctx_(ctx)
{
    ACE_ASSERT(coap_ctx_ != 0);
}

CoAPResource::~CoAPResource()
{
}

void* CoAPResource::Create()
{
    return Create_i();
}

CoAPWrapper *CoAPResource::get_wrapper()
{
    return coap_ctx_;
}

std::string CoAPResource::uri()
{
    return uri_;
}

void CoAPResource::SetCoAPResource(void *coap_resource)
{
    coap_resource_ = coap_resource;
}

void CoAPResource::SetURI(std::string& uri)
{
    uri_ = uri;
}

void CoAPResource::set_payload(std::string &payload)
{
    ACE_DEBUG((LM_DEBUG, "payload = %s\n", payload.c_str()));
    payload_ = payload;
}


void CoAPResource::SetAttr(CoAP_Attr& attr)
{
    attr_ = attr;
}

void* CoAPResource::Create_i()
{
    return coap_ctx_->CreateResource(uri_,
                                     attr_,
                                     method_handler_resource,
                                     this);
    
}

void CoAPResource::handler_get(CoAPCallback &callback)
{
    if(coap_ctx_)
        coap_ctx_->default_handle_request(callback);
}

void CoAPResource::handler_post(CoAPCallback &callback)
{
    if(coap_ctx_)
        coap_ctx_->default_handle_request(callback);
}

void CoAPResource::handler_put(CoAPCallback &callback)
{
    if(coap_ctx_)
        coap_ctx_->default_handle_request(callback);
}

void CoAPResource::handler_delete(CoAPCallback &callback)
{
    if(coap_ctx_)
        coap_ctx_->default_handle_request(callback);
}

void CoAPResource::add_node_to_cache(CoAPResource *r)
{
    ep_cache_.push_back(r);
}

CoAPResource *CoAPResource::find_resource_from_cache_by_ep(std::string &domain, std::string &ep)
{
    CoAPResource::coap_resource_cache_t::iterator e = ep_cache_.begin();

    for (; e != ep_cache_.end(); ++e)
    {
        if ((*e)->compare(domain, ep))
        {
            return (*e);
        }
    }
    
    return 0;
}

bool CoAPResource::compare(std::string &domain, std::string &ep)
{
    std::string ep_self((*attr_.find("ep")).second);
    std::string domain_self((*attr_.find("d")).second);

    if ((ep_self.compare(ep) == 0) && 
        (domain_self.compare(domain) == 0))
    {
        return true;
    }

    return false;
}

void CoAPResource::update_lifetime(int lt)
{
    char buf[20];
    
    if (lt < 60 )
        lt = 60;

    sprintf(buf, "%d", lt);
    attr_["lt"] = buf;
}

void CoAPResource::update_et(std::string &et)
{
    if (et.empty())
        return;

    attr_["et"] = et;
}

void CoAPResource::update_context(std::string &con)
{
    if (con.empty())
        return;

    attr_["con"] = con;
}

void CoAPResource::update(CoAPCallback &callback)
{
    std::string context = "";
    std::string et = "";
    int lifetime = 0;
    
    if (get_wrapper())
    {
        get_wrapper()->get_query_value(callback, "lt", lifetime);
        get_wrapper()->get_query_value(callback, "et", et);
        get_wrapper()->get_query_value(callback, "con", context);   
    }

    if (context.empty())
        get_wrapper()->get_default_con(callback, context);
        
    update_lifetime(lifetime);
    update_context(context);
    update_et(et);
    
}



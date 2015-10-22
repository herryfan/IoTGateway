/*
* This file implement CoAP Resource
*/

#include "CoAP_Wrapper.h"
#include "CoAP_Resource.h"


CoAPResource::CoAPResource(CoAPWrapper* ctx)
:coap_ctx_(ctx)
{
    ACE_ASSERT(coap_ctx_ != 0);
}

CoAPResource::~CoAPResource()
{
}


void CoAPResource::SetURI(std::string& uri)
{
    uri_ = uri;
}

void CoAPResource::SetAttr(CoAP_Attr& attr)
{
    attr_ = attr;
}

void CoAPResource::SetMethodHandler(method_handler_t& method_handler)
{
    method_handler_ = method_handler;
}

int CoAPResource::Create_i()
{
    return coap_ctx_->CreateResource(uri_,
                                     attr_,
                                     method_handler_);
    
}



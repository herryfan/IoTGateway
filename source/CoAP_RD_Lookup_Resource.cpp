/*
* This file implement lookup resource
*/

#include "Config.h"
#include "CoAP_RD_Lookup_Resource.h"
#include "CoAP_RD_Lookup_D_Resource.h"
#include "CoAP_RD_Lookup_EP_Resource.h"
#include "CoAP_RD_Lookup_RES_Resource.h"
#include "CoAP_Wrapper.h"


CoAPRDLookUpResource::CoAPRDLookUpResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
    domain_resource_ = new CoAPRDLookUpDomainResource(ctx);
    ep_resource_ = new CoAPRDLookUpEPResource(ctx);
    res_resource_ = new CoAPRDLookUpResResource(ctx);
}

CoAPRDLookUpResource::~CoAPRDLookUpResource()
{
    delete domain_resource_;
    delete ep_resource_;
    delete res_resource_;
    
}

void* CoAPRDLookUpResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd-lookup");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd-lookup\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    domain_resource_->Create();
    ep_resource_->Create();
    res_resource_->Create();
    
    return rd_resource;
}




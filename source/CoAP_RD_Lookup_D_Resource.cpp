/*
* This file implement lookup D resource
*/

#include "Config.h"
#include "CoAP_RD_Lookup_D_Resource.h"
#include "CoAP_Wrapper.h"

CoAPRDLookUpDomainResource::CoAPRDLookUpDomainResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
}

CoAPRDLookUpDomainResource::~CoAPRDLookUpDomainResource()
{

}

void* CoAPRDLookUpDomainResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd-lookup/d");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd-lookup.d\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}



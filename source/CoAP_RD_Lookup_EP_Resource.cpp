/*
* This file implement lookup EP resource
*/

#include "Config.h"
#include "CoAP_RD_Lookup_EP_Resource.h"
#include "CoAP_Wrapper.h"

CoAPRDLookUpEPResource::CoAPRDLookUpEPResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
}

CoAPRDLookUpEPResource::~CoAPRDLookUpEPResource()
{

}

void* CoAPRDLookUpEPResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd-lookup/ep");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd-lookup.ep\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}


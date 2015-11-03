/*
* This file implement lookup RES resource
*/

#include "Config.h"
#include "CoAP_RD_Lookup_RES_Resource.h"
#include "CoAP_Wrapper.h"

CoAPRDLookUpResResource::CoAPRDLookUpResResource(CoAPWrapper* ctx)
:CoAPResource(ctx)
{
}

CoAPRDLookUpResResource::~CoAPRDLookUpResResource()
{

}

void* CoAPRDLookUpResResource::Create()
{
    CoAP_Attr attr;
    std::string uri("rd-lookup/res");    
    
    SetURI(uri);
    attr.insert(std::make_pair("ct","40"));
    attr.insert(std::make_pair("rt","\"core.rd-lookup.res\""));
    attr.insert(std::make_pair("ins","\"default\""));

    SetAttr(attr);

    void* rd_resource = Create_i();
    SetCoAPResource(rd_resource);

    return rd_resource;
}



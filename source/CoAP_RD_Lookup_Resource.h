/*
* This file defines lookup resource
*/

#ifndef __COAP_RD_LOOKUP_RESOURCE_H__
#define __COAP_RD_LOOKUP_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPWrapper;
class CoAPRDLookUpDomainResource;
class CoAPRDLookUpEPResource;
class CoAPRDLookUpResResource;

class CoAPRDLookUpResource: public CoAPResource
{
public:

    CoAPRDLookUpResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpResource();

    void* Create();

private:

    CoAPRDLookUpDomainResource *domain_resource_;
    CoAPRDLookUpEPResource *ep_resource_;
    CoAPRDLookUpResResource *res_resource_;
    
    
    
};












#endif //__COAP_RD_LOOKUP_RESOURCE_H__






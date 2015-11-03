/*
* This file defines lookup Domain resource
*/

#ifndef __COAP_RD_LOOKUP_D_RESOURCE_H__
#define __COAP_RD_LOOKUP_D_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPWrapper;

class CoAPRDLookUpDomainResource: public CoAPResource
{
public:

    CoAPRDLookUpDomainResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpDomainResource();

    void* Create();

private:
    
    
};



#endif //__COAP_RD_LOOKUP_D_RESOURCE_H__




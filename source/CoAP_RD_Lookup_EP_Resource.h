/*
* This file defines lookup  EP resource
*/

#ifndef __COAP_RD_LOOKUP_EP_RESOURCE_H__
#define __COAP_RD_LOOKUP_EP_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPWrapper;

class CoAPRDLookUpEPResource: public CoAPResource
{
public:

    CoAPRDLookUpEPResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpEPResource();

    void* Create();

private:
    
    
};



#endif //__COAP_RD_LOOKUP_EP_RESOURCE_H__





/*
* This file defines lookup RES resource
*/

#ifndef __COAP_RD_LOOKUP_RES_RESOURCE_H__
#define __COAP_RD_LOOKUP_RES_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPWrapper;

class CoAPRDLookUpResResource: public CoAPResource
{
public:

    CoAPRDLookUpResResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpResResource();

    void* Create();

private:
    
    
};



#endif //__COAP_RD_LOOKUP_RES_RESOURCE_H__




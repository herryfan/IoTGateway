/*
*  This file defines RD Resource
*/

#ifndef __COAP_RD_RESOURCE_H__
#define __COAP_RD_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPWrapper;

class CoAP_RD_Resource : public CoAPResource
{
public:
    
    CoAP_RD_Resource(CoAPWrapper* ctx);
    ~CoAP_RD_Resource();

    int Create();

private:
    
};


#endif //__COAP_RD_RESOURCE_H__


/*
* This file defines RD Node Resource
*/

#ifndef __COAP_RD_NODE_RESOURCE_H__
#define __COAP_RD_NODE_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPRDNodeResource : public CoAPResource
{
public:

    CoAPRDNodeResource(CoAPWrapper* ctx);
    virtual ~CoAPRDNodeResource();


private:

};



#endif //__COAP_RD_NODE_RESOURCE_H__




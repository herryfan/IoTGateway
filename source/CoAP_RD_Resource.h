/*
*  This file defines RD Resource
*/

#ifndef __COAP_RD_RESOURCE_H__
#define __COAP_RD_RESOURCE_H__

#include "CoAP_Resource.h"

class CoAPWrapper;
class CoAPCallback;

class CoAP_RD_Resource : public CoAPResource
{
public:
    
    CoAP_RD_Resource(CoAPWrapper* ctx);
    virtual ~CoAP_RD_Resource();

    void* Create();
    
    virtual void handler_post(CoAPCallback &callback); 

protected:

    CoAPResource *create_node_resource(CoAPCallback &callback, std::string &domain, std::string &ep);
    std::string create_node_uri(std::string base);
    void response_ok(CoAPResource *node, CoAPCallback &callback);

private:

    void *rd_resource_;
    
};


#endif //__COAP_RD_RESOURCE_H__


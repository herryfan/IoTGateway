/*
* This file defins CoAP Resource
*/

#ifndef __COAP_RESOURCE_H__
#define __COAP_RESOURCE_H__

#include <string>
#include <map>

class CoAPWrapper;

typedef std::map<std::string, std::string> CoAP_Attr;

class CoAPResource
{
public:

   typedef struct method_handler_t
   {
    void* handler_get;
    void* handler_post;
    void* handler_put;
    void* handler_delete;
   }method_handler_t;

    CoAPResource(CoAPWrapper* ctx);
    virtual ~CoAPResource();

    virtual int Create() = 0;

    void SetURI(std::string& uri);
    void SetAttr(CoAP_Attr& attr);
    void SetMethodHandler(method_handler_t& method_handler);

protected:

    int Create_i();
    
private:

    std::string uri_;
    CoAP_Attr attr_;
    
    CoAPWrapper* coap_ctx_;

    method_handler_t method_handler_;
    
};



#endif //__COAP_RESOURCE_H__




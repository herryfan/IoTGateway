/*
* This file defins CoAP Resource
*/

#ifndef __COAP_RESOURCE_H__
#define __COAP_RESOURCE_H__

#include <string>
#include <map>
#include <list>


class CoAPWrapper;
class CoAPCallback;

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
   
    typedef std::list<CoAPResource*> coap_resource_cache_t;

    CoAPResource(CoAPWrapper* ctx);
    virtual ~CoAPResource();

    virtual void* Create();

    void SetURI(std::string& uri);
    void set_payload(std::string &payload);
    void SetAttr(CoAP_Attr& attr);
    void SetCoAPResource(void *coap_resource);
    std::string uri();
    bool compare(std::string &domain, std::string &ep);
    void update(CoAPCallback &callback);
    void update_lifetime(int lt);
    void update_et(std::string &et);
    void update_context(std::string &con);
    
    virtual void handler_get(CoAPCallback &callback);
    virtual void handler_post(CoAPCallback &callback);
    virtual void handler_put(CoAPCallback &callback);
    virtual void handler_delete(CoAPCallback &callback);
    
protected:

    void* Create_i();
    CoAPWrapper *get_wrapper();
    CoAPResource *find_resource_from_cache_by_ep(std::string &domain, std::string &ep);
    void add_node_to_cache(CoAPResource *r);
    
private:

    std::string uri_;
    std::string payload_;
    CoAP_Attr attr_;
    
    CoAPWrapper* coap_ctx_;
    void *coap_resource_;

    static coap_resource_cache_t ep_cache_;
};



#endif //__COAP_RESOURCE_H__




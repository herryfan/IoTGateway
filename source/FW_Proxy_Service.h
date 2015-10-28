/*
 * This file defines Forward Proxy service for coap
 */

#ifndef __FW_PROXY_SERVICE_H__
#define __FW_PROXY_SERVICE_H__

#include "Config.h"


class CoAPWrapper;
class CfgService;
class NetService;

class FWProxyService : public ACE_Event_Handler
{
public:

    FWProxyService(CfgService *conf, NetService *net);
    ~FWProxyService();

    int Init();
    int Close();

    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

    virtual ACE_HANDLE get_handle (void) const;

private:
    
    CfgService *conf_;
    NetService *net_;
    CoAPWrapper *coap_wrapper_;
};





#endif //__FW_PROXY_SERVICE_H__




/*
* This file defines Resource directory service
*/

#ifndef __RD_SERVICE_H__
#define __RD_SERVICE_H__

#include "Config.h"

class CoAPWrapper;
class CoAP_RD_Resource;
class CfgService;
class NetService;
class CoAPRDLookUpResource;

class RDService : public ACE_Event_Handler
{
public:

    RDService(CfgService *conf, NetService *net);
    ~RDService();

    int Init();
    int Close();

    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
    
    virtual ACE_HANDLE get_handle (void) const;

private:
    
    CoAP_RD_Resource *rd_resource_;
    CoAPRDLookUpResource *lookup_resource_;
    CfgService *conf_;
    NetService *net_;
    CoAPWrapper *coap_wrapper_;
    
};



#endif //__RD_SERVICE_H__




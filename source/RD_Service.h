/*
* This file defines Resource directory service
*/

#ifndef __RD_SERVICE_H__
#define __RD_SERVICE_H__

#include "Config.h"

class CoAPWrapper;
class CoAP_RD_Resource;

class RDService
{
public:

    RDService(CoAPWrapper* ctx);
    ~RDService();

    int Init();
    int Close();

private:
    CoAP_RD_Resource* rd_resource_;
    
};



#endif //__RD_SERVICE_H__




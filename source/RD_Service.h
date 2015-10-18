/*
* This file defines Resource directory service
*/

#ifndef __RD_SERVICE_H__
#define __RD_SERVICE_H__

#include "Config.h"
#include "coap.h"


class RDService
{
public:

    RDService(struct coap_context_t* ctx);
    ~RDService();

    int Init();
    int Close();

private:

    struct coap_context_t* coap_ctx_;
    
};



#endif //__RD_SERVICE_H__




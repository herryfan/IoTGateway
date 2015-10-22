/*
*  This file implement Resource directory service
*/

#include "RD_Service.h"
#include "CoAP_Wrapper.h"
#include "CoAP_RD_Resource.h"



RDService::RDService(CoAPWrapper* ctx)
:rd_resource_(new CoAP_RD_Resource(ctx))
{
    ACE_ASSERT(rd_resource_ != 0);
}

RDService::~RDService()
{   
}

int RDService::Init()
{  
    if (rd_resource_)
    {
        return rd_resource_->Create();
    }
    
    return 0;
}

int RDService::Close()
{
    if (rd_resource_ )
        delete rd_resource_;
        
    return 0;
}


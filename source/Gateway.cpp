/*
*  This file implement Gateway
*/

#include "Config.h"
#include "Gateway.h"
#include "CfgService.h"
#include "NET_Service.h"
#include "RD_Service.h"
#include "FW_Proxy_Service.h"

int Gateway::Init()
{
    svc_conf_ = new CfgService();
    svc_net_ = new NetService();

    if ( svc_conf_ == 0 || 
         svc_net_ == 0 )
    {
        return -1;
    }

    
    if (svc_conf_->Init("gateway.conf") < 0)
    {
        return -1;
    }

    svc_net_->SetConf(svc_conf_);
    
    if (svc_net_->Init() < 0)
    {
        return -1;
    }


    if ((svc_rd_ = new RDService(svc_conf_, svc_net_)) == 0)
    {
        return -1;
    }

    if (svc_rd_->Init() < 0)
    {
        return -1;
    }

    if((svc_proxy_= new FWProxyService(svc_conf_, svc_net_)) == 0)
    {
        return -1;
    }

    if (svc_proxy_->Init() < 0)
    {
        return -1;
    }

    return 0;

}

int Gateway::Start()
{
    if (svc_net_->Start() == -1)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to start net service \n"));
        return -1;
    }

    return 0;
}

int Gateway::Stop()
{
    // FIXME-be careful, the net serice must first to be closed.
    svc_rd_->Close();
    svc_net_->Close();
    svc_conf_->Close();
    svc_proxy_->Close();

    Close();
    
    return 0;
}

int Gateway::Close()
{
    if (svc_proxy_)
    {
        delete svc_proxy_;
        svc_proxy_ = 0;
    }
    
    if (svc_rd_ )
    {
        delete svc_rd_;
        svc_rd_ = 0;
    }

    if (svc_net_ )
    {
        delete svc_net_;
        svc_net_ = 0;
    }

    if (svc_conf_ )
    {
        delete svc_conf_;
        svc_conf_ = 0;
    }
    
    return 0;
}







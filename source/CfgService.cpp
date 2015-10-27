/*
*  This file implement Cfg service
*/

#include "Config.h"
#include "CfgService.h"

CfgService::CfgService()
:conf_(0)
{
    Default();
}

void CfgService::Default()
{
    mcast_addr_ = "ff05::fd";
    mcast_addr_port_ = 5683;
    svc_addr_ = "::";
    svc_addr_port_ = 5683;
    proxy_addr_ = "::";
    proxy_addr_port_ = 5683;
    rd_addr_ = "::";
    rd_addr_port_ = 5683;

    enable_proxy_ = true;
    enable_rd_ = true;

    cfg_file_ = "gateway.conf";
    coap_debug_level_ = 3;
}

CfgService::~CfgService()
{
    Close();
}


int CfgService::GetValue(const char* item, ACE_TString &value)
{
    ACE_Configuration_Section_Key key;
    std::string gateway_section("gateway");

    int code = conf_->open_section(conf_->root_section(), 
                     gateway_section.c_str(), 
                     0,
                     key);

     
    if (code < 0 )
    {
     ACE_DEBUG((LM_DEBUG, 
     "Failed to get section %s\n",gateway_section.c_str()));

     return -1;
    }
    
    return conf_->get_string_value(key, item, value);
    
}

int CfgService::GetValue(const char* item, int &value)
{
    ACE_TString value_s;

    if (GetValue(item, value_s) >= 0 )
    {
        value = ACE_OS::atoi(value_s.c_str());
    }

    return 0;
}

int CfgService::GetValue(const char* item, bool &value)
{
    ACE_TString value_s;

    if (GetValue(item, value_s) >= 0 )
    {
        value = (::strcasecmp(
                value_s.c_str(), "true")
                ) == 0 ? true: false;
    }

    return 0;

}

int CfgService::Init(const char* cfile_name)
{
    conf_ = new ACE_Configuration_Heap();

    ACE_ASSERT(conf_ != NULL);
    
    ACE_Ini_ImpExp ini(*conf_);
    cfg_file_ = cfile_name;

    conf_->open();

    if ( ini.import_config(cfg_file_.c_str()) == -1)
    {
        ACE_DEBUG((LM_DEBUG, 
        "Failed to open config file %s. please check...\n",
        cfg_file_.c_str()));
        
        return -1;
    }

    GetValue("multicast_addr", mcast_addr_);
    GetValue("multicast_addr_port", mcast_addr_port_);
    GetValue("server_addr", svc_addr_);
    GetValue("server_addr_port", svc_addr_port_);
    GetValue("proxy_addr", proxy_addr_);
    GetValue("proxy_addr_port", proxy_addr_port_);
    GetValue("rd_addr", rd_addr_);
    GetValue("rd_addr_port", rd_addr_port_);
    
    GetValue("enable_proxy", enable_proxy_);
    GetValue("enable_rd", enable_rd_);
    GetValue("coap_debug", coap_debug_level_);
    
    return 0;
}

int CfgService::Close()
{
    if (conf_)
    {
        delete conf_;
        conf_ = 0;
    }

    return 0;
}












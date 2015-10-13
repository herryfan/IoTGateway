/*
*  This file implement Cfg service
*/

#include "Config.h"
#include "CfgService.h"

CfgService::CfgService()
:ipv6_flag_(0)
{
}

CfgService::~CfgService()
{
}

int CfgService::Init(const char* cfile_name)
{
    ACE_Configuration_Heap cfg;
    ACE_Ini_ImpExp ini(cfg);
    cfg_file_ = cfile_name;

    cfg.open();

    if ( ini.import_config(cfg_file_.c_str()) == -1)
    {
        ACE_DEBUG((LM_DEBUG, 
        "Failed to open config file %s. please check...\n",
        cfg_file_.c_str()));

        return -1;
    }

    ACE_Configuration_Section_Key key;
    std::string gateway_section("gateway");

    int code = cfg.open_section(cfg.root_section(), 
                     gateway_section.c_str(), 
                     0,
                     key);
                     
    if (code < 0 )
    {
        ACE_DEBUG((LM_DEBUG, 
        "Failed to get section %s\n",gateway_section.c_str()));

        return -1;
    }

    ACE_TString is_ipv6("is_ipv6");
    ACE_TString value;

    code = cfg.get_string_value(key, is_ipv6.c_str(), value);

    if (code < 0 )
    {
        ACE_DEBUG((LM_DEBUG, 
        "Failed to get %s\n",is_ipv6.c_str()));

        return -1;
    }

    ipv6_flag_ = atoi(value.c_str());


    ACE_TString server_uri("server_uri");

    code = cfg.get_string_value(key, server_uri.c_str(), svc_uri_);

    if (code < 0)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to get server_uri\n"));

        return -1;
    }
    
    multicast_ipv4_.set(5683,"224.0.1.187");
    multicast_ipv6_.set(5683,"ff05::fd", AF_INET6);
    
    //
    // Debug information
    //
    ACE_DEBUG((LM_DEBUG,
               "dump cfg information\n"));
               
    ACE_DEBUG((LM_DEBUG,"ipv6_flag_ = %d\n", ipv6_flag_));
    ACE_DEBUG((LM_DEBUG,"server_uri = %s\n", svc_uri_.c_str()));
    
    ACE_TCHAR s[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16];
    multicast_ipv4_.addr_to_string(s, ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s\n"), s));

    multicast_ipv6_.addr_to_string(s, ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s\n"), s));
    
    return 0;
}

int CfgService::Close()
{
    return 0;
}

bool CfgService::IsIpv6()
{
    return ipv6_flag_ > 0 ? true : false;
}

const char* CfgService::GetServerUri()
{
    return svc_uri_.c_str();
}

ACE_INET_Addr& CfgService::GetMcastAddrV4()
{
    return multicast_ipv4_;
}

ACE_INET_Addr& CfgService::GetMcastAddrV6()
{
    return multicast_ipv6_;
}

















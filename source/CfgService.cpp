/*
*  This file implement Cfg service
*/

#include "Config.h"
#include "CfgService.h"

CfgService::CfgService()
:coap_debug_level_(3)
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

    /* Get coap debug level*/
    {
        std::string debug_level("coap_debug");
        std::string debug_lelvel_value;

        code = cfg.get_string_value(key, debug_level.c_str(), debug_lelvel_value);

        if (code < 0 )
        {
            ACE_DEBUG((LM_DEBUG,
                "Not set coap debug level\n"));
        }
        else
        {
            coap_debug_level_ = atoi(debug_lelvel_value.c_str());
        }
    }

    /* Get server_ip and port string*/
    ACE_TString server_ip("server_ip");
    ACE_TString server_port("server_port");
    ACE_TString svr_ip;
    ACE_TString svr_port;
    
    code = cfg.get_string_value(key, server_ip.c_str(), svr_ip);
    code |= cfg.get_string_value(key, server_port.c_str(), svr_port);

    if (code < 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                   "Failed to get server ip and port\n"));

        return -1;
    }

    code = svc_addr_.set(atoi(svr_port.c_str()),svr_ip.c_str());

    if ( code < 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to set server ip and port,"
                    "maybe the port and ip not to be correctly config\n"));
    }

    /* Get multicast ip and port */
    ACE_TString mcast_ip("multicast_ip");
    ACE_TString mcast_port("multicast_port");
    ACE_TString multicast_ip;
    ACE_TString multicast_port;

    code = cfg.get_string_value(key, mcast_ip.c_str(), multicast_ip);
    code |= cfg.get_string_value(key, mcast_port.c_str(), multicast_port);

    if (code < 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                   "Failed to get multicast ip and port\n"));

        return -1;
    }

    code = multicast_addr_.set(atoi(multicast_port.c_str()),multicast_ip.c_str());

    if ( code < 0 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to set multicast ip and port,"
                    "maybe the port and ip not to be correctly config\n"));
    }
    
    //
    // Debug information
    //
    ACE_DEBUG((LM_DEBUG,
               "dump cfg information\n"));
               
    ACE_TCHAR s[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16];
    multicast_addr_.addr_to_string(s, ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s\n"), s));

    svc_addr_.addr_to_string(s, ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s\n"), s));
    
    return 0;
}

int CfgService::Close()
{
    return 0;
}

ACE_INET_Addr& CfgService::GetMcastAddr()
{
    return multicast_addr_;
}

ACE_INET_Addr& CfgService::GetServerAddr()
{
    return svc_addr_;
}

int CfgService::GetCoapDebugLevel()
{
    return coap_debug_level_;
}
















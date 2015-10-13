/*
* This file defines Cfg feature. HOW to manage gateway configration items
*/

#ifndef __CFG_SERVICE_H__
#define __CFG_SERVICE_H__

#include "Config.h"

class CfgService
{
public:

    CfgService();
    ~CfgService();

    int Init(const char* cfile_name);
    int Close();
    bool IsIpv6();
    ACE_INET_Addr& GetMcastAddrV4();
    ACE_INET_Addr& GetMcastAddrV6();
    const char* GetServerUri();
    
private:

    u_int ipv6_flag_;
    ACE_INET_Addr multicast_ipv4_;
    ACE_INET_Addr multicast_ipv6_;
    ACE_TString cfg_file_;
    ACE_TString svc_uri_;
    ;
};

typedef ACE_Singleton<CfgService, ACE_Mutex> Cfg_Service;


#endif // __CFG_SERVICE_H__


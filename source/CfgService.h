/*
* This file defines Cfg feature. HOW to manage gateway configration items
*/

#ifndef __CFG_SERVICE_H__
#define __CFG_SERVICE_H__

#include "Config.h"

class CfgService
{
public:

    CfgService(const char* cfgfile);
    ~CfgService();

    int Init();
    int Close();
    bool IsIpv6();
    ACE_INET_Addr& GetMcastAddrV4();
    ACE_INET_Addr& GetMcastAddrV6();
    
private:

    u_int ipv6_flag_;
    ACE_INET_Addr multicast_ipv4_;
    ACE_INET_Addr multicast_ipv6_;
    ACE_TString cfg_file;
};


#endif // __CFG_SERVICE_H__


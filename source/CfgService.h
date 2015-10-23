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
    ACE_INET_Addr& GetMcastAddr();
    ACE_INET_Addr& GetServerAddr();
    int GetCoapDebugLevel();
    
private:

    ACE_INET_Addr multicast_addr_;
    ACE_INET_Addr svc_addr_;
    
    ACE_TString cfg_file_;
    ACE_TString svc_uri_;
    int coap_debug_level_;
};

typedef ACE_Singleton<CfgService, ACE_Mutex> Cfg_Service;


#endif // __CFG_SERVICE_H__


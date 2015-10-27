/*
* This file defines Cfg feature. HOW to manage gateway configration items
*/

#ifndef __CFG_SERVICE_H__
#define __CFG_SERVICE_H__

#include "Config.h"

class ACE_Ini_ImpExp;

class CfgService
{
public:

    CfgService();
    ~CfgService();

    int Init(const char* cfile_name);
    int Close();
    
    ACE_TString mcast_addr_;
    int mcast_addr_port_;
    ACE_TString svc_addr_;
    int svc_addr_port_;
    ACE_TString proxy_addr_;
    int proxy_addr_port_;
    ACE_TString rd_addr_;
    int rd_addr_port_;

    bool enable_proxy_;
    bool enable_rd_;    
    int coap_debug_level_;

private:

    void Default();
    int GetValue(const char* item, ACE_TString &value);
    int GetValue(const char* item, int &value);
    int GetValue(const char* item, bool &value);

    ACE_TString cfg_file_;
    
    ACE_Configuration_Heap *conf_;
};

typedef ACE_Singleton<CfgService, ACE_Mutex> Cfg_Service;


#endif // __CFG_SERVICE_H__


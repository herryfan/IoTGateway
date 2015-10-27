/*
* This file defines Gateway feature
*/

#ifndef __GATE_WAY_H__
#define __GATE_WAY_H__

#include "Config.h"


class CfgService;
class NetService;
class RDService;

class Gateway
{
public:
    
      int Init();
      int Start();
      int Stop();
      int Close();
      
private:

    CfgService *svc_conf_;
    NetService *svc_net_;
    RDService  *svc_rd_;
    
};

// Use ACE Singleton wrapper Gateway object.
typedef ACE_Singleton<Gateway, ACE_Mutex> IoTGateway;

#endif /* __GATE_WAY_H__*/







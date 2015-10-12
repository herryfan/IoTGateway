/*
* This file defines Gateway feature
*/

#ifndef __GATE_WAY_H__
#define __GATE_WAY_H__

#include "Config.h"

class CfgService;
class NetService;

class Gateway
{
public:
    
      int Initialize();
      int Start();
      int Stop();
      int Run();

private:

    CfgService* cfgsvc_;
    NetService* netsvc_;
};

// Use ACE Singleton wrapper Gateway object.
typedef ACE_Singleton<Gateway, ACE_Null_Mutex> IoTGateway;

#endif /* __GATE_WAY_H__*/







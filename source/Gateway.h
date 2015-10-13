/*
* This file defines Gateway feature
*/

#ifndef __GATE_WAY_H__
#define __GATE_WAY_H__

#include "Config.h"

class Gateway
{
public:
    
      int Initialize();
      int Start();
      int Stop();
      int Run();

private:

};

// Use ACE Singleton wrapper Gateway object.
typedef ACE_Singleton<Gateway, ACE_Mutex> IoTGateway;

#endif /* __GATE_WAY_H__*/







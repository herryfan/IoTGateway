/*
* This file defines Net service feature for Gateway
*/

#ifndef __NET_SERVICE_H__
#define __NET_SERVICE_H__

class ACE_Reactor;

class NetService
{
public:
    
    NetService();
    ~NetService();

    int Init();
    int Close();
    int Run();
    
private:

    ACE_Reactor* reactor_;

    
    
};



#endif /*__NET_SERVICE_H__ */







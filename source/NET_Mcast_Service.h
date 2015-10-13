/*
* This file defines multicast service
*/

#ifndef __NET_MCAST_SERVICE_H__
#define __NET_MCAST_SERVICE_H__


#include "Config.h"

class NetMcastService : public ACE_Event_Handler
{
public:

    NetMcastService();
    ~NetMcastService();

    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);    
    virtual ACE_HANDLE get_handle (void) const;
    
    int Start();
    
private:
    
    ACE_SOCK_Dgram_Mcast mcast_;
};



#endif //__NET_MCAST_SERVICE_H__





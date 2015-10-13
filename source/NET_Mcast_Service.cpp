/*
* This file implement Multicast service
*/

#include "Config.h"
#include "CfgService.h"
#include "NET_Mcast_Service.h"


NetMcastService::NetMcastService()
{
}

NetMcastService::~NetMcastService()
{
}

int NetMcastService::handle_input (ACE_HANDLE fd)
{
    char buf[0xff];
    int buf_len = 0xff;
    ACE_INET_Addr remote_addr;
        
    
    ssize_t retcode = mcast_.recv (buf, buf_len, remote_addr);

    // error occur
    if (retcode == -1)
    {
        return -1;
    }

    // Check the message is reqeuest service port or not
    if ( true)
    {
        const char* server_uri = Cfg_Service::instance()->GetServerUri();

        strncpy(buf, server_uri, 0xff );

        retcode = mcast_.ACE_SOCK_Dgram::send(buf, 0xff, remote_addr);

        if(retcode == -1 )
        {
            ACE_DEBUG((LM_DEBUG,
                        "Failed to send respons to client[%s]\n",buf));
        }
    }
    
    return 0;
}

int NetMcastService::handle_timeout (const ACE_Time_Value &tv,
                            const void *arg)
{
    return -1;
}


ACE_HANDLE NetMcastService::get_handle (void) const
{
    printf("get_handle: %d \n", mcast_.get_handle());
    
    return mcast_.get_handle();
}

int NetMcastService::Start()
{
    ACE_INET_Addr mcast_addr;

    if ( Cfg_Service::instance()->IsIpv6())
    {
        mcast_addr.set(Cfg_Service::instance()->GetMcastAddrV6());
    }
    else
    {
        mcast_addr.set(Cfg_Service::instance()->GetMcastAddrV4());
    }

    
    int code = mcast_.join(mcast_addr);

    if (code == -1)
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to join into multicast group\n"));

        return -1;
    }

    return 0;
}













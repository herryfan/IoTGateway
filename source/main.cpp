/*
* This file defines main method for Gateway
*/

#include "Config.h"
#include "Gateway.h"

/*
* Gateway uses the ace log system to record it's log
* and debug
*/
static int Initialize_ACE_Log()
{
    return 0;
}

int main(int argc, char** argv)
{
    Initialize_ACE_Log();
    
    if ( IoTGateway::instance()->Initialize() >= 0)
    {
        int code = IoTGateway::instance()->Start();

        if ( code >= 0 )
        {
            IoTGateway::instance()->Run();
            IoTGateway::instance()->Stop();
        }
        else
        {
           ACE_DEBUG((LM_DEBUG,
                    "Failed to start gateway, please check...\n"));
        }
    }
    else
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to initialize gateway, please check...\n"));
    }

    return 0;
}







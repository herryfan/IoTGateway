/*
*  This file defines CoAP Event Handler
*/

#ifndef __COAP_EVENT_HANDLER_H__
#define __COAP_EVENT_HANDLER_H__

#include "Config.h"

class CoAPWrapper;

class CoAPEventHandler : public ACE_Event_Handler
{
public:

    CoAPEventHandler(CoAPWrapper* ctx );
    ~CoAPEventHandler();

    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
    
    virtual ACE_HANDLE get_handle (void) const;
    


private:

    CoAPWrapper* ctx_;
    
};


#endif //__COAP_EVENT_HANDLER_H__




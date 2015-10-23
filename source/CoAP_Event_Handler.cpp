/*
* This file implement CoAP Event Handler
*/

#include "CoAP_Event_Handler.h"
#include "CoAP_Wrapper.h"


CoAPEventHandler::CoAPEventHandler(CoAPWrapper* ctx)
:ctx_(ctx)
{
    ACE_ASSERT(ctx_ != 0);
}

CoAPEventHandler::~CoAPEventHandler()
{
}

int CoAPEventHandler::handle_input (ACE_HANDLE fd)
{
    if (ctx_)
        ctx_->DoEventDispatch();
    
        
    return 0;
}

int CoAPEventHandler::handle_timeout (const ACE_Time_Value &tv,
                            const void *arg)
{
    return 0;
}

ACE_HANDLE CoAPEventHandler::get_handle (void) const
{
    ACE_DEBUG((LM_DEBUG,
               "call coap event handler get_handle\n"));
               
    if (ctx_)
    {
        return ctx_->GetMcastHandle();
    }

    return -1;    
}




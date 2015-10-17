/*
* This file implement CoAP Event Handler
*/

#include "CoAP_Event_Handler.h"
#include "coap.h"


CoAPEventHandler::CoAPEventHandler()
{
}

CoAPEventHandler::~CoAPEventHandler()
{
}

void CoAPEventHandler::SetCoAPCtx(struct coap_context_t* ctx)
{
    ctx_ = ctx;
}

int CoAPEventHandler::handle_input (ACE_HANDLE fd)
{
    ACE_DEBUG((LM_DEBUG,
                "call coap event handler handle_input\n"));


    if ( ctx_ )
    {
        coap_read( ctx_ );   /* read received data */
        coap_dispatch( ctx_ );   /* and dispatch PDUs from receivequeue */
    }
    
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
        return ctx_->sockfd;
    }

    return -1;    
}




#include "L3_FSMevent.h"
#include "L3_msg.h"
#include "L3_timer.h"
#include "L3_LLinterface.h"
#include "protocol_parameters.h"
#include "mbed.h"


//FSM state -------------------------------------------------
#define L3STATE_IDLE                0
#define L3STATE_TX					1
#define	L3STATE_RecPDU				2

//state variables
static uint8_t main_state = L3STATE_IDLE; //protocol state
static uint8_t prev_state = main_state;

//SDU (input)
static uint8_t originalWord[200];
static uint8_t wordLen=0;
static uint8_t loRegpdu;  
static uint8_t losignal_pdu;

static uint8_t sdu[200];
static uint8_t accept_sdu[200]; 

//serial port interface
static Serial pc(USBTX, USBRX);


//application event handler : generating SDU from keyboard input
static void L3service_processInputWord(void)
{
    char c = pc.getc();
    if (!L3_event_checkEventFlag(L3_event_dataToSend))
    {
        if (c == '\n' || c == '\r')
        {
            originalWord[wordLen++] = '\0';
            L3_event_setEventFlag(L3_event_dataToSend);
            debug_if(DBGMSG_L3,"word is ready! ::: %s\n", originalWord);
        }
        else
        {
            originalWord[wordLen++] = c;
            if (wordLen >= L3_MAXDATASIZE-1)
            {
                originalWord[wordLen++] = '\0';
                L3_event_setEventFlag(L3_event_dataToSend);
                pc.printf("\n max reached! word forced to be ready :::: %s\n", originalWord);
            }
        }
    }
}



void L3_initFSM()
{
    //initialize service layer
    pc.attach(&L3service_processInputWord, Serial::RxIrq);

    pc.printf("Give a word to send : ");
}

void L3_FSMrun(void)
{   
    if (prev_state != main_state)
    {
        debug_if(DBGMSG_L3, "[L3] State transition from %i to %i\n", prev_state, main_state);
        prev_state = main_state;
    }

    //FSM should be implemented here! ---->>>>
    switch (main_state)
    {
        case L3STATE_IDLE: //IDLE state description
            
            if (L3_event_checkEventFlag(L3_event_msgRcvd)) //if data reception event happens
            {
                //Retrieving data info.
                uint8_t* dataPtr = L3_LLI_getMsgPtr();
                uint8_t size = L3_LLI_getSize();

                debug("\n -------------------------------------------------\nRCVD MSG : %s (length:%i)\n -------------------------------------------------\n", 
                            dataPtr, size);
                
                pc.printf("Give a word to send : ");
                
                L3_event_clearEventFlag(L3_event_msgRcvd);
            }
            else if (L3_event_checkEventFlag(L3_event_dataToSend)) //if data needs to be sent (keyboard input)
                {
					// event a (SDU in, PDU send)
					if(L3)
						{
						
                    strcpy((char*)sdu, (char*)originalWord);
                    L3_LLI_dataReqFunc(losignal_pdu), wordLen);

                   // debug_if(DBGMSG_L3, "[L3] sending location siganl....\n");
                	L3_event_clearEventFlag(L3_event_msgRcvd);
				   	wordLen = 0;
					main_state = MAINSTATE_TX;
				}
            }
			
            break;
			
			case L3STATE_TX:
				if(L3_event_checkEventFlag(L3_event_dataToSend)) //if data reception happens
					{
					uint8_t* dataPtr = L3_LLI_getMsgPtr();
					uint8_t size = L3_LLI_getSize();

					debug("\n -------------------------------------------------\nRCVD MSG : %s (length:%i)\n -------------------------------------------------\n", 
                            dataPtr, size);
                
                	L3_event_clearEventFlag(L3_event_msgRcvd);
					}
				//else if (L3_event_checkEventFlag(L3_event_dataToSend)) //if data needs to be sent
				//	{
				//	L3_timer_startTimer();
				//	L3_event_clearEventFlag(L3_event_dataToSend);
				//	}
				else if (L3_event_checkEventFlag(L3_event_LoRemsg)) // receive 'location register message' 
					{
				L3_msg_encode(accept_sdu); // encode 'location register accept sdu'
				L3_LLI_dataReqFunc(accept_sdu, wordLen); // send 'location register accept sdu'?
				L3_event_clearEventFlag(L3_event_LoRemsg);
				wordLen = 0; //check if it is necessary
				main_state = L3STATE_RecPDU;
    				}
				
    		break; 
				
			case L3STATE_RecPDU:
				L3_timer_getTimerStatus();
				L3_timer_startTimer();
				if (L3_event_checkEventFlag()) // send location register pdu 
					{
					
					L3_timer_stopTimer();
					main_state = L3STATE_IDLE;
					}
				else if(L3_event_checkEventFlag(L3_event_timeout))
					{
					L3_event_clearEventFlag(L3_event_timeout);
					L3_timer_stopTimer();
					main_state = L3STATE_RecPDU; 
					}
					break;
    	}
				
					
					

        default :
            break;
    }
}
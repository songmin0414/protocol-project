#include "mbed.h"
#include "string.h"
#include "L2_FSMmain.h"
#include "L3_FSMmain.h"


//define FSM state
#define MAINSTATE_IDLE
#define MAINSTATE_TX
#define MAINSTATE_ACK
#define MAINSTATE_RecPDU

//serial port interface
Serial pc(USBTX, USBRX);

//GLOBAL variables (DO NOT TOUCH!) ------------------------------------------

//source/destination ID
uint8_t input_thisId=1;
uint8_t input_destId=0;

//FSM operation implementation ------------------------------------------------
int main(void){

    //initialization
    pc.printf("------------------ protocol stack starts! --------------------------\n");
        //source & destination ID setting
    pc.printf(":: ID for this node : ");
    pc.scanf("%d", &input_thisId);
    pc.printf(":: ID for the destination : ");
    pc.scanf("%d", &input_destId);
    pc.getc();

    pc.printf("endnode : %i, dest : %i\n", input_thisId, input_destId);
    
}
				
				
    //initialize lower layer stacks
    L2_initFSM(input_thisId, input_destId);
    L3_initFSM();
    
    //while(1)
    //{
        //L2_FSMrun();
        //L3_FSMrun();
    //}
//}while(1)
{
//debug message 
	if (prev_state != main_state)
		{
		debug_if(DBGMSG_ARQ, '[ARQ] State transition from %i to %i\n', prev_state, main_state);
		prev_state = main_state;
		}

	//FSM should be implemented here! ---
	switch (main_state)
		{
		case MAINSTATE_IDLE: 
			if (L3_event_checkEventFlag()) // if data reception event happens 
				{
				//Retrieving data info 
				uint8_t = 

				pc.printf()
				pduSize = Msg_encode(Ack, rxSeqNum); //need to generate function for encoding PDU
				L2_msg_encodeData(); //generate PDU
				main_state = MAINSTATE_TX; 
				flag_needPrint = 1;

				L2_event_clearEventFlag(); //need input variable 
				}
				else if 
		case MAINSTATE_TX : 
			if (L2_event_checkEventFlag(L2_event_dataTxDone)) //event b +timer start
				{
				timer = L2_timer_startTimer(); //need to set timer variable

				main_state = MAINSTATE_ACK; 
				L2_event_clearEventFlag(L2_event_dataTxDone); //need input variable 
				}
			else if (L2_event_checkEventFlag(L2_event_ackTxDone))
				{
				if (L2_timer_getTimerStatus() == 0 &&
					L2_event_checkEventFlag(L2_event_arqTimeout) == 0) //timer is running
					{
					main_state = MAINSTATE_IDLE;
						}
					L2_event_clearEventFlag(L2_event_dataTxDone);
				}
			else if (L2_event_checkEventFlag(L2_event_dataRcvd)) // event e 
				{
				//Retrieving data info.
				uint8_t srcId = L2_LLI_getSrcId();
				uint8_t *dataPtr = L2_LLI_getRcvdDataPtr();
				uint8_t size = L2_LLI_getSize();
				uint8_t rxSeqNum = L2_msg_getSeq(dataPtr); 

				pc.printf("\n ---------------------------------------\nRCVD from %i: %s(length:%i, seq:%i)\n ---------------------------------------\n",srcId, arqMsg_getWord(dataPtr), size, rxSeqNum);
				pduSize = L2_msg_encodeAck(arqAck, rxSeqNum);
				L2_LLI_sendData(arqAck, ARQMSG_ACKSIZE, srcId);

				pc.printf("[MAIN] sending ACK to %i(seq:%i)\n",srcId, rxSeqNum);
				
				main_state = MAINSTATE_RecPDU;
				
				}
			
		case MAINSTATE_ACK:
			if (L2_event_checkEventFlag(L2_event_ackRcvd)) //event c +timer stop
				{
				pc.printf("ACK received for PDU %i\n", seqNum-1);
				L2_timer_stopTimer();
				main_state = MAINSTATE_IDLE; 
				L2_event_clearEventFlag(L2_event_ackRcvd);
				}
			else if (L2_event_checkEventFlag(L2_event_dataRcvd))
				{
				//Retrieving data info. 
				}
			else if (L2_event_checkEventFlag(L2_event_arqTimeout)) // event d + retran
				{
				if (retxCnt < L2_ARQ_MAXRETRANSMISSION)
					{
					L2_LLI_sendData(arqPdu, pduSize, destL2ID);
					retxCnt = retxCnt + 1;
					main_state = MAINSTATE_TX;
					}
				else // time out + go idle state 
					{ 
					main_state = MAINSTATE_IDLE; 
					}
					L2_event_clearEventFlag(L2_event_arqTimeout);
				}
			break;
		case MAINSTATE_RecPDU :
			if (L2_event_checkEventFlag(L2_event_ackRcvd)) // f event
				{
				main_state = MAINSTATE_ACK;
				}
				L2_event_checkEventFlag(L2_event_ackRcvd);
			break;

			default:
				break;
		}
}
}
			

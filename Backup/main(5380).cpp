#include "mbed.h"
#include "string.h"
#include "L2_FSMmain.h"
#include "L3_FSMmain.h"


//define FSM state
#define MAINSTATE_IDLE

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
    
    
while(1)
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
			if () // if data reception event happens 
				{
				//Retrieving data info 
				uint8_t 
			
				
    //initialize lower layer stacks
    L2_initFSM(input_thisId, input_destId);
    L3_initFSM();
    
    while(1)
    {
        L2_FSMrun();
        L3_FSMrun();
    }
}
#include "FSM.h"

FSM::FSM()
{
	curSensor = 0;
}

FSM::input(int input)
{
	switch(input){
		case 0:
			right();
			break;
		case 1:
			left();
			break;
		case 2:
			select();
			break;
		case 3:
			record();
			break;
		default:
			// idk
	}
}

FSM::right()

FSM::left()

FSM::record()

FSM::select()
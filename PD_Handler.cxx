#ifndef PD_HANDLER_H_SENTRY
#define PD_HANDLER_H_SENTRY

#include "PD_Handler.H"


void PD_InputHandler::Processing(const char* str)
{
	int status;
	char* current; //current string
	while(str) {
		switch(*str) {
		case '-': //cmline option
			//it is option
			OptionHandler();
			break;
		case o_letter:
			//it is letter in original language
			break;
		case t_letter:
			//it is letter in translate language
			break;
		default:
			//incorrect input
		}
	}
}

#endif

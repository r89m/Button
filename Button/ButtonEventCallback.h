/*
 * Button.h
 *
 * Created: 18/11/2014 19:33:02
 *  Author: Richard
 */ 


#ifndef BUTTONEVENTCALLBACK_H
#define BUTTONEVENTCALLBACK_H

#include <Button.h>

class ButtonEventCallback{
	
	private:
		EventType _type = evtUninitialised;
		uint16_t _delay = 0;
		uint16_t _max_delay = 0;
		uint16_t _repeat_period = 0;
		uint8_t _execution_count = 1;
	
		void calculateNextExecutionTime();
	
		// Callbacks
		ButtonOnEventCallback _callback;
		ButtonOnEventRepeatCallback _callback_repeating;
	
		// Keep track of when the callback should be executed
		uint16_t _next_execution_time = 0;
	
	public:
		ButtonEventCallback();
		
		EventType getType();
		
		void setType(EventType);
		void setDelay(uint16_t);
		void setMaxDelay(uint16_t);
		void setRepetitionPeriod(uint16_t);
		void setCallback(ButtonOnEventCallback);
		void setRepeatingCallback(ButtonOnEventRepeatCallback);
		
		void executeCallbackIfTime(uint16_t, Button&);
		void reset();
	
};

#endif // BUTTONEVENTCALLBACK_H
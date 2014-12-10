/*
 * Button.h
 *
 * Created: 18/11/2014 19:33:02
 *  Author: Richard
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include <inttypes.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Button;

// Define callback types
typedef void (*ButtonOnPressCallback)(Button&);
typedef void (*ButtonOnEventCallback)(Button&, uint16_t);
typedef void(*ButtonOnEventRepeatCallback)(Button&, uint16_t, uint16_t);

class Button{
	
	private:
		uint32_t _button_pressed_timestamp;
		boolean _is_pressed;
		void (*_on_press_callback)(Button&);
		void (*_on_release_callback)(Button&, uint16_t);
		
		// Store initial delays
		uint16_t* _on_release_delay;
		uint16_t* _on_hold_delay;
		uint16_t* _on_hold_repeat_delay;
		
		// Store the max wait for release events
		uint16_t* _on_release_max_delay;
		
		// Store next execution times for repeating hold events
		uint32_t* _on_hold_repeat_next_execution;
		
		// Store repeat delays for repeating hold events
		uint16_t* _on_hold_repeat_repetition_delay;
		
		// Store execution count for repeating events
		uint16_t* _on_hold_repeat_execution_count;
		
		// Store actual callbacks
		ButtonOnEventCallback** _on_release_callbacks;
		ButtonOnEventCallback** _on_hold_callbacks;
		ButtonOnEventRepeatCallback** _on_hold_repeat_callbacks;
		
		// Store number of callbacks
		uint8_t _on_release_callbacks_count = 0;
		uint8_t _on_hold_callbacks_count = 0;
		uint8_t _on_hold_repeat_callbacks_count = 0;
		
	protected:
		virtual boolean _update_button_state()=0;
		void _button_pressed();
		void _button_released();
		void _button_held();
		uint16_t _button_time_elapsed();
		boolean performReleaseCallbackIfTime(uint8_t, Button&, uint16_t);
		boolean performHoldCallbackIfTime(uint8_t, Button&, uint16_t);
		boolean performHoldRepeatCallbackIfTime(uint8_t, Button&, uint16_t);
	
	public:
		void onPress(ButtonOnPressCallback);
		void onRelease(ButtonOnEventCallback);
		void onRelease(uint16_t, ButtonOnEventCallback);
		void onRelease(uint16_t, uint16_t, ButtonOnEventCallback);
		void onHold(uint16_t, ButtonOnEventCallback);
		void onHoldRepeat(uint16_t, uint16_t, ButtonOnEventRepeatCallback);
		
		void update();
		boolean isPressed();
};
  
#endif /* BUTTON_H_ */
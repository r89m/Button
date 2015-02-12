/*
 * Button.cpp
 *
 * Created: 18/11/2014 19:33:09
 *  Author: Richard
 */ 

#include "Button.h"
#include "ButtonEventCallback.h"

void Button::_button_pressed(){
	
	// Set the button pressed state to true
	_is_pressed = true;
	
	// Record when the button was originally pressed
	_button_pressed_timestamp = millis();
	
	// Fire the onPress callback if one has been specified
	if(_on_press_callback){
		_on_press_callback(*this);
	}
	
	// Reset all callbacks
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON; i++){
		_eventCallbacks[i]->reset();
	}	
}

void Button::_button_released(){

	// Set the button pressed state to false
	_is_pressed = false;
	
	_execute_callbacks();
	
	/*
	
	Serial.println("Do release callback");
	delay(500);
	
	// Fire the onRelease callback if one has been specified
	//if(_on_release_callback){
		//_on_release_callback(*this, _button_time_elapsed());
	//}
	
	Serial.println("Did release callback okay");
	delay(500);
	
	// Iterate 
	// Search for the most appropriate onRelease callback with wait - only do this if we have some release callbacks registered though
	if(_on_release_callbacks_count > 0){
		// Iterate from the end of the array, as we want the first callback who's "wait" has elapsed
		for(uint8_t i = _on_release_callbacks_count - 1; i >= 0; i--){
		  if(performReleaseCallbackIfTime(i, *this, _button_time_elapsed())){
			break;
		  }
		}
	}
	*/
}

void Button::_button_held(){
	
	_execute_callbacks();
	/*
	// Search for any appropriate onHold callbacks - non repeating first
	for(uint8_t i = 0; i < _on_hold_callbacks_count; i++){
		performHoldCallbackIfTime(i, *this, _button_time_elapsed());
	}
	// ... then repeating ones
	for(uint8_t i = 0; i < _on_hold_repeat_callbacks_count; i++){
		performHoldRepeatCallbackIfTime(i, *this, _button_time_elapsed());
	}
	*/
}

void Button::_execute_callbacks(){
	
	uint16_t button_time_elapsed = _button_time_elapsed();
	
	// Iterate over all callbacks
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++){
		_eventCallbacks[i]->executeCallbackIfTime(button_time_elapsed, *this);
	}
	
}

uint16_t Button::_button_time_elapsed(){
	
	return millis() - _button_pressed_timestamp;
}

/*
boolean Button::performReleaseCallbackIfTime(uint8_t index, Button& btn, uint16_t elapsed_time){
	
	// Get the necessary information
	uint16_t _delay = _on_release_delay[index];
	uint16_t _max_delay = _on_release_max_delay[index];
	
	ButtonOnEventCallback _callback;
	
	// Check whether we should perform the callback
	if((elapsed_time >= _delay) && (elapsed_time <= _max_delay)){
		// Do the callback
		_callback = *_on_release_callbacks[index];
		if(_callback != NULL){
			_callback(btn, elapsed_time);
			return true;
		}
	}
	
	// Return false by default
	return false;	
}

boolean Button::performHoldCallbackIfTime(uint8_t index, Button& btn, uint16_t elapsed_time){
	
	ButtonOnEventCallback _callback;
	
	// Check whether enough time has passed
	if(elapsed_time >= _on_hold_delay[index]){
		_callback = *_on_hold_callbacks[index];
		if(_callback != NULL){
			_callback(btn, elapsed_time);
			return true;
		}
	}
	
	// Otherwise, return false
	return false;
}

boolean Button::performHoldRepeatCallbackIfTime(uint8_t index, Button& btn, uint16_t elapsed_time){
	
	ButtonOnEventRepeatCallback _callback;
	
	// Check whether enough time has passed
	if(elapsed_time >= _on_hold_repeat_next_execution[index]){
		_callback = *_on_hold_repeat_callbacks[index];
		if(_callback != NULL){
			_callback(btn, _on_hold_repeat_execution_count[index], elapsed_time);
			// Increment execution count
			_on_hold_repeat_execution_count[index]++;
			// Recalculate next execution time
			_on_hold_repeat_next_execution[index] += _on_hold_repeat_repetition_delay[index];
			return true;
		}
	}
	
	// Otherwise, return false
	return false;
}
*/


void Button::update(){
	
	// Record the previous and new state of the button
	boolean _previous_button_state = isPressed();
	boolean _new_button_state = _update_button_state();
	
	// Record the new state of the button
	_is_pressed = _new_button_state;
	
	//Serial.println("Button Update");
		
	// If the state of the button has changed
	if(_previous_button_state != _new_button_state){
		// If the button is now pressed
		if(_new_button_state){
			_button_pressed();
		} else {
			// Otherwise if it has just been let go
			_button_released();
		}
	// If the state hasn't changed but the button is pressed - ie it is being held
	} else if(_new_button_state){
		_button_held();
	}	
}

void Button::onPress(ButtonOnPressCallback callback){
	
	_on_press_callback = callback;
}

CallbackAttachedResponse Button::onRelease(ButtonOnEventCallback callback){
	
	return onRelease(0, callback);
}

CallbackAttachedResponse Button::onRelease(uint16_t wait, ButtonOnEventCallback callback){
	
	return onRelease(wait, -1, callback);
}

CallbackAttachedResponse Button::onRelease(uint16_t wait, uint16_t max_wait, ButtonOnEventCallback callback){
	
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++){
		// If this callback handler has not be initialised, we can use it
		if(_eventCallbacks[i]->getType() == evtUninitialised){
			_eventCallbacks[i]->setType(evtRelease);
			_eventCallbacks[i]->setDelay(wait);
			_eventCallbacks[i]->setMaxDelay(max_wait);
			_eventCallbacks[i]->setCallback(callback);
			
			// Now that we're done, let the user know
			return attSuccessful;
		}
	}
	
	// If we get this far, there was no more space to add a handler
	return attNoMoreRoom;
	
	//uint8_t next_index = _on_release_callbacks_count + 1;
	
	/* Resize all the necessary arrays - callbacks, waits and max waits */
	/*
	// Callbacks
	_on_release_callbacks = (ButtonOnEventCallback**) realloc(_on_release_callbacks, next_index * sizeof(ButtonOnEventCallback*));
	// Waits
	_on_release_delay = (uint16_t*) realloc(_on_release_delay, next_index * sizeof(uint16_t*));
	// Max Delays
	_on_release_max_delay = (uint16_t*) realloc(_on_release_max_delay, next_index * sizeof(uint16_t*));
	
	// Store the info for the callback
	_on_release_callbacks[next_index] = &callback;
	_on_release_delay[next_index] = wait;
	_on_release_max_delay[next_index] = max_wait;
	
	// Increment callback counter
	_on_release_callbacks_count++;
	*/
}

CallbackAttachedResponse Button::onHold(uint16_t duration, ButtonOnEventCallback callback){
	
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++){
		// If this callback handler has not be initialised, we can use it
		if(_eventCallbacks[i]->getType() == evtUninitialised){
			_eventCallbacks[i]->setType(evtHold);
			_eventCallbacks[i]->setDelay(duration);
			_eventCallbacks[i]->setCallback(callback);
			
			// Now that we're done, let the user know
			return attSuccessful;
		}
	}
	
	// If we get this far, there was no more space to add a handler
	return attNoMoreRoom;
	//uint8_t next_index = _on_hold_callbacks_count + 1;
	
	/* Resize all the necessary arrays - callbacks and durations */
	/*
	// Callbacks
	_on_hold_callbacks = (ButtonOnEventCallback**) realloc(_on_hold_callbacks, next_index * sizeof(ButtonOnEventCallback*));
	// Delays
	_on_hold_delay = (uint16_t*) realloc(_on_hold_delay, next_index * sizeof(uint16_t*));
	
	// Store the info for the callback
	_on_hold_callbacks[next_index] = &callback;
	_on_hold_delay[next_index] = duration;
	
	// Increment callback counter
	_on_hold_callbacks_count++;
	*/
}

CallbackAttachedResponse Button::onHoldRepeat(uint16_t duration, uint16_t repeat_every, ButtonOnEventRepeatCallback callback){
 
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++){
		// If this callback handler has not be initialised, we can use it
		if(_eventCallbacks[i]->getType() == evtUninitialised){
			_eventCallbacks[i]->setType(evtHoldRepeat);
			_eventCallbacks[i]->setDelay(duration);
			_eventCallbacks[i]->setRepetitionPeriod(repeat_every);
			_eventCallbacks[i]->setRepeatingCallback(callback);
			
			// Now that we're done, let the user know
			return attSuccessful;
		}
	}
	
	// If we get this far, there was no more space to add a handler
	return attNoMoreRoom;
	// uint8_t next_index = _on_hold_repeat_callbacks_count + 1;
	
	/* Resize all the necessary arrays - callbacks, durations, repeat delays and next execution times */
	/*
	// Callbacks
	_on_hold_repeat_callbacks = (ButtonOnEventRepeatCallback**) realloc(_on_hold_repeat_callbacks, next_index * sizeof(ButtonOnEventRepeatCallback*));
	// Durations
	_on_hold_repeat_delay = (uint16_t*) realloc(_on_hold_repeat_delay, next_index * sizeof(uint16_t*));
	// Repeat Delays
	_on_hold_repeat_repetition_delay = (uint16_t*) realloc(_on_hold_repeat_repetition_delay, next_index * sizeof(uint16_t*));
	// Next Execution Times
	_on_hold_repeat_next_execution = (uint32_t*) realloc(_on_hold_repeat_next_execution, next_index * sizeof(uint32_t*));
	
	// Store the info for the callback
	_on_hold_repeat_callbacks[next_index] = &callback;
	_on_hold_repeat_delay[next_index] = duration;
	_on_hold_repeat_repetition_delay[next_index] = repeat_every;
	_on_hold_repeat_next_execution[next_index] = duration;
	
	// Increment callback counter
	_on_hold_repeat_callbacks_count++;
	*/
}


boolean Button::isPressed(){
	
	return _is_pressed;
}
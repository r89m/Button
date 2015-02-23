/*
 * MPR121Button.cpp
 *
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */ 

#include "MPR121Button.h"

MPR121Button::MPR121Button(Adafruit_MPR121 &touchSensor, uint8_t electrode){
	
	_touchSensor = &touchSensor;
	_electrode = electrode;
}

void MPR121Button::update(uint16_t latestReading){
	
	_latestReading = latestReading;
	Button::update();
}

boolean MPR121Button::_update_button_state(){
	
	uint16_t touchSwitchState;
	
	// Check to see if we were given a reading from the touch sensor (this will help minimise the requests over i2c)
	if(_latestReading){
		touchSwitchState = _latestReading;
		// Reset the latest reading variable to zero. This ensures that we don't hang on to an old value
		_latestReading = 0;
	} else {		
		// Read the latest value from the touch switch
		touchSwitchState = _touchSensor->touched();
	}
	
	// Return whether it is pressed or not
	return (bitRead(touchSwitchState, _electrode));
}
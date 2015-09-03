/*
 * CapacitiveButton.cpp
 *
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */ 

#include "CapacitiveButton.h"

CapacitiveButton::CapacitiveButton(uint8_t sendPin, uint8_t receivePin){
	
	// Initialise variables
	capSense = CapacitiveSensor(sendPin, receivePin);	// Called with pins set to -1 as there is not default constructor
	_threshold = 1000;
	_number_of_samples = 30;
}

void CapacitiveButton::configureButton(CapacitiveButtonConfigurationCallback configurationCallback){
	
	configurationCallback(capSense);	
}

void CapacitiveButton::setNumberOfSamples(uint8_t samples){
	
	_number_of_samples = samples;
}

void CapacitiveButton::setThreshold(long threshold){
	
	_threshold = threshold;
}


boolean CapacitiveButton::_update_button_state(){
	
	// Only bother checking the touch switch if a threshold has been set
	if(_threshold == 0){
		return false;
	}
	
	// Read the latest value from the touch switch
	long lastestReading = capSense.capacitiveSensor(_number_of_samples);
	// Return whether it is pressed or not
	return (lastestReading > _threshold);
}
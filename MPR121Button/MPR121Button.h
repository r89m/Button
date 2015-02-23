/*
 * CapacitiveButton.h
 *
 * Created: 18/11/2014 19:34:16
 *  Author: Richard
 */ 


#ifndef MPR121BUTTON_H_
#define MPR121BUTTON_H_

#include "Button.h"
#include <Adafruit_MPR121.h>

class MPR121Button : public Button {
	
	private:
		Adafruit_MPR121* _touchSensor;			// Store a reference to an MPR121 Touch Sensor
		uint8_t _electrode;						// Which electrode are we monitoring
		uint16_t _latestReading;			// The latest reading from the touch sensor. This helps minimise requests over the i2c bus
	
	protected:
		boolean _update_button_state();
	
	public:
		MPR121Button(Adafruit_MPR121&, uint8_t);	
		using Button::update;					// 'Unhide' the update method of the parent class Button
		void update(uint16_t);
};

#endif /* MPR121BUTTON_H_ */
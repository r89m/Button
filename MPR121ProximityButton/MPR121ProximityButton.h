/*
 * CapacitiveButton.h
 *
 * Created: 18/11/2014 19:34:16
 *  Author: Richard
 */ 


#ifndef MPR121PROXIMITYBUTTON_H_
#define MPR121PROXIMITYBUTTON_H_

#include "Button.h"
#include <Adafruit_MPR121.h>

class MPR121ProximityButton : public Button {
	
	private:
		Adafruit_MPR121* _touchSensor;			// Store a reference to an MPR121 Touch Sensor
		uint8_t _electrode;						// Which electrode are we monitoring
    
    void initSensor();
	
	protected:
		boolean _update_button_state();
	
	public:
		MPR121ProximityButton(Adafruit_MPR121&, uint8_t);
		using Button::update;					// 'Unhide' the update method of the parent class Button
		void update(uint16_t);
    void setThresholds(uint8_t, uint8_t);
    void setBaseline(uint16_t);
};

#endif /* MPR121PROXIMITYBUTTON_H_ */
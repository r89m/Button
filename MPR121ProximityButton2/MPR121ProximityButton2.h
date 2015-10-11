/*
 * CapacitiveButton.h
 *
 * Created: 18/11/2014 19:34:16
 *  Author: Richard
 */ 


#ifndef MPR121PROXIMITYBUTTON2_H_
#define MPR121PROXIMITYBUTTON2_H_

#include "Button.h"
#include <Adafruit_MPR121.h>

#if !defined(MPR121PROXIMITYBUTTON_RUNNING_AVERAGE_COUNT)
  #define MPR121PROXIMITYBUTTON_RUNNING_AVERAGE_COUNT 5
#endif

class MPR121ProximityButton2 : public Button {
	
	private:
		Adafruit_MPR121* _touchSensor;			// Store a reference to an MPR121 Touch Sensor
		uint8_t _electrode;						// Which electrode are we monitoring
		uint16_t _last_reading;			// The last reading from the touch sensor
    uint16_t _pre_touch_reading;   // The reading taken before the electrode was touched
    uint8_t _threshold_touch = 40;
    uint8_t _threshold_release = 5;
    uint16_t _threshold_touch_raw;
    uint16_t _threshold_release_raw;
    uint32_t _last_reading_timestamp;
    uint16_t _reading_delay = 30;        // Default to waiting at least 10 ms between reading the touch sensor
    
    int32_t _avg_last_differentials[MPR121PROXIMITYBUTTON_RUNNING_AVERAGE_COUNT];
    uint8_t _avg_index = 0;
    uint8_t _avg_count = 0;
    int32_t _avg_sum = 0;
    
    void clearThresholds();
    int32_t differentialRunningAverage(int32_t);
	
	protected:
		boolean _update_button_state();
	
	public:
		MPR121ProximityButton2(Adafruit_MPR121&, uint8_t);	
    void setThresholds(uint8_t, uint8_t);
    void setThresholdsRaw(uint16_t, uint16_t);
    void setRefreshDelay(uint16_t);
};

#endif /* MPR121PROXIMITYBUTTON2_H_ */
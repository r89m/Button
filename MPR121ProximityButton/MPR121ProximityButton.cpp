/*
 * MPR121ProximityButton.cpp
 *
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */ 

#include <Adafruit_MPR121.h>
#include "MPR121ProximityButton.h"

MPR121ProximityButton::MPR121ProximityButton(Adafruit_MPR121 &touchSensor, uint8_t electrode){
	
	_touchSensor = &touchSensor;
	_electrode = electrode;
  
  initSensor();
}

boolean MPR121ProximityButton::_update_button_state(){
  
  uint32_t time_now = millis();
  uint16_t time_since_last_check = time_now - _last_reading_timestamp;
  boolean new_button_state = isPressed();    // Default to the existing state
  
  // Check whether enough time has passed to read from the sensor again
  if(time_since_last_check > _reading_delay){
    uint16_t current_reading = _touchSensor->filteredData(_electrode);
    
    int32_t delta = (int32_t)_last_reading - (int32_t)current_reading;
    int32_t current_diff = (delta * 1000) / time_since_last_check;
    
    // Filter out unrealistic changes
    if(abs(current_diff) > 2000){
      _last_reading = current_reading;
      return isPressed();
    }
    
    // Get the rate of change of the reading - average out over the last MPR121PROXIMITYBUTTON_RUNNING_AVERAGE_COUNT readings
    int32_t differential = differentialRunningAverage(current_diff);
    
    // Create variables to store the thresholds - default to the raw value - proportional ones can be calculated in a second if needs be
    int32_t current_threshold_touch = _threshold_touch_raw;
    int16_t current_threshold_release = _threshold_release_raw;
    
    
    // Check whether to do proportional comparison
    if(_threshold_touch != 0 && _threshold_release != 0){
      int16_t datum_reading;
      if(isPressed()){
        datum_reading = (int16_t) _pre_touch_reading;
        current_threshold_release = (int16_t)((datum_reading * _threshold_release) / 100);
      } else {
        datum_reading = ((int16_t) _last_reading);
        current_threshold_touch = (((((int32_t)datum_reading * _threshold_touch) / 100) * 1000) / (time_since_last_check * 100));
      }        
    }      
    
    int16_t calc_reading_pre;
    int16_t calc_reading_current = current_reading;
    
    // If the button is currently pressed...
    if(isPressed()){
      calc_reading_pre = _pre_touch_reading;
      // Check if the button has been released
      if((calc_reading_pre - calc_reading_current) < current_threshold_release){
        new_button_state = false;
      }
    } else {
      calc_reading_pre = _last_reading;
      // Check if the button has been pressed
      if(_last_reading > 0 && differential > current_threshold_touch){
        new_button_state = true;
        _pre_touch_reading = _last_reading;
      }          
    }
    
    // Record these values for use next time round the loop
    _last_reading = current_reading;
    _last_reading_timestamp = time_now;       
  }
	
	// Return whether it is pressed or not
	return new_button_state;
}

void MPR121ProximityButton::setThresholds(uint8_t touchThreshold, uint8_t releaseThreshold){
  
  _touchSensor->setThreshholds(touchThreshold, releaseThreshold);
}

void MPR121ProximityButton::setBaseline(uint16_t baseline){
  
  _touchSensor->writeRegister()
}
/*
 * MPR121ProximityButton.cpp
 *
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */ 

#include "MPR121ProximityButton2.h"

MPR121ProximityButton2::MPR121ProximityButton2(Adafruit_MPR121 &touchSensor, uint8_t electrode){
	
	_touchSensor = &touchSensor;
	_electrode = electrode;
}

boolean MPR121ProximityButton2::_update_button_state(){
  
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

void MPR121ProximityButton2::clearThresholds(){
  
  // Ensure the proportional values are zero
  _threshold_touch = 0;
  _threshold_release = 0;
  
  // Ensure the raw values are zero
  _threshold_touch_raw = 0;
  _threshold_release_raw = 0;
}

void MPR121ProximityButton2::setThresholds(uint8_t touchThreshold, uint8_t releaseThreshold){
  
  clearThresholds();
  
  // Save the new thresholds
  _threshold_touch = touchThreshold;
  _threshold_release = releaseThreshold;
}

void MPR121ProximityButton2::setThresholdsRaw(uint16_t touchThreshold, uint16_t releaseThreshold){
  
  clearThresholds();
  
  // Save the new thresholds
  _threshold_touch_raw = touchThreshold;
  _threshold_release_raw = releaseThreshold;
}

void MPR121ProximityButton2::setRefreshDelay(uint16_t delay){
  
  _reading_delay = delay;
}

int32_t MPR121ProximityButton2::differentialRunningAverage(int32_t last_differential){

  // keep sum updated to improve speed.
  _avg_sum -= _avg_last_differentials[_avg_index];
  _avg_last_differentials[_avg_index] = last_differential;
  _avg_sum += _avg_last_differentials[_avg_index];
  _avg_index = _avg_index % MPR121PROXIMITYBUTTON_RUNNING_AVERAGE_COUNT;
  if (_avg_count < MPR121PROXIMITYBUTTON_RUNNING_AVERAGE_COUNT) _avg_count++;

  return _avg_sum / _avg_count;
}
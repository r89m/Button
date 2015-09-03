/*
 * CapactiveButton example. This shows a basic example of how to register events with two touch sensitive electrodes.
 */

#include <Button.h>
#include <ButtonEventCallback.h>
#include <CapacitiveButton.h>
#include <CapacitiveSensor.h>    // https://github.com/PaulStoffregen/CapacitiveSensor

// Create an instance of CapacitiveButton charging pin 3 and reading pin 4
CapacitiveButton button1 = CapacitiveButton(3, 4);
// Create another instance of CapacitiveButton charging pin 3 and reading pin 5 (remember, the same pin can charge several touch sensor electrodes)
CapacitiveButton button2 = CapacitiveButton(3, 5);

void setup(){

        // Open up the serial port so that we can write to it
	      Serial.begin(9600);

        // Configure both touch sensor electrodes in the same way - they can be configure individually by passing a different function in as an argument
        // This step is not necessary if you're happy with the defaults
        button1.configureButton(configureCapacitiveButton);
        button2.configureButton(configureCapacitiveButton);

        // Set the touch threshold for each electrode (default is 1000)
        button1.setThreshold(500);      // More sensitive
        button2.setThreshold(2500);     // Less sensitive

        // Set how many samples you want to consider for electrode 1 (default is 30)
        button1.setNumberOfSamples(50);

        /* ==== Electrode 1 ==== */
        // When electrode 1 is first pressed, call the function onButtonPressed (further down the page)
        button1.onPress(onButtonPressed);
        // Once electrode 1 has been held for 1 second (1000ms) call onButtonHeld. Call it again every 0.5s (500ms) until it is let go
	      button1.onHoldRepeat(1000, 500, onButtonHeld);

        /* ==== Electrode 2 ==== */
        // When electrode 2 is released, call onButtonReleased
        button2.onRelease(onButtonReleased);
}

void loop(){
        // Check the state of the electrodes
        button1.update();
        button2.update();
}

// Use this function to configure the internal CapSense object to suit you. See the documentation at: http://playground.arduino.cc/Main/CapacitiveSensor
// This function can be left out if the defaults are acceptable - just don't call configureButton
void configureCapacitiveButton(CapacitiveSensor& capSense){

        // Set the capacitive sensor to timeout after 300ms
        capSense.set_CS_Timeout_Millis(300);
        // Set the capacitive sensor to auto-calibrate every 10secs
        capSense.set_CS_AutocaL_Millis(10000);
}

// btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
void onButtonPressed(Button& btn){

        Serial.println("button pressed");
}

// duration reports back how long it has been since the button was originally pressed.
// repeatCount tells us how many times this function has been called by this button.
void onButtonHeld(Button& btn, uint16_t duration, uint16_t repeatCount){

        Serial.print("button has been held for ");
        Serial.print(duration);
        Serial.print(" ms; this event has been fired ");
        Serial.print(repeatCount);
        Serial.println(" times");
}

// duration reports back the total time that the button was held down
void onButtonReleased(Button& btn, uint16_t duration){

        Serial.print("button released after ");
        Serial.print(duration);
        Serial.println(" ms");
}

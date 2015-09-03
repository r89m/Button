/*
 * MPR121Button example. This shows a basic example of how to receive events from electrodes on an MPR121 Capactive Touch IC
 */

#include <Button.h>
#include <ButtonEventCallback.h>
#include <MPR121Button.h>
#include <Adafruit_MPR121.h>    // https://github.com/adafruit/Adafruit_MPR121_Library
#include <Wire.h>

// Create an instance of Adafruit_MPR121 to communicate with your IC via i2C
Adafruit_MPR121 touchSensor = Adafruit_MPR121();

// Create a couple of instances of MPRButton reading electrodes 1, 2 and 3
MPR121Button button1 = MPR121Button(touchSensor, 1);
MPR121Button button2 = MPR121Button(touchSensor, 2);
MPR121Button button3 = MPR121Button(touchSensor, 3);

void setup(){

        // Open up the serial port so that we can write to it
        Serial.begin(9600);

        // Initialise the touch sensor
        touchSensor.begin();

        /* ==== Electrode 1 ==== */
        // When electrode 1 is first pressed, call the function onButtonPressed (further down the page)
        button1.onPress(onButtonPressed);

        /* ==== Electrode 2 ==== */
        // Once electode 2 has been held for 1 second (1000ms) call onButtonHeld. Call it again every 0.5s (500ms) until it is let go
        button2.onHoldRepeat(1000, 500, onButtonHeld);

        /* ==== Electrode 3 ==== */
        // When electrode 3 is released, call onButtonReleased
        button3.onRelease(onButtonReleased);
}

void loop(){
        // Check the state of the button - this is not necessarily the most efficient way of doing this. For a better way see the example MPR121Button_Fast
        button1.update();
        button2.update();
        button3.update();
}

// btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
void onButtonPressed(Button& btn){

        Serial.println("electrode touched");
}

// duration reports back how long it has been since the button was originally pressed.
// repeatCount tells us how many times this function has been called by this button.
void onButtonHeld(Button& btn, uint16_t duration, uint16_t repeatCount){

        Serial.print("electrode has been held for ");
        Serial.print(duration);
        Serial.print(" ms; this event has been fired ");
        Serial.print(repeatCount);
        Serial.println(" times");
}

// duration reports back the total time that the button was held down
void onButtonReleased(Button& btn, uint16_t duration){

        Serial.print("electrode released after ");
        Serial.print(duration);
        Serial.println(" ms");
}

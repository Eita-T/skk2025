#include <Modulino.h>

// Create object instance
ModulinoButtons buttons;

void setup() {
  Serial.begin(115200);
  Modulino.begin();
  buttons.begin();
  
  // Function to control the LEDs on top of each button
  buttons.setLeds(true, true, true);
}

void loop() {
  static int iicbuttonState = 0;
  // Request new data from the Modulino Buttons
  if (buttons.update()) {
    if (buttons.isPressed(0) && iicbuttonState/4 == 0){
      iicbuttonState += 4;
    }
    if(!(buttons.isPressed(0)) && iicbuttonState/4 == 1){
      iicbuttonState -=4;
    }
    if (buttons.isPressed(1) && (iicbuttonState%4)/2 == 0){
      iicbuttonState += 2;
    }
    if(!(buttons.isPressed(1)) && (iicbuttonState%4)/2 == 1){
      iicbuttonState -=2;
    }
    if (buttons.isPressed(2) && iicbuttonState%2 == 0){
      iicbuttonState += 1;
    }
    if(!(buttons.isPressed(2)) && iicbuttonState%2 == 1){
      iicbuttonState -=1;
    }
  }
  Serial.println(iicbuttonState);
}
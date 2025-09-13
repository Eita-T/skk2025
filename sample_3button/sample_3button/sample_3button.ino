#include <Modulino.h>
#include "analogWave.h" // Include the library for analog waveform generation

analogWave wave(DAC);   // Create an instance of the analogWave class, using the DAC pin
ModulinoButtons buttons;

int freq_count = 0;  // in hertz, change accordingly
int freq[8] = {261, 294, 330, 349, 392, 440, 494, 522};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wave.sine(10);       // Generate a sine wave with the initial frequency
  Modulino.begin();
  buttons.begin();
}

void loop() {
  int buttonState = 0;
  // put your main code here, to run repeatedly:
  // Request new data from the Modulino Buttons
  if (buttons.update()) {
    // Check if any button has been pressed
    if (buttons.isPressed(0)){
      buttonState += 4;
    }
    if (buttons.isPressed(1)){
      buttonState += 2;
    }
    if (buttons.isPressed(2)){
      buttonState += 1;
    }
  }
  Serial.println(String(buttonState));
  if(buttonState!=0){
    wave.freq(freq[buttonState]);  // Set the frequency of the waveform generator to the updated value
  }else{
    wave.stop();
  }
  delay(1000);
}

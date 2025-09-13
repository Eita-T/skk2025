#include <Modulino.h>
#include "analogWave.h" // Include the library for analog waveform generation

analogWave wave(DAC);   // Create an instance of the analogWave class, using the DAC pin
ModulinoButtons buttons;

int freq_count = 0;  // in hertz, change accordingly
int freq[9] = {0, 261, 293, 329, 392, 440, 494, 587, 523};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wave.sine(10);       // Generate a sine wave with the initial frequency
  Modulino.begin();
  buttons.begin();
  pinMode(7, INPUT);
}

void loop() {
  int iicbuttonState = 0, buttonState = 0;
  // put your main code here, to run repeatedly:
  // Request new data from the Modulino Buttons
  if (buttons.update()) {
    // Check if any button has been pressed
    if (buttons.isPressed(0)){
      iicbuttonState += 4;
    }
    if (buttons.isPressed(1)){
      iicbuttonState += 2;
    }
    if (buttons.isPressed(2)){
      iicbuttonState += 1;
    }
  }
  if(!(digitalRead(7))){
    buttonState = 1;
  }
  Serial.print(String(iicbuttonState));
  Serial.print(" ");
  Serial.println(String(buttonState));
  if(iicbuttonState!=0){
    wave.freq(freq[iicbuttonState]);  // Set the frequency of the waveform generator to the updated value
  }else if(buttonState!=0){
    wave.freq(freq[8]);
  }else{
    wave.stop();
  }
  delay(1000);
}

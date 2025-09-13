/*
  SineWave

  Generates a pre-generated sawtooth-waveform.

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/dac
*/

#include "analogWave.h" // Include the library for analog waveform generation
#include "Arduino_LED_Matrix.h"
#include <Modulino.h>

analogWave wave(DAC);   // Create an instance of the analogWave class, using the DAC pin
ArduinoLEDMatrix matrix;
ModulinoButtons buttons;

int freq_count = 0;  // in hertz, change accordingly
int freq[8] = {261, 294, 330, 349, 392, 440, 494, 522};
int pinx[4] = {3, 4, 5, 6};
int buttonState[4] = {0,0,0,0}; // initial state 

void setup() {
  Serial.begin(115200);  // Initialize serial communication at a baud rate of 115200
  wave.sine(10);       // Generate a sine wave with the initial frequency
  matrix.begin(); // need matrix
  pinMode(pinx[0], INPUT);
  pinMode(pinx[1], INPUT);
  pinMode(pinx[2], INPUT);
  pinMode(pinx[3], INPUT);
  Modulino.begin();
  buttons.begin();
  // Function to control the LEDs on top of each button
  buttons.setLeds(true, true, true);
}

void loop() {
  // Read an analog value from pin A5 and map it to a frequency range
  //freq = map(analogRead(A5), 0, 1024, 0, 10000);
  uint8_t frame[8][12] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };     // initial matrix
   

  // Reading Button
  for(int i=0; i<4; i++){
    if(i=3){
      buttonState[i] = digitalRead(pinx[i]);
    }else{
      buttonState[i] = buttons.isPressed(i);
    }
  }

  // update frame
  for(int i=0; i<4; i++){
    frame[1][i+1] = buttonState[i];
  }

  // for(int i=0; i<4; i++){
  //   buttonState[i] = digitalRead(pin[i]);
  //   frame[1][i+1] = buttonState[i];
    // frame[0][i] = degitalRead(pin[i]);
  // }

  matrix.renderBitmap(frame, 8, 12);    // display matrix
  
  // Print the updated frequency to the serial monitor
  // Serial.println("Frequency is now " + String(freq[freq_count]) + " hz");
  for(int i=0; i<4; i++){
    Serial.println(String(buttonState[i]));
  }
  Serial.println("Input Bit is now " + String(frame[1][1]) + String(frame[1][2]) + String(frame[1][3]) + String(frame[1][4]) );

  // wave.freq(freq[freq_count]);  // Set the frequency of the waveform generator to the updated value
  delay(1000);      // Delay for one second before repeating
  // freq_count++;
  // if(freq_count >= 8){
  //   freq_count = 0;
  // }
}
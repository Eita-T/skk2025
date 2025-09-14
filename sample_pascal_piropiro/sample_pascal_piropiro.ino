#include <Modulino.h>
#include "analogWave.h" // Include the library for analog waveform generation

analogWave wave(DAC);   // Create an instance of the analogWave class, using the DAC pin
ModulinoButtons buttons;

int freq_count = 0;  // in hertz, change accordingly
int freq[9] = {0, 261, 293, 329, 392, 440, 494, 587, 523};
int pascalState = 0;
bool isplusflag = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wave.sine(10);       // Generate a sine wave with the initial frequency
  Modulino.begin();
  buttons.begin();
  pinMode(7, INPUT);
  pinMode(A2,INPUT);
}

void loop() {
  static int iicbuttonState = 0, buttonState = 0;
  // put your main code here, to run repeatedly:
  // Request new data from the Modulino Buttons
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

  // pascal sensor
  pascalState = analogRead(A2);
  Serial.println("pascal = " + String(pascalState));

  Serial.print(String(iicbuttonState));
  Serial.print(" ");
  Serial.println(String(buttonState));
  if(iicbuttonState!=0){
    // lets piropiro!
    if (isplusflag==0 && pascalState>300){
      wave.freq(freq[iicbuttonState] + pascalState);  // Set the frequency of the waveform generator to the updated value
      Serial.println(String(freq[iicbuttonState] + pascalState));
    }else if (isplusflag==1 && pascalState>300){
      wave.freq(freq[iicbuttonState] - pascalState);  // Set the frequency of the waveform generator to the updated value
      Serial.println(String(freq[iicbuttonState] - pascalState));
    }else{
      wave.freq(freq[iicbuttonState]);
    }
    isplusflag = !isplusflag;
  }else if(buttonState!=0){
    wave.freq(freq[8]);
  }else{
    wave.stop();
  }



  delay(100);
}

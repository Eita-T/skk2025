#include <Modulino.h>
#include "analogWave.h"

analogWave wave(DAC);
ModulinoKnob knob;

void setup() {
   Serial.begin(9600);
   wave.sine(440);
   Modulino.begin();
   knob.begin();
}

void loop(){
   int position = knob.get();
   bool click = knob.isPressed();
   float volume = (float)(position%48) / 47;
   if(volume < 0){
      volume = 0;
   }
   wave.amplitude(volume);
   Serial.print("position: ");
   Serial.print(position);
   Serial.print(" volume: ");
   Serial.println(volume);

   if(click){
      Serial.println("Clicked!");
   }

}
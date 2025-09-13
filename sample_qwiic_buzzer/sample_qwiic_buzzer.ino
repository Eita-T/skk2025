#include <Modulino.h>

ModulinoBuzzer buzzer;

int f_do = 261, f_mi = 330, f_so = 392;  // Frequency of the tone in Hz
int duration = 1000;  // Duration of the tone in milliseconds

void setup(){
  Modulino.begin();
  buzzer.begin();
}

void loop(){
  buzzer.tone(f_do, duration);  // Generate the tone
  // delay(duration);
  buzzer.tone(f_mi, duration);  // Generate the tone
  // delay(duration);
  buzzer.tone(f_so, duration);  // Generate the tone
  // delay(1000);  // Wait for 1 second
  // buzzer.tone(0, duration);  // Stop the tone
  // delay(duration);  // Wait for 1 second
}
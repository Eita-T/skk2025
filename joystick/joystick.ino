#include <Modulino.h>

ModulinoBuzzer buzzer;

int duration = 1000;
int JoyStick_X = 4; // x
int JoyStick_Y = 5; // y
int JoyStick_Z = 3; // key

int x, y, z; // ← これをグローバルスコープの上に移動（重要）

void setup() {
  pinMode(JoyStick_Z, INPUT);
  Modulino.begin();
  buzzer.begin();
  Serial.begin(9600);
}

void loop() {
  x = analogRead(JoyStick_X);
  y = analogRead(JoyStick_Y);
  z = digitalRead(JoyStick_Z);

  int freq = 0;

  if (z == 1) {
    freq = 220;
  } else {
    if (x >= 900 && y >= 900) {
      freq = 130;
      Serial.println("a");
    } else if (x >= 900 && y <= 200) {
      freq = 147;
      Serial.println("b");
    } else if (x <= 200 && y <= 200) {
      freq = 174;
      Serial.println("c");
    } else if (x <= 200 && y >= 900) {
      freq = 195;
      Serial.println("d");
    } else {
      freq = 0;
    }
  }

  if (freq > 0) {
      Serial.println(freq);
    buzzer.tone(freq, duration);
  }

  Serial.print(x, DEC);
  Serial.print(",");
  Serial.print(y, DEC);
  Serial.print(",");
  Serial.println(z, DEC);

  delay(1000);
}

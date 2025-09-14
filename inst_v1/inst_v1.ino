#include <Modulino.h>
ModulinoButtons buttons;

const int speakerPin = A0;
const int pascalPin  = A2;
const int joyY       = A4; // Y軸（半音上げ下げ）
const int joyX       = A5; // X軸（オクターブ上下）

int chromaticFreq[] = {
  261, 277, 293, 311, 329, 349,
  370, 392, 415, 440, 466, 494, 523
};

int naturalIndex[9] = {0, 0, 2, 4, 5, 7, 9, 11, 12};

int pascalState = 0;
bool isplusflag = 0;

void setup() {
  Serial.begin(115200);
  Modulino.begin();
  buttons.begin();
  pinMode(pascalPin, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyX, INPUT);
}

void loop() {
  static int iicbuttonState = 0, buttonState = 0;

  if (buttons.update()) {
    if (buttons.isPressed(0) && iicbuttonState/4 == 0) iicbuttonState += 4;
    if (!(buttons.isPressed(0)) && iicbuttonState/4 == 1) iicbuttonState -= 4;
    if (buttons.isPressed(1) && (iicbuttonState%4)/2 == 0) iicbuttonState += 2;
    if (!(buttons.isPressed(1)) && (iicbuttonState%4)/2 == 1) iicbuttonState -= 2;
    if (buttons.isPressed(2) && iicbuttonState%2 == 0) iicbuttonState += 1;
    if (!(buttons.isPressed(2)) && iicbuttonState%2 == 1) iicbuttonState -= 1;
  }

  pascalState = analogRead(pascalPin);

  int baseIndex = -1;
  int baseFreq = 0;

  if (iicbuttonState != 0) {
    baseIndex = naturalIndex[iicbuttonState];
    baseFreq = chromaticFreq[baseIndex];
  } else if (buttonState != 0) {
    baseIndex = naturalIndex[8];
    baseFreq = chromaticFreq[baseIndex];
  }

  if (baseIndex >= 0) {
    int joyValY = analogRead(joyY);
    if (joyValY > 800 && baseIndex < 12) {
      baseIndex++;
    } else if (joyValY < 200 && baseIndex > 0) {
      baseIndex--;
    }

    int playFreq = chromaticFreq[baseIndex];

    int joyValX = analogRead(joyX);
    if (joyValX < 200) {           // 左 → オクターブ上
      playFreq *= 2;
    } else if (joyValX > 800) {    // 右 → オクターブ下
      playFreq /= 2;
    }

    tone(speakerPin, playFreq);
    Serial.print("freq: ");
    Serial.println(playFreq);

  } else {
    noTone(speakerPin);
  }

  delay(100);
}

#include <Modulino.h>
ModulinoButtons buttons;

const int speakerPin = A0;   // スピーカー
const int pascalPin  = A2;   // センサ
const int octaveBtn  = 7;    // オクターブアップスイッチ
const int joyY       = A4;   // ジョイスティックY軸

// クロマチックスケール（半音階）
int chromaticFreq[] = {
  261, // C
  277, // C#
  293, // D
  311, // D#
  329, // E
  349, // F
  370, // F#
  392, // G
  415, // G#
  440, // A
  466, // A#
  494, // B
  523  // C
};

// ドレミファソラシド (自然音階のベース)
int naturalIndex[9] = {0, 0, 2, 4, 5, 7, 9, 11, 12};
//   freq[1]=C(261) → chromatic[0]
//   freq[2]=D(293) → chromatic[2]
//   freq[3]=E(329) → chromatic[4]
//   freq[4]=F(349) → chromatic[5]
//   freq[5]=G(392) → chromatic[7]
//   freq[6]=A(440) → chromatic[9]
//   freq[7]=B(494) → chromatic[11]
//   freq[8]=C(523) → chromatic[12]

int pascalState = 0;
bool isplusflag = 0;

void setup() {
  Serial.begin(115200);
  Modulino.begin();
  buttons.begin();
  pinMode(octaveBtn, INPUT_PULLUP);
  pinMode(pascalPin, INPUT);
  pinMode(joyY, INPUT);
}

void loop() {
  static int iicbuttonState = 0, buttonState = 0;

  // ボタン処理（省略は元コード通り）
  if (buttons.update()) {
    if (buttons.isPressed(0) && iicbuttonState/4 == 0) iicbuttonState += 4;
    if (!(buttons.isPressed(0)) && iicbuttonState/4 == 1) iicbuttonState -= 4;
    if (buttons.isPressed(1) && (iicbuttonState%4)/2 == 0) iicbuttonState += 2;
    if (!(buttons.isPressed(1)) && (iicbuttonState%4)/2 == 1) iicbuttonState -= 2;
    if (buttons.isPressed(2) && iicbuttonState%2 == 0) iicbuttonState += 1;
    if (!(buttons.isPressed(2)) && iicbuttonState%2 == 1) iicbuttonState -= 1;
  }

  pascalState = analogRead(pascalPin);

  int baseIndex = -1;  // chromaticFreq のインデックス
  int baseFreq = 0;

  if (iicbuttonState != 0) {
    baseIndex = naturalIndex[iicbuttonState];  // ドレミの位置をクロマチックに変換
    baseFreq = chromaticFreq[baseIndex];
  } else if (buttonState != 0) {
    baseIndex = naturalIndex[8];
    baseFreq = chromaticFreq[baseIndex];
  }

  if (baseIndex >= 0) {
    // 🎮 ジョイスティックで半音移動
    int joyVal = analogRead(joyY);

    if (joyVal > 800 && baseIndex < 12) {     // 上に倒す → 半音上
      baseIndex++;
    } else if (joyVal < 200 && baseIndex > 0) { // 下に倒す → 半音下
      baseIndex--;
    }

    int playFreq = chromaticFreq[baseIndex];

    // オクターブアップ（スイッチ押下時）
    if (digitalRead(octaveBtn) == LOW) {
      playFreq *= 2;
    }

    tone(speakerPin, playFreq);
    Serial.print("freq: ");
    Serial.println(playFreq);

  } else {
    noTone(speakerPin);
  }

  delay(100);
}

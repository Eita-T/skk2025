#include <Modulino.h>
#include "Arduino_LED_Matrix.h"
#include "FspTimer.h"

ModulinoButtons buttons;
ArduinoLEDMatrix matrix;
FspTimer _timer;

const int speakerPin = A0;
const int pascalPin  = A2;
const int joyY       = A4; // Y軸（半音上げ下げ）
const int joyX       = A5; // X軸（オクターブ上下）
const int joyZ       = 3;    // ジョイスティックZ軸(ボタン)

int chromaticFreq[] = {
  261, 277, 293, 311, 329, 349,
  370, 392, 415, 440, 466, 494, 523
};

int naturalIndex[9] = {0, 0, 2, 4, 5, 7, 9, 11, 12};

int pascalState = 0;
bool isplusflag = 0, isMatrixOn = 0, timerFlag = 0;
int matrixMode = 0;

static int timercount = 0;

const uint32_t p0[] = {
  0x00000000,
  0x00000000,
  0x00000000
};

const uint32_t p1[] = {
  0xFFFFFFFF,
  0xFFFFFFFF,
  0xFFFFFFFF,
};

const uint32_t p2[] = {
  0x000100FF,
  0x0007FE10,
  0x0000FFF1
};

const uint32_t p3[] = {
  0x0002227E,
  0x0002227E,
  0x00007E02
};



const uint32_t* frames[] = {p0, p1};  // フレーム配列をまとめる

const uint32_t on[] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
};
const uint32_t off[] = {
    0x00000000,
    0x00000000,
    0x00000000
};

void callbackfunc(timer_callback_args_t *arg) {
  isMatrixOn = !isMatrixOn;
  // if(timercount > 10){
  //   matrixMode++;
  //   timercount = 0;
  // }
  // timercount = timercount+1;
}


void setup() {
  Serial.begin(115200);
  Modulino.begin();
  buttons.begin();
  matrix.begin();
  pinMode(pascalPin, INPUT);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyZ, INPUT);

  uint8_t type;
  int8_t ch = FspTimer::get_available_timer(type);
  if (ch < 0) {
    return;
  }
  _timer.begin(TIMER_MODE_PERIODIC, type, ch, 10.0f, 50.0f, callbackfunc, nullptr);
  _timer.setup_overflow_irq();
  _timer.open();
  _timer.start();
}

void loop() {
  static int iicbuttonState = 0, buttonState = 0;
  static int interval = 120;
  static int pre_interval = 120;
  int joyZState = digitalRead(joyZ);

  pre_interval = interval;
  if (joyZState==1 && buttons.isPressed(0)==1){
    interval = interval + 60;
    if (interval > 300){
      interval =300;
    }
  }else if (joyZState==1 && buttons.isPressed(2)==1){
    interval = interval - 60;
    if (interval < 0){
      interval = 60;
    }
  }

  if (interval!=pre_interval){
    Serial.println(interval);
    uint8_t type;
    int8_t ch = FspTimer::get_available_timer(type);
    if (ch < 0) {
      return;
    }
    _timer.stop();
    _timer.begin(TIMER_MODE_PERIODIC, type, ch, interval/60, 50.0f, callbackfunc, nullptr);
    _timer.setup_overflow_irq();
    _timer.open();
    _timer.start();
  }

  if(isMatrixOn) {
    Serial.println("on");
    matrix.loadFrame(on);
  }else {
    Serial.println("off");
    matrix.loadFrame(off);
  }

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

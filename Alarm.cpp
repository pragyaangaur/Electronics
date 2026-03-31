#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int modeBtn = A0;
int incBtn  = A1;
int setBtn  = A2;

int buzzer = 8;

int hours = 0;
int minutes = 0;
int seconds = 0;

int alarmH = 0;
int alarmM = 1;

int mode = 0;

bool lastMode = HIGH;
bool lastInc  = HIGH;
bool lastSet  = HIGH;

unsigned long lastTick = 0;

void setup() {
  lcd.begin(16,2);

  pinMode(modeBtn, INPUT_PULLUP);
  pinMode(incBtn, INPUT_PULLUP);
  pinMode(setBtn, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
}

void loop() {

  unsigned long now = millis();

  if(now - lastTick >= 1000) {
    lastTick = now;
    seconds++;

    if(seconds >= 60){
      seconds = 0;
      minutes++;
    }

    if(minutes >= 60){
      minutes = 0;
      hours++;
    }

    if(hours >= 24){
      hours = 0;
    }
  }

  bool modeState = digitalRead(modeBtn);
  bool incState  = digitalRead(incBtn);
  bool setState  = digitalRead(setBtn);

  if(lastMode == HIGH && modeState == LOW){
    mode = (mode + 1) % 5;
  }

  if(lastInc == HIGH && incState == LOW){
    if(mode == 1) hours = (hours + 1) % 24;
    if(mode == 2) minutes = (minutes + 1) % 60;
    if(mode == 3) alarmH = (alarmH + 1) % 24;
    if(mode == 4) alarmM = (alarmM + 1) % 60;
  }

  if(lastSet == HIGH && setState == LOW){
    mode = 0;
  }

  lastMode = modeState;
  lastInc  = incState;
  lastSet  = setState;

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Time: ");
  printTime(hours, minutes, seconds);

  lcd.setCursor(0,1);
  lcd.print("Alarm: ");
  printTime(alarmH, alarmM, 0);

  if(hours == alarmH && minutes == alarmM && seconds == 0){
    tone(buzzer, 1000);
  } else {
    noTone(buzzer);
  }

  delay(100);
}

void printTime(int h, int m, int s){
  if(h < 10) lcd.print("0");
  lcd.print(h);
  lcd.print(":");

  if(m < 10) lcd.print("0");
  lcd.print(m);
  lcd.print(":");

  if(s < 10) lcd.print("0");
  lcd.print(s);
}

#include <ESP32Servo.h>

Servo myServo;

const int SERVO_PIN = 9;
const int POT_PIN = 1;
const int PIEZO_PIN = 21;
const int LED_PINS[] = {4, 5, 6, 7};

int angle = 0;
int direction = 1;
bool moveServo = false;
unsigned long lastMoveTime = 0;

String serialInput = "";
bool inputComplete = false;

struct Note {
  int freq;
  int duration;
};

Note theme[] = {
  {200, 300}, {400, 500}, {1000, 500},
  {420, 500}, {1200, 500}, {1000, 500},
  {800, 500}, {420, 500}
};

int noteIndex = 0;
bool playTheme = false;
unsigned long noteStart = 0;

void setup() {
  myServo.attach(SERVO_PIN);
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(PIEZO_PIN, OUTPUT);
}

void loop() {
  int brightness = map(analogRead(POT_PIN), 0, 1023, 0, 255);
  for (int i = 0; i < 4; i++) {
    analogWrite(LED_PINS[i], brightness);
  }

  if (inputComplete) {
    if (serialInput == "BATTLE") {
      playTheme = true;
      noteIndex = 0;
      noteStart = millis();
      moveServo = true;
      angle = 0;
      direction = 1;
      myServo.write(angle);
      lastMoveTime = millis();
    } else if (serialInput == "SERVO") {
      moveServo = true;
      angle = 0;
      direction = 1;
      myServo.write(angle);
      lastMoveTime = millis();
    } else {
      int inputAngle = serialInput.toInt();
      if (inputAngle >= 0 && inputAngle <= 180) {
        myServo.write(inputAngle);
      }
    }
    serialInput = "";
    inputComplete = false;
  }

  if (playTheme && noteIndex < 8) {
    if (millis() - noteStart >= theme[noteIndex].duration + 100) {
      tone(PIEZO_PIN, theme[noteIndex].freq, theme[noteIndex].duration);
      noteStart = millis();
      noteIndex++;
    }
  } else if (playTheme && noteIndex >= 8) {
    playTheme = false;
    noTone(PIEZO_PIN);
  }

  if (moveServo && millis() - lastMoveTime >= 15) {
    angle += direction;
    myServo.write(angle);
    lastMoveTime = millis();

    if (angle >= 180 || angle <= 0) {
      direction = -direction;
      if (angle <= 0) moveServo = false;
    }
  }
}

void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputComplete = true;
    } else {
      serialInput += c;
    }
  }
}

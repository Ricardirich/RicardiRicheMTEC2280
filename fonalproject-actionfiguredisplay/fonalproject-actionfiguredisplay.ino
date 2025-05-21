#include <ESP32Servo.h>  // Include the ESP32 Servo library for controlling servos

Servo myServo;  // Create a Servo object

// Define pin numbers
const int servoPin = 9;  // Servo is connected to pin 9
const int ledPins[] = {4, 5, 6, 7};  // LED pins connected to pins 4, 5, 6, 7
const int numLeds = 4;  // There are 4 LEDs
const int potPin = 1;  // Potentiometer connected to analog pin 1
const int piezoPin = 21;  // Piezo buzzer connected to pin 21

int angle = 0;  // Servo angle (0 to 180)
int dir = 1;  // Direction of servo sweep (1 means increase, -1 means decrease)
bool servoActive = false;  // Flag to check if the servo is active

unsigned long lastMoveTime = 0;  // Stores the time when the last servo move happened
const int stepDelay = 15;  // Delay between each servo move (in ms)

String inputString = "";  // To hold incoming data from the serial port
bool stringComplete = false;  // Flag to check if the input string is complete

// Define the structure for a musical note (frequency and duration)
struct Note {
  int freq;  // Frequency of the note (in Hz)
  int duration;  // Duration of the note (in milliseconds)
};

// Define the "Battle Theme" melody
Note battleTheme[] = {
  {200, 300}, {400, 500}, {1000, 500},
  {420, 500}, {1200, 500}, {1000, 500},
  {800, 500}, {420, 500}
};

// Calculate number of notes in the battle theme
const int numNotes = sizeof(battleTheme) / sizeof(battleTheme[0]);
int currentNote = 0;  // Tracks the current note in the melody
bool isPlayingBattle = false;  // Flag to check if the battle theme is being played
unsigned long noteStartTime = 0;  // Start time of the current note

void setup() {
  myServo.attach(servoPin);  // Attach the servo to the specified pin
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);  // Set the LED pins as output
  }
  pinMode(piezoPin, OUTPUT);  // Set the piezo buzzer pin as output
  Serial.begin(115200);  // Start serial communication at 115200 baud rate
  inputString.reserve(50);  // Reserve 50 bytes for the input string
}

void loop() {
  // LED brightness is controlled by the potentiometer
  int potValue = analogRead(potPin);  // Read the potentiometer value (0-1023)
  int brightness = map(potValue, 0, 1023, 0, 255);  // Map the value to 0-255 for brightness
  for (int i = 0; i < numLeds; i++) {
    analogWrite(ledPins[i], brightness);  // Set the LED brightness
  }

  // Handle serial input (commands like "BATTLE" or "SERVO")
  if (stringComplete) {
    inputString.trim();  // Remove any extra spaces or newline characters
    if (inputString == "BATTLE") {
      isPlayingBattle = true;  // Start playing the battle theme
      currentNote = 0;  // Reset the current note
      noteStartTime = millis();  // Store the current time
      servoActive = true;  // Activate the servo sweep
      angle = 0;  // Reset the servo angle
      dir = 1;  // Set the servo direction to increase angle
      myServo.write(angle);  // Set the servo to the initial angle
      lastMoveTime = millis();  // Store the current time for servo movement
    } else if (inputString == "SERVO") {
      // Reset servo to start sweep if "SERVO" command is received
      servoActive = true;
      angle = 0;
      dir = 1;
      myServo.write(angle);
      lastMoveTime = millis();
    } else {
      // If the string contains a valid servo angle, move the servo to that position
      int servoVal = inputString.toInt();  // Convert the string to an integer
      if (servoVal >= 0 && servoVal <= 180) {
        myServo.write(servoVal);  // Move the servo to the specified angle
      }
    }
    inputString = "";  // Clear the input string
    stringComplete = false;  // Reset the string complete flag
  }

  // Handle piezo buzzer melody (battle theme)
  if (isPlayingBattle && currentNote < numNotes) {
    if (millis() - noteStartTime >= battleTheme[currentNote].duration + 100) {
      // Play the current note
      if (battleTheme[currentNote].freq > 0) {
        tone(piezoPin, battleTheme[currentNote].freq, battleTheme[currentNote].duration);  // Play sound
      }
      noteStartTime = millis();  // Update the note start time
      currentNote++;  // Move to the next note
    }
  } else if (isPlayingBattle && currentNote >= numNotes) {
    // Stop playing the battle theme when all notes are played
    isPlayingBattle = false;
    noTone(piezoPin);  // Stop the sound
  }

  // Handle servo sweeping
  if (servoActive && millis() - lastMoveTime >= stepDelay) {
    angle += dir;  // Increment or decrement the angle based on the direction
    myServo.write(angle);  // Move the servo to the new angle
    lastMoveTime = millis();  // Update the last move time

    // Change direction when the servo reaches the limits
    if (angle >= 180 || angle <= 0) {
      dir = -dir;  // Reverse the direction
      if (angle <= 0) {
        servoActive = false;  // Stop the servo sweep when it reaches 0 degrees
      }
    }
  }
}

// This function is triggered when new data is received via serial
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();  // Read a single byte from the serial buffer
    if (inChar == '\n') {
      stringComplete = true;  // Set the flag when the end of the string is reached
    } else {
      inputString += inChar;  // Append the character to the input string
    }
  }
}

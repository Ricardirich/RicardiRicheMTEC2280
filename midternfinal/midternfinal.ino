const int buttonPin = 9;
const int ledPins[] = {4, 5, 6, 7}; // Array to hold the LED pins (4 LEDs)
const int numLeds = 4;               
const int potPin = 1;  
const int piezoPin = 21;             

bool buttonState = 0;     
bool lastButtonState = 0; 
bool toggle = 0;         
bool fallingToggle = 0;    

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  

  
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT); // Set all LED pins as output

  }

  Serial.begin(115200);  
}

void loop() {
  buttonState = !digitalRead(buttonPin); 

  
  if (buttonState == HIGH && lastButtonState == LOW) {
    fallingToggle = 1;  // Detect falling edge
  }


  if (fallingToggle) {
    toggle = !toggle;  
    fallingToggle = 0;  
  }

  
  int potValue = analogRead(potPin);

 
  int brightness = map(potValue, 0, 1023, 0, 255); // Map the potentiometer value to a brightness (0-255)

tone(piezoPin, 1000, 500);

  tone(piezoPin, 420, 500);
  
  tone(piezoPin, 1000, 500);
  
  tone(piezoPin, 420, 500);
  
  tone(piezoPin, 1000, 500);
  
  
  
  
  if (toggle) {
    for (int i = 0; i < numLeds; i++) {
      analogWrite(ledPins[i], brightness);  
    }
  } else {
    
    for (int i = 0; i < numLeds; i++) {
      analogWrite(ledPins[i], brightness);
      delay(200);
      analogWrite(ledPins[i], LOW);
      delay(200);
    }
  }
  
  lastButtonState = buttonState;

  // Print the current states to the Serial Monitor
  Serial.printf("Button = %i _ Toggle = %i _ Falling = %i _ Potentiometer = %i _ Brightness = %i\n", 
                buttonState, toggle, fallingToggle, potValue, brightness);

  delay(50);  // Small delay to debounce the button
  
}

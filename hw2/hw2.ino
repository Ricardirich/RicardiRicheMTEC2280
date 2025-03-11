const int buttonPin = 9;
const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 6;
const int ledPin4 = 7;

bool buttonState = 0;     
bool lastButtonState = 0; 
bool toggle = 0;         
bool fallingToggle = 0;    // Used to detect falling edge

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Configure button input with pullup
  pinMode(ledPin1, OUTPUT);          // Set LED pins as output
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  Serial.begin(115200);  // Start serial communication for debugging
}

void loop() {
  buttonState = !digitalRead(buttonPin);  // Read button state (active low)

  // Check for falling edge (button press)
  if (buttonState == HIGH && lastButtonState == LOW) {
    fallingToggle = 1;  // Detect falling edge
  }

  // When button is pressed, toggle patterns
  if (fallingToggle) {
    toggle = !toggle;  // Toggle between two patterns
    fallingToggle = 0;  // Reset the falling edge detection
  }

  // Pattern 1: All LEDs ON or Pattern 2: LEDs blink in sequence
  if (toggle) {
    // Pattern 1: All LEDs ON
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
  } else {
    // Pattern 2: LEDs blink in sequence
    digitalWrite(ledPin1, HIGH);
    delay(200);
    digitalWrite(ledPin1, LOW);

    digitalWrite(ledPin2, HIGH);
    delay(200);
    digitalWrite(ledPin2, LOW);

    digitalWrite(ledPin3, HIGH);
    delay(200);
    digitalWrite(ledPin3, LOW);

    digitalWrite(ledPin4, HIGH);
    delay(200);
    digitalWrite(ledPin4, LOW);
  }

  // Update the last button state
  lastButtonState = buttonState;

  // Print the current states to the Serial Monitor
  Serial.printf("Button = %i _ Toggle = %i _ Falling = %i\n", buttonState, toggle, fallingToggle);

  delay(50);  // Small delay to debounce the button
}

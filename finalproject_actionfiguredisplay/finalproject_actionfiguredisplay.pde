// Import the Serial library to communicate with Arduino
import processing.serial.*;

// Create a Serial object for communication
Serial myPort;

// Dimensions for the on-screen buttons
int buttonWidth = 150;
int buttonHeight = 50;

void setup() {
  size(500, 500);  // Set the canvas size
  textSize(20);    // Set text size for button labels
  textAlign(CENTER, CENTER);  // Center-align the text
  rectMode(CENTER);  // Draw rectangles from the center

  // Print available serial ports to the console
  printArray(Serial.list());

  // Choose the correct port from the list (change index if needed)
  String portName = Serial.list()[2];  // Change this index based on your system
  myPort = new Serial(this, portName, 115200);  // Open the port at 115200 baud rate
}

void draw() {
  backgroundGradient();  // Draw the gradient background

  // Draw the two interactive buttons
  drawButton(width / 2, 180, "Play Battle Theme");  // Top button
  drawButton(width / 2, 320, "Sweep Servo");        // Bottom button

  // Draw a red circle that follows the mouse
  fill(255, 0, 0);
  circle(mouseX, mouseY, 20);

  // Check if the mouse is currently hovering over either button
  boolean mouseOverButton = 
    isInside(mouseX, mouseY, width / 2, 180, buttonWidth, buttonHeight) ||
    isInside(mouseX, mouseY, width / 2, 320, buttonWidth, buttonHeight);

  // If the mouse is not on a button, send a mapped Y value as servo angle
  if (!mouseOverButton) {
    // Map mouseY (from 0 to height) to servo angle (0 to 180)
    int servoAngle = int(map(mouseY, 0, height, 0, 180));
    servoAngle = constrain(servoAngle, 0, 180);  // Limit angle between 0 and 180

    // Send the angle to the Arduino via Serial
    myPort.write(servoAngle + "\n");

    // Add a short delay to avoid overwhelming the serial buffer
    delay(15);
  }
}

// Draws the background as a vertical gradient
void backgroundGradient() {
  // Top half gradient (black to white)
  for (int i = 0; i < height / 2; i++) {
    int c = int(map(i, 0, height / 2, 0, 255));
    stroke(c);
    line(0, i, width, i);
  }

  // Bottom half gradient (white to black)
  for (int i = height; i > height / 2; i--) {
    int c = int(map(i, height / 2, height, 255, 0));
    stroke(c);
    line(0, i, width, i);
  }

  // Draw a gray line at the center
  stroke(127);
  line(0, height / 2, width, height / 2);
}

// Draws a button at a given location with a label
void drawButton(float x, float y, String label) {
  fill(64);           // Dark gray button color
  rect(x, y, buttonWidth, buttonHeight);  // Draw rectangle
  fill(255);          // White text color
  text(label, x, y);  // Draw button label
}

// Triggered when mouse is pressed
void mousePressed() {
  // If clicked on the "Battle Theme" button
  if (isInside(mouseX, mouseY, width / 2, 180, buttonWidth, buttonHeight)) {
    myPort.write("BATTLE\n");  
  }

  // If clicked on the "Sweep Servo" button
  if (isInside(mouseX, mouseY, width / 2, 320, buttonWidth, buttonHeight)) {
    myPort.write("SERVO\n");   
  }
}

// Helper function: checks if a point (mx, my) is inside a rectangle (bx, by, bw, bh)
boolean isInside(float mx, float my, float bx, float by, float bw, float bh) {
  return mx > bx - bw / 2 && mx < bx + bw / 2 && //mx is the mouse position x, bw is for the width, bh is fo the height 
         my > by - bh / 2 && my < by + bh / 2;
}

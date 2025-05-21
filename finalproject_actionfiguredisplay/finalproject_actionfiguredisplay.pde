import processing.serial.*;

Serial myPort;
int buttonWidth = 150;
int buttonHeight = 50;

void setup() {
  size(500, 500);
  textSize(20);
  textAlign(CENTER, CENTER);
  rectMode(CENTER);
  printArray(Serial.list());
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 115200);
}

void draw() {
  backgroundGradient();
  drawButton(width / 2, 180, "Play Battle Theme");
  drawButton(width / 2, 320, "Sweep Servo");
  fill(255, 0, 0);
  circle(mouseX, mouseY, 20);

  boolean mouseOverButton = 
    isInside(mouseX, mouseY, width / 2, 180, buttonWidth, buttonHeight) ||
    isInside(mouseX, mouseY, width / 2, 320, buttonWidth, buttonHeight);

  if (!mouseOverButton) {
    int servoAngle = int(map(mouseY, 0, height, 0, 180));
    servoAngle = constrain(servoAngle, 0, 180);
    myPort.write(servoAngle + "\n");
    delay(15);
  }
}

void backgroundGradient() {
  for (int i = 0; i < height / 2; i++) {
    int c = int(map(i, 0, height / 2, 0, 255));
    stroke(c);
    line(0, i, width, i);
  }

  for (int i = height; i > height / 2; i--) {
    int c = int(map(i, height / 2, height, 255, 0));
    stroke(c);
    line(0, i, width, i);
  }

  stroke(127);
  line(0, height / 2, width, height / 2);
}

void drawButton(float x, float y, String label) {
  fill(64);
  rect(x, y, buttonWidth, buttonHeight);
  fill(255);
  text(label, x, y);
}

void mousePressed() {
  if (isInside(mouseX, mouseY, width / 2, 180, buttonWidth, buttonHeight)) {
    myPort.write("BATTLE\n");  
  }

  if (isInside(mouseX, mouseY, width / 2, 320, buttonWidth, buttonHeight)) {
    myPort.write("SERVO\n");   
  }
}

boolean isInside(float mx, float my, float bx, float by, float bw, float bh) {
  return mx > bx - bw / 2 && mx < bx + bw / 2 &&
         my > by - bh / 2 && my < by + bh / 2;
}

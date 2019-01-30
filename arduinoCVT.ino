// Servo - Version: Latest 
#include <Servo.h>

Servo pan;
Servo tilt;
String inputString;
int ledState = LOW;
int buttonState;
int lastButtonState = LOW;
const int buttonPin = 7;
const int ledPin = 8;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pan.attach(9);
  tilt.attach(10);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, ledState);
  
  //init to 90
  pan.write(90);
  tilt.write(90);
  
  //serial stuff
  Serial.begin(115200);
  Serial.setTimeout(3);
  inputString.reserve(200);
}

void serialEvent() {
  inputString = Serial.readString();
  
  pan.write(parseDataX(inputString));
  tilt.write(parseDataY(inputString));
}

int parseDataX(String data){
  data.remove(data.indexOf(":"));
  data.remove(data.indexOf("X"), 1);

  return data.toInt();
}

int parseDataY(String data){
  data.remove(0,data.indexOf(":") + 1);
  data.remove(data.indexOf("Y"), 1);

  return data.toInt();
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
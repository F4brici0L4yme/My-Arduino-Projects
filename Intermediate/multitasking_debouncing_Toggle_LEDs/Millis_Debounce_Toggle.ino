// This reminds me those christmas lights and the button they have to toggle their states

#define BUTTON_PIN 2
#define LED10_PIN 10
#define LED11_PIN 11
#define LED12_PIN 12

unsigned long lastTimeButtonStateChanged = millis();
unsigned long debounceDelay = 50;
  
unsigned long lastTimeLED10Blinked = millis();
unsigned long blinkDelay = 1000;

byte stateLED10 = LOW;
byte buttonState = LOW;

int toggleLEDState = 1;

//LED10 blinking
void blinkLED10() {
  if(stateLED10 == HIGH) {
    stateLED10 = LOW;
  }
  else {
  	stateLED10 = HIGH;
  }
  digitalWrite(LED10_PIN, stateLED10);
}

//Toggle states of LEDs 11 & 12
void toggleOtherLEDs() {
  if (toggleLEDState == 1) {
    toggleLEDState = 2;
    digitalWrite(LED11_PIN, LOW);
    digitalWrite(LED12_PIN, HIGH);
  }
  else {
    toggleLEDState = 1;
    digitalWrite(LED11_PIN, HIGH);
    digitalWrite(LED12_PIN, LOW);
  }
}


void setup(){
  pinMode(LED10_PIN, OUTPUT);
  pinMode(LED11_PIN, OUTPUT);
  pinMode(LED12_PIN, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT);
  buttonState = digitalRead(BUTTON_PIN);
}


void loop(){
  unsigned long currentTime = millis();
  
  if(currentTime - lastTimeLED10Blinked > blinkDelay) { //delay the blinking
  	lastTimeLED10Blinked += blinkDelay;
    blinkLED10();
  }
  
  
  if(currentTime - lastTimeButtonStateChanged > debounceDelay) { //debouncing
    byte newButtonState = digitalRead(BUTTON_PIN);
    if(buttonState != newButtonState){
      	lastTimeButtonStateChanged = currentTime;
    	buttonState = newButtonState; //toggle state
      if(buttonState == HIGH)
        toggleOtherLEDs(); //toggle LEDs 11 & 12 while LED 10 is blinking
    }
  }
}

// C++ code
//Interruptions

#define LED_PIN 12
#define BUTTON_PIN 2// only digital pins 2 & 3 can manage interruptions

byte LEDState = LOW;
volatile boolean buttonReleased = false;

void toggleLED() {
	if(LEDState == HIGH)
      LEDState = LOW;
  	else
      LEDState = HIGH;
  	digitalWrite(LED_PIN, LEDState);

}

void buttonReleasedInterrupt(){
	buttonReleased = true;
}

void setup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), 
                  buttonReleasedInterrupt, 
                  FALLING);
}

void loop(){
  if(buttonReleased){
  	buttonReleased = false;
    toggleLED();
  }
}
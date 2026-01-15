#define BUTTON_PIN 2
unsigned long lastTimeButtonStateChanged = millis();
unsigned long debounceDelay = 50;
byte buttonState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

  if (currentTime - lastTimeButtonStateChanged > debounceDelay) { //check time before accepting other input 
    byte newButtonState = digitalRead(BUTTON_PIN);
    if(buttonState != newButtonState) { //check match of state
      lastTimeButtonStateChanged = currentTime; //with this value updated we will not read those bounces
      buttonState = newButtonState;
      if(buttonState == HIGH) {
        Serial.println("Button pressed");
      }
      else
        Serial.println("Button released");
    }
  }
  

}

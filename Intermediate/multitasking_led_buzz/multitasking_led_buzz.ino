// This algorithm makes a buzzer buzz whenever the LED has a HIGH value. 
unsigned long prevTimeBlink = 0;
unsigned long interval = 1000;

#define LED_PIN 11
#define BUZZER_PIN 10 //buzzer

int ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (millis() - prevTimeBlink >= interval) { //checking time
    prevTimeBlink = millis();
    
    ledState = !ledState; //change state
    
    digitalWrite(LED_PIN, ledState);
    digitalWrite(BUZZER_PIN, ledState);
    Serial.println(prevTimeBlink); // cheking for additional ms (so far so good)
  }
}
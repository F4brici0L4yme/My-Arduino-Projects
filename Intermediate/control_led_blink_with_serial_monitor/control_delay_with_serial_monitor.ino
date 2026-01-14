// YT Video: https://www.youtube.com/watch?v=lAkvQA-d3HM

#define LED_PIN 11
// This algorithm give you the power to control the delay value between a HIGH and LOW value for a LED (between 10ms & 1000ms).
int delay_value = 1000;

void setup() {
  //matching the Serial Monitor's bauds value
  Serial.begin(115200);
  pinMode(11, OUTPUT);
  Serial.setTimeout(50);
}

void loop() {
  if(Serial.available() > 0) { // checking if there was an input
    int input_value = Serial.parseInt();
    
    if(input_value > 10 && input_value < 1000){
      delay_value = input_value; // assigning the value
      Serial.println(delay_value);
    }
  }

  digitalWrite(LED_PIN, HIGH);
  delay(delay_value);
  digitalWrite(LED_PIN, LOW);
  delay(delay_value);
}
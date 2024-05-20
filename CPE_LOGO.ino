int Led_1 = 2;
int Led_2 = 3;
int Led_3 = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for(int LED = 2; LED <=4; LED++){
    pinMode(LED, OUTPUT)
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Led_1, HIGH);
  digitalWrite(Led_2, HIGH);
  digitalWrite(Led_3, HIGH);
}

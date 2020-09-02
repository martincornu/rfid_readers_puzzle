#define PIN_RF_1  2
#define PIN_RF_2  3
#define PIN_RF_3  4
#define PIN_RF_4  5
#define PIN_RF_5  6

#define OUTPUT_PIN  7

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_RF_1, INPUT);
  pinMode(PIN_RF_2, INPUT);
  pinMode(PIN_RF_3, INPUT);
  pinMode(PIN_RF_4, INPUT);
  pinMode(PIN_RF_5, INPUT);

  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((digitalRead(PIN_RF_1) == 1) && (digitalRead(PIN_RF_2) == 1) && (digitalRead(PIN_RF_3) == 1) && (digitalRead(PIN_RF_4) == 1) && (digitalRead(PIN_RF_5) == 1))
  {
    Serial.println("ACTIVE ELECTRO AIMANT");
    digitalWrite(OUTPUT_PIN, HIGH);  
  }
  else
  {
    digitalWrite(OUTPUT_PIN, LOW);  
  }
}

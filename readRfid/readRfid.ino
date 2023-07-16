//#define DEBUG

#define PIN_RF_1  2
//#define PIN_RF_2  3 // rfid sensor removed from system because this one is damaged.
#define PIN_RF_3  4
#define PIN_RF_4  5
#define PIN_RF_5  6

#define OUTPUT_PIN  7

#define BTN_EMERGENCY_PIN  8

#define DELAY_LOOP_MS       200

uint8_t g_u8Emergency = 0u;             // Flag to indicate an emergency fired

int g_s32ButtonState;             // the current reading from the input pin
int g_s32LastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long g_u64LastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long g_u64DebounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_RF_1, INPUT);
  //pinMode(PIN_RF_2, INPUT);
  pinMode(PIN_RF_3, INPUT);
  pinMode(PIN_RF_4, INPUT);
  pinMode(PIN_RF_5, INPUT);

  pinMode(BTN_EMERGENCY_PIN, INPUT);

  pinMode(LED_BUILTIN, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);

#ifdef DEBUG
  Serial.begin(115200);
#endif
}

void loop() {  
  // Btn emergency management
  BtnEmergencyManagement();
  
  // put your main code here, to run repeatedly:
  if (  (1u == g_u8Emergency)
      || (  (digitalRead(PIN_RF_1) == 1) /*&& (digitalRead(PIN_RF_2) == 1)*/
         && (digitalRead(PIN_RF_3) == 1)
         && (digitalRead(PIN_RF_4) == 1)
         && (digitalRead(PIN_RF_5) == 1)) )
  {
  #ifdef DEBUG
    if (1u == g_u8Emergency)
    {
      Serial.println("Emergency fired ! Stop program like a success.");
    }
    else
    {
      Serial.println("Success !");
    }
  #endif
    digitalWrite(OUTPUT_PIN, HIGH);  
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(OUTPUT_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);  
  }

  delay(DELAY_LOOP_MS);  
}

void BtnEmergencyManagement(void) {
   int l_s32Reading = 0;                 // btn emergency reading
  
  // read the state of the switch into a local variable:
  l_s32Reading = digitalRead(BTN_EMERGENCY_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (l_s32Reading != g_s32LastButtonState) {
    // reset the debouncing timer
    g_u64LastDebounceTime = millis();
  }

  if ((millis() - g_u64LastDebounceTime) > g_u64DebounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (l_s32Reading != g_s32ButtonState) {
      g_s32ButtonState = l_s32Reading;

      // button state is high
      if (g_s32ButtonState == HIGH) {
        g_u8Emergency = 1u;
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the g_s32LastButtonState:
  g_s32LastButtonState = l_s32Reading;
}

#include <BleGamepad.h>
#include <FastLED.h>

CRGB leds[6];
BleGamepad bleGamepad("ESP Controller", "ElectroPoint4u", 100);

// TRIGGER BUTTONS
#define LT 2
#define RT 23

// LEFT-JOYSTICK
#define LH 34
#define LV 35
#define LS 15

// RIGHT-JOYSTICK
#define RH 39
#define RV 36
#define RS 19

// D-PAD
#define L1 32
#define L2 33
#define L3 27
#define L4 14

#define R1 18
#define R2 5
#define R3 17
#define R4 16

// BATTERY VOLTAGE
#define ADC 4

// NEOPIXEL
#define DATA_PIN 13

// PERIFERALS
//  #define MIC 25
//  #define SPEAKER 26

int buttons[8] = {32, 33, 27, 14, 18, 5, 17, 16};

int period = 1000;
unsigned long time_now = 0;

const int numberOfPotSamples = 5;     // Number of pot samples to take (to smooth the values)
const int delayBetweenSamples = 2;    // Delay in milliseconds between pot samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports
const int debounceDelay = 10;        // Delay in milliseconds between button press

int previousButton1State = HIGH;
int previousButton2State = HIGH;
int previousButton3State = HIGH;
int previousButton4State = HIGH;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, 6);
  FastLED.setBrightness(64);
  for (int i = 0; i < 8; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
  pinMode(LT, INPUT_PULLUP);
  pinMode(RT, INPUT_PULLUP);
  pinMode(LS, INPUT_PULLUP);
  pinMode(RS, INPUT_PULLUP);
  bleGamepad.begin();
  Serial.println("Starting BLE work!");

  leds[0] = CRGB::Blue;
  leds[5] = CRGB::Blue;
  FastLED.show();
  delay(200);
  leds[1] = CRGB::Blue;
  leds[4] = CRGB::Blue;
  FastLED.show();
  delay(300);
  leds[2] = CRGB::Blue;
  leds[3] = CRGB::Blue;
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  if (bleGamepad.isConnected()) {
    for (int i = 0; i < 6; i++) {
      leds[i] = CHSV( HUE_GREEN, 255, 64);
    }
    FastLED.show();
    while (millis() > time_now + period) {
      Serial.println("Checking Battery Level");
      batteryLevel();
      time_now = millis();
    }
    int currentButton1State = digitalRead(R1);
    int currentButton2State = digitalRead(R2);
    int currentButton3State = digitalRead(R3);
    int currentButton4State = digitalRead(R4);

    if (currentButton1State != previousButton1State) {
      if (currentButton1State == LOW)
        bleGamepad.press(BUTTON_5);
      else
        bleGamepad.release(BUTTON_5);
    }
    previousButton1State = currentButton1State;

    if (currentButton2State != previousButton2State) {
      if (currentButton2State == LOW)
        bleGamepad.press(BUTTON_2);
      else
        bleGamepad.release(BUTTON_2);
    }
    previousButton2State = currentButton2State;

    if (currentButton3State != previousButton3State) {
      if (currentButton3State == LOW)
        bleGamepad.press(BUTTON_1);
      else
        bleGamepad.release(BUTTON_1);
    }
    previousButton3State = currentButton3State;

    if (currentButton4State != previousButton4State) {
      if (currentButton4State == LOW)
        bleGamepad.press(BUTTON_4);
      else
        bleGamepad.release(BUTTON_4);
    }
    previousButton4State = currentButton4State;

    if (digitalRead(LS) == LOW) {
      Serial.println("Left Joystick");
      bleGamepad.press(BUTTON_6);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_6);
    }
    if (digitalRead(RS) == LOW) {
      Serial.println("Right Joystick");
      bleGamepad.press(BUTTON_3);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_3);
    }

    if (digitalRead(LT) == LOW) {
      Serial.println("Left trigger");
      bleGamepad.press(BUTTON_7);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_7);
    }
    if (digitalRead(RT) == LOW) {
      Serial.println("Right trigger");
      bleGamepad.press(BUTTON_8);
      delay(debounceDelay);
      bleGamepad.release(BUTTON_8);
    }

    int potValues[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues[i] = analogRead(LH);
      delay(delayBetweenSamples);
    }
    int potValue = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue += potValues[i];
    }
    potValue = potValue / numberOfPotSamples;
    int adjustedValue = map(potValue, 0, 4095, 127, -127);


    int potValues2[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues2[i] = analogRead(LV);
      delay(delayBetweenSamples);
    }
    int potValue2 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue2 += potValues2[i];
    }
    potValue2 = potValue2 / numberOfPotSamples;
    int adjustedValue2 = map(potValue2, 0, 4095, 127, -127);


    int potValues3[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues3[i] = analogRead(RH);
      delay(delayBetweenSamples);
    }
    int potValue3 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue3 += potValues3[i];
    }
    potValue3 = potValue3 / numberOfPotSamples;
    int adjustedValue3 = map(potValue3, 0, 4095, 127, -127);


    int potValues4[numberOfPotSamples];
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValues4[i] = analogRead(RV);
      delay(delayBetweenSamples);
    }
    int potValue4 = 0;
    for (int i = 0 ; i < numberOfPotSamples ; i++) {
      potValue4 += potValues4[i];
    }
    potValue4 = potValue4 / numberOfPotSamples;
    int adjustedValue4 = map(potValue4, 0, 4095, 127, -127);
    /*
      Serial.print(adjustedValue);
      Serial.print(" || ");
      Serial.print(adjustedValue2);
      Serial.print(" || ");
      Serial.print(adjustedValue3);
      Serial.print(" || ");
      Serial.println(adjustedValue4);
    */
    bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_CENTERED);
    delay(delayBetweenHIDReports);

    if (digitalRead(L1) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_UP);
    if (digitalRead(L2) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_LEFT);
    if (digitalRead(L3) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_DOWN);
    if (digitalRead(L4) == LOW)
      bleGamepad.setAxes(adjustedValue, adjustedValue2, 0, 0, adjustedValue3, adjustedValue4, DPAD_RIGHT);
  }
}

void batteryLevel() {
  int sensorValue = analogRead(ADC);
  float voltage = sensorValue * (5.12 / 4095.0);
  Serial.print(voltage);
  Serial.print("V ||");
  int percentage = (voltage / 4.2) * 100;
  Serial.print(percentage);
  Serial.println("%");
  bleGamepad.setBatteryLevel(percentage);
  if (percentage < 60) {
    Serial.println("LOW battery");
    for (uint8_t i = 30; i < 220; i++) {
      for (int j = 0; j < 6; j++) {
        leds[j] = CHSV( HUE_RED, 255, i);
      }
      FastLED.show();
      delay(15);
    }
    for (uint8_t i = 220; i > 30; i--) {
      for (int j = 0; j < 6; j++) {
        leds[j] = CHSV( HUE_RED, 255, i);
      }
      FastLED.show();
      delay(15);
    }
  }
}

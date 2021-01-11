#include "FastLED.h"
#define BRIGHTNESS 25

const int NUM_SLIDERS = 6;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4, A5};
const int NUM_BUTTONS = 6;
const int DigitalButtons[NUM_BUTTONS]= {0, 1, 2, 3, 4, 5};
int DigitalButtonsValues[NUM_BUTTONS];

//bool input_state= LOW; // input state high/low 
//bool previous_state[6]= {LOW}; //  previeus input state 
//int status=1;

int mute [6] = {0, 0, 0, 0, 0, 0};
int color[6] = {0, 0, 0, 0, 0, 0};

int analogSliderValues[NUM_SLIDERS];
CRGB leds[6];
void setup() { 
  FastLED.setBrightness(BRIGHTNESS );
  FastLED.addLeds<NEOPIXEL, 6>(leds, 6);
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    pinMode(DigitalButtons[i], INPUT_PULLUP);    
  }
  Serial.begin(9600);
}

void loop() {
  Mute();
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  delay(300);
  // printSliderValues(); // For debug
  
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);


      if(mute[i] == 1){
         leds[5-i] = CRGB::Red;
         analogSliderValues[i] = 1023;
      }
      else{
        if(analogSliderValues[i] >= 1020){
          leds[5-i] = CRGB::Green;
        }
        else{
          leds[5-i] = CRGB::Black;
        }
      }
  }

       
     FastLED.show();
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void Mute(){
  for(int i=0; i<6; i++){
    DigitalButtonsValues[5-i] = digitalRead(DigitalButtons[5-i]);
    if(DigitalButtonsValues[5-i] == 0){
      if(mute[i] == 0){
       mute[i]=1; 
      }else mute[i] = 0;
    }
  }
}

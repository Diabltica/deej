#include "FastLED.h"

const int NUM_SLIDERS = 6;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4, A5};

const int NUM_MUTE_BUTTONS = 6; // numbers of mute button
const int MuteInput [NUM_MUTE_BUTTONS] = {0, 1, 2, 3, 4, 5}; // Mute Buttons Pins

const int Data_LED = 6; //Led Data pin 
const int NUM_LED = 6; // Number of Led

int analogSliderValues[NUM_SLIDERS];
bool IsMute [NUM_MUTE_BUTTONS];

int Memory[6]={0,0,0,0,0,0};

CRGB leds[NUM_LED];


void setup() { 
  FastLED.addLeds<NEOPIXEL, Data_LED>(leds, NUM_LED);
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    pinMode(MuteInput[i],INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  MuteSlider();
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    if(IsMute[i]=true){
      analogSliderValues[i]=0;
      }else{
        analogSliderValues[i] = analogRead(analogInputs[i]);
        }
    if (analogSliderValues[i] == 0 && IsMute[i]==true){
      ControlLed(i,1);
      }else if (analogSliderValues[i]==0 && IsMute[i]==false){
        ControlLed(i,2);
      }
  }
}

void sendSliderValues() {
  String builtString = String("");
  if(TrackChanges() == 0){
    for (int i = 0; i < NUM_SLIDERS; i++) {
      builtString += String((int)analogSliderValues[i]);
  
      if (i < NUM_SLIDERS - 1) {
        builtString += String("|");
      }
    }
    
    Serial.println(builtString);
  }
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
void MuteSlider(){
    for(int i = 0; i<NUM_MUTE_BUTTONS; i++){
      if(digitalRead[MuteInput[i]]==LOW){
        IsMute[i]=true;
        }else{
          IsMute[i]=false;
          }
    }
}
void ControlLed(int NLed, int C){
  //The variable NLed define which led is control
  //the variable C controle the color of the Led.
  //1 for Red
  //2 for Green
  switch(C){
    case 1:leds[NLed] = CRGB::Red; FastLED.show(); delay(30);
    case 2:leds[NLed] = CRGB::Green; FastLED.show(); delay(30);
  }
}

int TrackChanges(){
  for(int i = 0; i<NUM_SLIDERS; i++){
    if(analogSliderValues[i] == Memory[i]){
      return 0;
    }else if(analogSliderValues[i] != Memory[i]){
     Memory[i] = analogSliderValues[i]; 
     }
  }
  return 1;
}

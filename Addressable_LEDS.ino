// Much help from a myriad of tutorials online:  
// - SparkFun, FastLED Documentation(https://github.com/FastLED/FastLED/wiki/Basic-usage), @RupakPoddar on Github and Youtube,
// various Arduino forums, and Justin Gitlin. Thanks all! /J 

#include <FastLED.h>
#define NUMLEDS 60
#define DATA_PIN 7                // Connect W2818 control wire to this pin
#define MIC_PIN A0           // Connect sound detector to this pin

#define SENSITIVITY 600           // 0 to 1023
#define MAX_BRIGHT 200        // Ranges from 0 to 255
#define SATURATION 200            // 0 to 255
#define MINVAL 10
#define HUE_INITIAL 255
#define HUE_CHANGE 3. //Set this to determine the spread in color for linearcrawl

CRGB leds[NUMLEDS]; // set CRGB

byte brightness[NUMLEDS];
byte hue[NUMLEDS];
int analogVal;
int DELAY;

void setup() {
  Serial.begin(9600); //Start serioal connection
  pinMode(MIC_PIN, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUMLEDS);

  //Update the LED strip
  FastLED.show(); 

}

// Part that continually loops
void loop() {
  analogVal = analogRead(MIC_PIN);

//checking for analog data and assigning if higher than sensitivity setting above, user can set the sensitivity above
  if(analogVal > SENSITIVITY){
  analogVal = SENSITIVITY;
  }
  
  if(analogVal < MINVAL){
  analogVal = 0;
  }


// Function to call - uncomment the one you're like to push to the Arduino
  //  BrightnessReactive();
    LinearCrawl();
  // CenterBeatDetection();
   // EdgeProgress();
  FastLED.show();
  

}

void CenterBeatDetection(){
  byte val = map(analogVal, 0, SENSITIVITY, 0, NUMLEDS/2);
  byte hue = HUE_INITIAL;
  for(int i = 0; i <= val; i++){
    leds[(NUMLEDS/2)+i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHT);
    leds[(NUMLEDS/2)-i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHT);
  }
    for(int i = val+1; i <= (NUMLEDS/2); i++){
    leds[(NUMLEDS/2)+i].nscale8(2);
    leds[(NUMLEDS/2)-i].nscale8(2);
  }
}

void BrightnessReactive(){
  byte val = map(analogVal, 0, SENSITIVITY+1, 0, MAX_BRIGHT);
  byte hue = HUE_INITIAL;
  for(int i = 0; i < NUMLEDS; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, val);
  }
}

void LinearCrawl(){
  byte val = map(analogVal, 0, SENSITIVITY+1, 0, MAX_BRIGHT);
  DELAY = map(analogVal, 0, SENSITIVITY+1, 20, 1);
  
  for(int i = 0; i < NUMLEDS; i++){
    brightness[NUMLEDS-i] = brightness[NUMLEDS-1-i];
  }
  
  for(int i = 0; i < NUMLEDS; i++){
    hue[NUMLEDS-i] = hue[NUMLEDS-i-1];
  }
  
  brightness[0] = val;
  byte hue = HUE_INITIAL;
  for(int i = 0; i < NUMLEDS; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, brightness[i]);
  }
  delay(DELAY);
}

void EdgeProgress(){
  byte val = map(analogVal, 0, SENSITIVITY, 0, NUMLEDS/2);
  byte hue = HUE_INITIAL;
  for(int i = 0; i <= val; i++){
    leds[i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHT);
    leds[NUMLEDS-i] = CHSV(hue += HUE_CHANGE, SATURATION, MAX_BRIGHT);
  }

  for(int i=val+1; i<=(NUMLEDS/2); i++){
    leds[i].nscale8(10);
    leds[NUMLEDS-i].nscale8(10);
  }
}

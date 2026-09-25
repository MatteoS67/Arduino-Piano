
#include "Melody.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h>

// --- OLED SETUP ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- PIANO SETUP ---
int buttonPins[] = {13, 12, 11, 10, 9, 8, 7, 6};
String pianoNotes[] = {"do", "re", "mi", "fa", "sol", "la", "si", "do"}; 
int frequencies[] = {523, 587, 659, 698, 784, 880, 988, 1047};
int buzzerPin = 5;
int MelodyButton = 4;



void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < 8; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(MelodyButton, INPUT_PULLUP);
  
  // Initialize the screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  // Initial Screen Text
  display.clearDisplay();
  display.setTextSize(2);      
  display.setTextColor(WHITE); 
  display.setCursor(0, 0);     
  display.println("Piano");
  display.println("Ready!");
  display.display();           
}

void loop() {
  
  // --- 1. PLAY HARRY POTTER WHEN PIN 4 IS PRESSED ---
  if(digitalRead(MelodyButton) == LOW) {
    
    // Update Screen for Song
    display.clearDisplay();
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.println("Harry");
    display.println("Potter");
    display.display();
    
    //Unsigned Long (UL) to prevent memory overflow
      int notesCount = sizeof(melody) / sizeof(melody[0]) / 2;
      unsigned long wholenote = (60000UL * 4) / tempo;
      
    // Loop through PROGMEM array
    for (int thisNote = 0; thisNote < notesCount * 2; thisNote = thisNote + 2) {
      
      // Cast the data directly to (int) to restore negative numbers
      int divider = (int)pgm_read_word(&melody[thisNote + 1]);
      int noteDuration = 0;
      
      if (divider > 0) {
        noteDuration = wholenote / divider;
      } else if (divider < 0) {
        noteDuration = wholenote / abs(divider);
        noteDuration *= 1.5; 
      }
      
      int pitch = (int)pgm_read_word(&melody[thisNote]);
      
      //Only trigger the buzzer if the note is NOT a rest (0 Hz)
      if (pitch > 0) {
        tone(buzzerPin, pitch, noteDuration * 0.9);
      }
      
      delay(noteDuration);
      noTone(buzzerPin);
    }
    
    // Wait until you let go of the button
    while(digitalRead(MelodyButton) == LOW){
      delay(10);
    }

    // Reset screen when song finishes
    display.clearDisplay();
    display.setCursor(0, 0);     
    display.println("Piano");
    display.println("Ready!");
    display.display(); 
  }

  // --- 2. PLAY NORMAL PIANO KEYS ---
  for(int i = 0; i < 8; i++){
    if(digitalRead(buttonPins[i]) == LOW){
      
      // Update screen with note name
      display.clearDisplay();
      display.setCursor(0, 16);    
      display.setTextSize(3);      
      display.println(pianoNotes[i]);
      display.display();
      
      tone(buzzerPin, frequencies[i]);

      while(digitalRead(buttonPins[i]) == LOW){
        delay(10);
      }
      noTone(buzzerPin);
      
      // Reset Screen when you let go of the key
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);     
      display.println("Piano");
      display.println("Ready!");
      display.display(); 
    }
  }
} 
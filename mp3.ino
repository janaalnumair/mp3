#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions
#define PIN_RX 2
#define PIN_TX 3
#define BTN_PLAY 4
#define BTN_NEXT 5
#define BTN_PREV 6

SoftwareSerial mySoftwareSerial(PIN_RX, PIN_TX);
DFRobotDFPlayerMini myDFPlayer;

// =========================================================================
// AUTOMATED PLAYLIST SECTION (Stored in Flash Memory safely to save RAM)
// =========================================================================
// Simply type your song names inside the curly braces separated by commas.
// The code automatically counts them and reads them from Flash (PROGMEM).
const char songNames[][30] PROGMEM = {
  "Affection",   // Track 1 -> Folder 01/001.mp3
  "Bubblegum",    // Track 2 -> Folder 01/002.mp3
  "Don't Let Go",         // Track 3 -> Folder 01/003.mp3
  "Heavenly",       // Track 4 -> Folder 01/004.mp3
  "K."   // Track 5 -> Folder 01/005.mp3
};
const int totalConfiguredSongs = sizeof(songNames) / sizeof(songNames[0]);

// State Variables
bool isPlaying = false;
int currentTrack = 1;
int currentVolume = 12; 
int totalTracks = totalConfiguredSongs;

// Button Timing and Cooldown Variables
const unsigned long LONG_PRESS_TIME = 500;   
const unsigned long COOLDOWN_TIME = 800;     // Increased to 800ms to give slow clones time to recover
unsigned long lastCommandTime = 0;          

unsigned long nextBtnPressedTime = 0;
unsigned long prevBtnPressedTime = 0;
bool nextIsPressed = false;
bool prevIsPressed = false;
bool nextActionTriggered = false;
bool prevActionTriggered = false;

void updateScreen(String statusText) {
  display.clearDisplay();
  
  // Status Bar
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(statusText);
  display.setCursor(85, 0);
  display.print("Vol:");
  display.print(currentVolume);
  
  display.drawFastHLine(0, 10, 128, SSD1306_WHITE);
  
  // Track Title
  display.setCursor(0, 16);
  display.setTextSize(1); 
  
  int listIndex = currentTrack - 1; 
  if (listIndex >= 0 && listIndex < totalConfiguredSongs) {
    char buffer[32]; 
    strcpy_P(buffer, songNames[listIndex]);
    display.print(buffer);
  } else {
    display.print("Track ");
    display.print(currentTrack);
  }
  
  display.display();
}

// Custom Function to ensure Clone Chips actually fire up and play
void forcePlayTrack(int trackNum) {
  myDFPlayer.pause();            // Step 1: Force stop anything ongoing
  delay(100);                    // Step 2: Give hardware buffer time to empty
  myDFPlayer.play(trackNum);     // Step 3: Command the index directly
  delay(100);                    // Step 4: Let the instruction pack land safely
  myDFPlayer.start();            // Step 5: Double-tap reinforcement play command
}

void setup() {
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);

  mySoftwareSerial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for(;;);
  }
  
  display.clearDisplay();
  display.setCursor(0,10);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print("Loading...");
  display.display();

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    display.clearDisplay();
    display.setCursor(0,10);
    display.print("DFPlayer Error!");
    display.display();
    while(true);
  }
  
  myDFPlayer.volume(currentVolume);
  delay(300); 
  totalTracks = myDFPlayer.readFileCounts();
  if (totalTracks <= 0) totalTracks = totalConfiguredSongs; 
  
  updateScreen("Paused");
}

void loop() {
  unsigned long currentMillis = millis();

  // ==========================================
  // 1. PLAY / PAUSE BUTTON
  // ==========================================
  if (digitalRead(BTN_PLAY) == LOW) {
    delay(50); 
    if (digitalRead(BTN_PLAY) == LOW) {
      if (isPlaying) {
        myDFPlayer.pause();
        isPlaying = false;
        updateScreen("Paused");
      } else {
        // FIXED: Instead of standard start, we force a complete index fire 
        // to catch the module if it got dropped or stuck during skip!
        forcePlayTrack(currentTrack); 
        isPlaying = true;
        updateScreen("Playing");
      }
      while(digitalRead(BTN_PLAY) == LOW); 
    }
  }

  // ==========================================
  // 2. NEXT / VOLUME UP BUTTON
  // ==========================================
  if (digitalRead(BTN_NEXT) == LOW) {
    if (!nextIsPressed) {
      nextIsPressed = true;
      nextBtnPressedTime = currentMillis;
      nextActionTriggered = false;
      delay(50); 
    }
    
    if (currentMillis - nextBtnPressedTime > LONG_PRESS_TIME) {
      if (currentVolume < 30) {
        currentVolume++;
        myDFPlayer.volume(currentVolume);
        updateScreen(isPlaying ? "Playing" : "Paused");
      }
      nextActionTriggered = true; 
      delay(150); 
    }
  } else {
    if (nextIsPressed) {
      if (!nextActionTriggered && (currentMillis - nextBtnPressedTime < LONG_PRESS_TIME)) {
        if (currentMillis - lastCommandTime > COOLDOWN_TIME) {
          currentTrack++;
          if (totalTracks > 0 && currentTrack > totalTracks) currentTrack = 1;
          
          // FIXED: Uses the new sequential robust timing play execution
          forcePlayTrack(currentTrack); 
          isPlaying = true;
          lastCommandTime = currentMillis; 
          updateScreen("Playing");
        }
      }
      nextIsPressed = false;
    }
  }

  // ==========================================
  // 3. PREVIOUS / VOLUME DOWN BUTTON
  // ==========================================
  if (digitalRead(BTN_PREV) == LOW) {
    if (!prevIsPressed) {
      prevIsPressed = true;
      prevBtnPressedTime = currentMillis;
      prevActionTriggered = false;
      delay(50); 
    }
    
    if (currentMillis - prevBtnPressedTime > LONG_PRESS_TIME) {
      if (currentVolume > 0) {
        currentVolume--;
        myDFPlayer.volume(currentVolume);
        updateScreen(isPlaying ? "Playing" : "Paused");
      }
      prevActionTriggered = true; 
      delay(150); 
    }
  } else {
    if (prevIsPressed) {
      if (!prevActionTriggered && (currentMillis - prevBtnPressedTime < LONG_PRESS_TIME)) {
        if (currentMillis - lastCommandTime > COOLDOWN_TIME) {
          currentTrack--;
          if(currentTrack < 1) {
            currentTrack = (totalTracks > 0) ? totalTracks : 1;
          }
          
          // FIXED: Uses the new sequential robust timing play execution
          forcePlayTrack(currentTrack); 
          isPlaying = true;
          lastCommandTime = currentMillis; 
          updateScreen("Playing");
        }
      }
      prevIsPressed = false;
    }
  }
  
  // ==========================================
  // 4. DFPLAYER AUTO-ADVANCE HANDLING
  // ==========================================
  if (myDFPlayer.available()) {
    uint8_t dfType = myDFPlayer.readType();
    if (dfType == DFPlayerCardOnline || dfType == DFPlayerCardInserted) {
       totalTracks = myDFPlayer.readFileCounts();
    }
    if (dfType == DFPlayerPlayFinished) {
       if (currentMillis - lastCommandTime > COOLDOWN_TIME) {
         currentTrack++;
         if (totalTracks > 0 && currentTrack > totalTracks) currentTrack = 1;
         
         forcePlayTrack(currentTrack);
         updateScreen("Playing");
       }
    }
  }
}



#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// DFPlayer serial communication
// Arduino RX = D10, Arduino TX = D11
SoftwareSerial playerSerial(10, 11);
DFRobotDFPlayerMini player;

// Pin assignments
const int PIR_PIN = 2;

// Audio settings
const int VOLUME = 25;
const int TOTAL_TRACKS = 44;

// 45-second cooldown
const unsigned long COOLDOWN_MS = 45000;

// State variables
bool cooldownActive = false;
bool lastMotionState = false;

unsigned long cooldownStart = 0;

void playRandomTrack() {
  int track = random(1, TOTAL_TRACKS + 1);

  Serial.print("Playing track ");
  Serial.println(track);

  player.playMp3Folder(track);

  // Start cooldown immediately
  cooldownActive = true;
  cooldownStart = millis();

  Serial.println("45-second cooldown started.");
}

void setup() {
  Serial.begin(9600);
  playerSerial.begin(9600);

  pinMode(PIR_PIN, INPUT);

  randomSeed(analogRead(A0));

  delay(2000);

  Serial.println("Starting Murray the demonic skull...");

  // Initialize DFPlayer
  if (player.begin(playerSerial, false, true)) {
    Serial.println("DFPlayer ready.");
    player.volume(VOLUME);
  } else {
    Serial.println("DFPlayer initialization failed!");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("PIR motion detection enabled.");
  Serial.println("45-second cooldown enabled.");
}

void loop() {
  bool motionDetected = digitalRead(PIR_PIN) == HIGH;

  // PIR debugging
  if (motionDetected != lastMotionState) {
    Serial.print("PIR: ");
    Serial.println(motionDetected ? "HIGH - MOTION" : "LOW - CLEAR");
  }

  // Check cooldown
  if (cooldownActive &&
      millis() - cooldownStart >= COOLDOWN_MS) {

    cooldownActive = false;

    Serial.println("Cooldown finished.");
    Serial.println("Murray is ready for another greeting.");
  }

  // Trigger only when motion changes from LOW to HIGH
  if (motionDetected && !lastMotionState) {

    Serial.println("Motion detected!");

    if (cooldownActive) {
      Serial.println("Ignored: 45-second cooldown active.");
    } else {
      playRandomTrack();
    }
  }

  lastMotionState = motionDetected;
}

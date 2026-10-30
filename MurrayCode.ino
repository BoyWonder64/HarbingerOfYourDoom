
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial playerSerial(10, 11);
DFRobotDFPlayerMini player;

const int PIR_PIN = 2;
const int VOLUME = 25;
const int TOTAL_TRACKS = 44;
const unsigned long MOTION_INTERVAL_MS = 20000;

bool lastMotionState = false;
unsigned long lastPlayTime = 0;
bool hasPlayed = false;

void playRandomTrack() {
  int track = random(1, TOTAL_TRACKS + 1);

  Serial.print("Playing track ");
  Serial.println(track);

  player.playMp3Folder(track);
}

void setup() {
  Serial.begin(9600);
  playerSerial.begin(9600);

  pinMode(PIR_PIN, INPUT);

  randomSeed(analogRead(A0));

  Serial.println("Starting Murray the demonic skull...");

  if (player.begin(playerSerial, false, true)) {
    Serial.println("DFPlayer ready.");
    player.volume(VOLUME);
  } else {
    Serial.println("DFPlayer initialization failed!");
    while (true) {}
  }

  Serial.println("Waiting for motion...");
}

void loop() {
  bool motionDetected = digitalRead(PIR_PIN) == HIGH;

  if (motionDetected != lastMotionState) {
    Serial.print("PIR: ");
    Serial.println(motionDetected ? "HIGH - MOTION" : "LOW - CLEAR");
  }

  // Only trigger when motion first changes from LOW to HIGH.
  if (motionDetected && !lastMotionState) {

    Serial.println("NEW MOTION DETECTED!");

    if (!hasPlayed ||
        millis() - lastPlayTime >= MOTION_INTERVAL_MS) {

      playRandomTrack();

      lastPlayTime = millis();
      hasPlayed = true;

    } else {
      Serial.println("Motion detected, but cooldown is active.");
    }
  }

  lastMotionState = motionDetected;
}

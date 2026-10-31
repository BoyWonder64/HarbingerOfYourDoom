# Murray the Demonic Skull

An Arduino Nano project that uses a PIR motion sensor to trigger a random greeting from a DFPlayer Mini. After playing a track, the project ignores new motion triggers for 45 seconds.

## What the code does

`MurrayCode.ino`:

1. Starts serial debugging at 9600 baud and sets up the DFPlayer Mini.
2. Sets the DFPlayer volume to 25 (the module's volume scale is 0–30).
3. Reads the PIR sensor on D2.
4. On a LOW-to-HIGH motion transition, chooses a random track from 1 to 44 and plays it from the DFPlayer's `MP3` folder.
5. Starts a 45-second cooldown as soon as playback is triggered. Motion during the cooldown is ignored.
6. Reports sensor changes, playback, and cooldown status in the Serial Monitor.

The DFPlayer is initialized with its busy-pin feedback disabled, so the `BUSY` pin is not used by this sketch.

## Parts and wiring

Use an Arduino Nano, PIR motion sensor, DFPlayer Mini, microSD card, speaker, and a 1 kΩ resistor.

| Component | Pin | Connect to |
|---|---|---|
| PIR sensor | VCC | Nano 5V |
| PIR sensor | OUT | Nano D2 |
| PIR sensor | GND | Nano GND |
| DFPlayer Mini | VCC | Nano 5V |
| DFPlayer Mini | GND | Nano GND |
| DFPlayer Mini | TX | Nano D10 (software serial RX) |
| DFPlayer Mini | RX | Nano D11 (software serial TX), through a 1 kΩ resistor |
| Speaker | Positive / negative | DFPlayer SPK1 / SPK2 |

Connect the Nano, PIR, and DFPlayer grounds together. Do not connect either speaker wire to Arduino ground; connect the speaker directly across SPK1 and SPK2.

### Wiring diagram

```text
Arduino Nano                 PIR sensor
-----------                  ----------
5V -------------------------- VCC
D2 <------------------------- OUT
GND ------------------------- GND

Arduino Nano                 DFPlayer Mini
-----------                  -------------
5V -------------------------- VCC
GND ------------------------- GND
D10 <------------------------ TX
D11 ---- 1 kΩ resistor -----> RX

DFPlayer Mini                Speaker
-------------                -------
SPK1 ------------------------ Positive
SPK2 ------------------------ Negative
```

In the serial connection, D10 is the Nano's receive pin and connects to the DFPlayer's TX. D11 is the Nano's transmit pin and connects through the resistor to the DFPlayer's RX. If using breadboard power rails, connect Nano 5V to the positive rail and Nano GND to the negative rail, then power the PIR and DFPlayer from those same rails. The DFPlayer `BUSY` pin is intentionally left unconnected because the sketch does not read it.

## Format and arrange the microSD card

The DFPlayer reads the audio from its microSD card. Back up the card first, since formatting erases its contents.

1. Format the card as **FAT16 or FAT32** using your computer's format utility. Use a card capacity supported by your DFPlayer module.
2. Create a folder named `MP3` at the root of the card. The folder name should be uppercase.
3. Put up to 44 MP3 audio files in that folder, named with four-digit track numbers:

```text
SD card/
└── MP3/
    ├── 0001.mp3
    ├── 0002.mp3
    ├── 0003.mp3
    └── ...
    └── 0044.mp3
```

The code randomly requests track numbers 1 through 44, so ensure every corresponding file exists. If you have fewer audio files, change `TOTAL_TRACKS` in `MurrayCode.ino` to the number of tracks actually on the card. Keep filenames zero-padded as shown. Copy the files to the card, safely eject it, then insert it into the DFPlayer Mini.

## Arduino project folder and files

The repository currently contains the sketch at its root:

```text
HarbingerOfYourDoom/
├── MurrayCode.ino
└── README.MD
```

To open it in Arduino IDE, put `MurrayCode.ino` in a sketch folder also named `MurrayCode` (Arduino IDE expects the main `.ino` file to match its sketch folder name), then open `MurrayCode.ino`. The resulting Arduino sketch layout is:

```text
MurrayCode/
└── MurrayCode.ino
```

Keep the audio files on the DFPlayer's microSD card; they do not go in the Arduino sketch folder.

## Build and upload

1. Install the **DFRobotDFPlayerMini** library using Arduino IDE's Library Manager. `SoftwareSerial` is included with the standard AVR Arduino core.
2. Connect the Nano over USB and select the correct board and port in Arduino IDE.
3. Open and upload `MurrayCode.ino`.
4. Open Serial Monitor at **9600 baud** to see startup, motion, and cooldown messages.
5. Allow the PIR sensor to settle after power-up before testing. Move in front of it; a new LOW-to-HIGH sensor transition should start a random greeting unless the cooldown is active.

If the DFPlayer fails to initialize, check its power and ground, the D10/D11 serial wiring, the formatted card, and the `MP3/0001.mp3`-style filenames.

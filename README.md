# Arduino Nano MP3 Player

This project is a small MP3 player built with an Arduino Nano, a DFPlayer Mini,
a 0.91-inch I2C OLED display, three buttons, and a TRRS audio connection.

## Controls

- Press **Play/Pause** once to start or pause the current track.
- Press **Next** briefly to play the next track.
- Press **Previous** briefly to play the previous track.
- Hold **Next** to increase the volume.
- Hold **Previous** to decrease the volume.

The buttons use the Arduino's internal pull-up resistors, so each button is
connected between its Arduino pin and ground.

## Track names

Track names are entered manually in `mp3.ino` in the `songNames` array. The
array currently contains `".."` placeholder entries between some real names.
These placeholders are an existing workaround for the DFPlayer behavior in
this build: after a skip, the module may stop or become one track out of sync
with the display. Pressing skip again advances both the module and the display
to the expected track.

Keep the names in the same order as the files on the SD card. The sketch uses
the track number as the array index, so a name at index 0 represents track 1.

## Audio noise fixes

Two wiring changes reduce audible buzzing:

1. Connect the TRRS sleeve/ring-2 ground directly to DFPlayer pin 7. This
  separates the audio return path from noise on the rest of the breadboard.
2. Place a 1 kOhm resistor between Arduino D3 and the DFPlayer RX pin.

See [wiring.md](wiring.md) for the complete connection list and
[materials.md](materials.md) for the required parts.

## Software

Install these Arduino libraries before compiling:

- SoftwareSerial
- DFRobotDFPlayerMini
- Adafruit GFX Library
- Adafruit SSD1306

Format the SD card as FAT32 and place the audio files in the naming/order
expected by the DFPlayer module. Upload `mp3.ino` to the Arduino Nano.

# Wiring

All Arduino pin names below use the Arduino Nano labels (`A4`, `A5`, `D2`,
etc.). Disconnect power before changing the wiring.

## Arduino Nano

| Nano pin | Connection |
| --- | --- |
| 5V | Breadboard VCC rail |
| GND | Breadboard GND rail |
| A5 | OLED SCL/SCK |
| A4 | OLED SDA |
| D2 | DFPlayer TX, pin 3 |
| D3 | 1 kOhm resistor, then DFPlayer RX, pin 2 |
| D4 | Play/pause button |
| D5 | Next/volume-up button |
| D6 | Previous/volume-down button |

## OLED display

| OLED pin | Connection |
| --- | --- |
| GND | Breadboard GND rail |
| VCC | Breadboard VCC rail |
| SCK/SCL | Nano A5 |
| SDA | Nano A4 |

## DFPlayer Mini

| DFPlayer pin | Connection |
| --- | --- |
| VCC, pin 1 | Breadboard VCC rail |
| RX, pin 2 | 1 kOhm resistor, then Nano D3 |
| TX, pin 3 | Nano D2 |
| DAC_R, pin 4 | TRRS ring 1, pin 2 |
| DAC_L, pin 5 | TRRS tip, pin 1 |
| GND, pin 7 | TRRS ring 2, pin 3 |
| GND, pin 10 | Breadboard GND rail |

The direct connection from DFPlayer pin 7 to the TRRS audio ground is
intentional and helps reduce constant background buzzing.

## Buttons

Connect one terminal of each button to the listed Nano pin and the other
terminal to the breadboard GND rail:

- Play/pause: D4
- Next/volume up: D5
- Previous/volume down: D6

The sketch configures these inputs with `INPUT_PULLUP`.

## TRRS audio connection

- Tip, pin 1 -> DFPlayer DAC_L, pin 5
- Ring 1, pin 2 -> DFPlayer DAC_R, pin 4
- Ring 2, pin 3 -> DFPlayer GND, pin 7








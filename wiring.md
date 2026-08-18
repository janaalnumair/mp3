arduino nano
5v -> VCC on breadboard
GND -> GND on breadboard
5a -> SCK on screen (3rd pin)
4a -> SDA on screen (4th pin)
2d -> TX on DFPlayer (3rd pin)
3d -> 1k ressistor -> rx on dfplayer (2nd pin)
4d -> play/pause button
5d -> skip forward/volume up button
6d -> skip backwards/volume down button

screen
gnd -> GND on breadboard
VCC -> VCC on breadboard
sck -> 5a on arduino
sda -> 4a on arduino

dfplayer
vcc(pin1) -> vcc on breadboard
rx(pin2) -> 1k ressistor -> 3d on arduino
tx(pin3) -> 2d on arduino
dac_r(pin4) -> ring1(pin2) on trrs
dac_l(pin5) -> tip(pin1) on trrs
gnd(pin7) -> ring2(pin3) on trrs
gnd(pin10) -> gnd on breadboard

trrs 
tip(pin1) -> dac_l(pin5) on dfplayer
ring1(pin2) -> dac_r(pin4) on dfplayer
ring2(pin3) -> gnd(pin7) on dfplayer

play/pause button -> 4d and gnd on breadboard
skip forward/volume up button -> 5d and GND on breadboard
skip backwards/volume down button -> 6d and GND on breadboard








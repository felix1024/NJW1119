# NJW1119

Arduino library for communicating with the NJW1119 audio tone control chip. It has two channels a & b and independet controls for treble, mid and bass.
The data sheets can be found [here](https://www.nisshinbo-microdevices.co.jp/en/pdf/datasheet/NJW1119A_E.pdf) (pdf).

## Download

Download the latest release over at the [Releases](https://github.com/felix1024/NJW1119/releases) page.

## Example

```c++
#include <NJW1119.h>

// The address wired into the muses chip (usually 0).
const uint_8 NJW_ADDRESS = 0;
const int CHIP_SELECT = 21;

NJW1119 njw(NJW_ADDRESS, CHIP_SELECT);
int currentTreble = 2;
int currentMid = -3;
int currentBass = 4;

void setup()
{
  // Initialize muses (SPI, pin modes)...
  njw.begin();

  // need to be unmuted first
  njw.mute(false);
  // tonecontrol needs to be enabled
  njw.setToneControl(true);
}

void loop()
{
  // adjust treble, mid and bass for both channels a & b
  // use two args in call to control a & b channel differently
  njw.setTreble(currentTreble;
  njw.setMid(currentMid);
  njw.setBass(currentBass);

  delay(10);
}

```

## Problems

Please post any problems on the [Issues](https://github.com/felix1024/njw1119/issues) page.

## License

Please read over the LICENSE file included in the project.

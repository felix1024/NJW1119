/*
  The MIT License (MIT)

  Copyright (c) 2024 Felix Thommen

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef INCLUDED_NJW1119
#define INCLUDED_NJW1119

#include <Arduino.h>
#include <SPI.h>

// control select addresses, chip address (low 4) ignored.
const uint8_t s_control_treble_a    = 0b00001100;
const uint8_t s_control_mid_a       = 0b00011100;
const uint8_t s_control_bass_a      = 0b00101100;
const uint8_t s_control_treble_b    = 0b00111100;
const uint8_t s_control_mid_b		= 0b01001100;
const uint8_t s_control_bass_b      = 0b01011100;

// control state bits.
const uint8_t s_state_bit_bypass_tone  = 2;
const uint8_t s_state_bit_mute         = 1;

class NJW1119
{
public:

	// specify a connection over an address using two output pins.
	NJW1119(uint8_t _address, uint8_t _ss);

	// set the pins in their correct states.
	void begin();

	// chip has channel a and b, using filters for treble, mid and bass
	// range is cut -12dB to boost +12dB step is 1dB
	// MSB (D15) 0 = cut (initial setting)
	// MSB (D15) 1 = boost
	// input goes from -12 to +12
	// if called with on arg, a & b gets same value
	// treble tone control
	void setTreble(uint8_t a_ch, uint8_t b_ch);
	inline void setTreble(uint8_t val) { setTreble(val, val); }
	
	// mid tone control
	void setMid(uint8_t a_ch, uint8_t b_ch);
	inline void setMid(uint8_t val) { setMid(val, val); }

	// bass tone control
	void setBass(uint8_t a_ch, uint8_t b_ch);
	inline void setBass(uint8_t val) { setBass(val, val); }

  	// mute output 
  	void mute(bool on);

	// enable or disable tone control.
	void setToneControl(bool on);

private:
  void transfer(uint8_t address, uint8_t data);

private:
	// for multiple chips on the same bus line.
	uint8_t _address;
    uint8_t _ss;
    SPISettings _spiSettings;

	// status bits
	//   [7-3]: treble control value
	//   2:     tone control (default 0 = off ->  bypass)
	//   1:		mute channel (default 1 = mute -> no output)
	//   0:		not used
	// library handles toneControl switch and mute switch for both channels a & b 
	uint8_t _status;
    // treble value needs to be saved, because it can not read back and the register
    // is combined with mute and tone control switch bits
	uint8_t _treble_a; // save treble value for channel a
	uint8_t _treble_b; // save treble value for chennel b
};
#endif // INCLUDED_NJW1119
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

#include "NJW1119.h"

static inline int8_t set_tone(int8_t val)
{
	// check set limits and set cut/boost
	// MSB = 1 -> boost
	// MSB = 0 -> cut
	// #=====================================#
	// |  -12 dB | in: [-12] -> 0b01100000 |
	// |  +12 dB | in: [+12] -> 0b11100000 |
	// #=====================================#
	uint8_t myval;
	// limit to 12 and shift left 3 bits
	myval = min(abs(val), 12) << 3;
	// set cut or boost bit according sign
	if (val < 0) bitWrite(myval, 7, 1);
	return myval;
}

NJW1119::NJW1119(uint8_t address, uint8_t ss)  {
	_address = address & 0b1111;
    _ss = ss;
    _status = 0b00000010;
    _treble_a = 0;
    _treble_b = 0;
}

void NJW1119::begin()
{
	// configure select pin as output and set it high
	digitalWrite(_ss, HIGH);
	pinMode(_ss, OUTPUT);
    // define SPI clockspeed, endian and mode
    _spiSettings = SPISettings(250000, MSBFIRST, SPI_MODE2);
	SPI.begin();
}

void NJW1119::setTreble(uint8_t a_ch, uint8_t b_ch)
{
	// save treble values
	_treble_a = set_tone(a_ch);
	_treble_b = set_tone(b_ch);
	// send treble combined with status (mute and tonecontrol bypass)
	transfer(s_control_treble_a, (_treble_a | _status));
	transfer(s_control_treble_b, (_treble_b | _status));		
}

void NJW1119::setMid(uint8_t a_ch, uint8_t b_ch)
{
	// send mid values
	transfer(s_control_mid_a, set_tone(a_ch));
	transfer(s_control_mid_b, set_tone(b_ch));
}

void NJW1119::setBass(uint8_t a_ch, uint8_t b_ch)
{
	// send bass values
	transfer(s_control_bass_a, set_tone(a_ch));
	transfer(s_control_bass_b, set_tone(b_ch));
}

void NJW1119::mute(bool on)
{
	// set mute bit and send with stored treble value;
  	bitWrite(_status, s_state_bit_mute, on);
	transfer(s_control_treble_a, (_treble_a | _status));
	transfer(s_control_treble_b, (_treble_b | _status));		
}

void NJW1119::setToneControl(bool on)
{
	// set bypass tone bit and send with stored treble value;
  	bitWrite(_status, s_state_bit_bypass_tone, on);
	transfer(s_control_treble_a, (_treble_a | _status));
	transfer(s_control_treble_b, (_treble_b | _status));		
}

void NJW1119::transfer(uint8_t address, uint8_t data)
{
  SPI.beginTransaction(_spiSettings);
  digitalWrite(_ss, LOW);
  SPI.transfer(data);
  SPI.transfer(address | _address);
  digitalWrite(_ss, HIGH);
  SPI.endTransaction();
}

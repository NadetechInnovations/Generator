// AD9834 library for Arduino, v0.2
// 11.10.2018, Arduino v1.8.7, LZ2WSG, KN34PC
//---------------------------------------------------------------------------------------------------------
#include "mbed.h"
#include "ad9834.h"
//---------------------------------------------------------------------------------------------------------
AD9834::AD9834(uint8_t _fsync, uint8_t _clk, uint8_t _data) {
  FSYNC = _fsync;
  CLK = _clk;
  DATA = _data;
}
//---------------------------------------------------------------------------------------------------------
void AD9834::init(uint32_t xt) {
  F_MCLK = xt;

  pinMode(FSYNC, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  digitalWrite(FSYNC, HIGH);
  digitalWrite(CLK, HIGH);

  write_reg(0x0100);                           // reset (RESET:1)
  write_reg(0x2100);                           // control (B28:1, HLB:0)
  control_reg = 0x2000;                        // default: DAC powered up, Internal clock enabled, sine
  write_reg(control_reg);
}
//---------------------------------------------------------------------------------------------------------
void AD9834::set_freq(uint32_t frequency) {
  uint32_t f = (uint32_t)(((uint64_t)frequency << 28) / F_MCLK);

  uint16_t data_1 = 0x4000 | (0x3FFF & (uint16_t)(f >> 14));
  uint16_t data_0 = 0x4000 | (0x3FFF & (uint16_t)f);

  write_reg(data_0);                           // FREQ0, data_0
  write_reg(data_1);                           // FREQ0, data_1
  write_reg(0xC000);                           // PHASE0
}
//---------------------------------------------------------------------------------------------------------
void AD9834::write_reg(uint16_t data) {
  digitalWrite(CLK, HIGH);                     // falling edges
  digitalWrite(FSYNC, LOW);
  shiftOut(DATA, CLK, MSBFIRST, data >> 8);    // high byte
  shiftOut(DATA, CLK, MSBFIRST, data & 0xFF);  // low byte
  digitalWrite(FSYNC, HIGH);
}
//---------------------------------------------------------------------------------------------------------
void AD9834::set_waveform(uint8_t wave) {
  switch (wave) {
    case     SINE: bitClear(control_reg, 5); bitClear(control_reg, 1); break;  // [OPBITEN, MODE] = [00]
    case TRIANGLE: bitClear(control_reg, 5); bitSet(control_reg, 1); break;    // [OPBITEN, MODE] = [01]
  }
  write_reg(control_reg);
}
//---------------------------------------------------------------------------------------------------------
void AD9834::dac_control(uint8_t state) {
  if (state == 0)
    bitClear(control_reg, 6);                  // DAC powered up
  else if (state == 1)
    bitSet(control_reg, 6);                    // DAC powered down

  write_reg(control_reg);
}
//---------------------------------------------------------------------------------------------------------
void AD9834::internal_clock_control(uint8_t state) {
  if (state == 0)
    bitClear(control_reg, 7);                  // enable Internal clock
  else if (state == 1)
    bitSet(control_reg, 7);                    // disable Internal clock

  write_reg(control_reg);
}

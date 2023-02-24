#include "mbed.h"

#ifndef ad9834_h
#define ad9834_h

#define SINE     0
#define TRIANGLE 1
//---------------------------------------------------------------------------------------------------------
class AD9834 {
  public:
    AD9834(uint8_t, uint8_t, uint8_t);               // Arduino pins to FSYNC, CLK, DATA
    void init(uint32_t xt);                          // XTAL crystal frequency [Hz]
    void set_freq(uint32_t frequency);               // Fout [Hz]
    void dac_control(uint8_t state);                 // DAC [0: powered up, 1: powered down]
    void internal_clock_control(uint8_t state);      // Internal clock [0: enabled, 1: disabled]
    void set_waveform(uint8_t wave);                 // Output waveform [SINE/TRIANGLE]

  private:
    uint32_t F_MCLK;
    uint8_t FSYNC;
    uint8_t CLK;
    uint8_t DATA;
    uint16_t control_reg;

    void write_reg(uint16_t data);
};
//---------------------------------------------------------------------------------------------------------
#endif

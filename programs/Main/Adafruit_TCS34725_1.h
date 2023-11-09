/**************************************************************************/
/*! 
    @file     Adafruit_TCS34725.h
    @author   KTOWN (Adafruit Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _TCS34725_1_HW_H_
#define _TCS34725_1_HW_H_

#include "Adafruit_TCS34725_common.h"

class Adafruit_TCS34725_1 {
 public:
  Adafruit_TCS34725_1(tcs34725IntegrationTime_t = TCS34725_INTEGRATIONTIME_2_4MS, tcs34725Gain_t = TCS34725_GAIN_1X);
  
  boolean  begin(void);
  void     setIntegrationTime(tcs34725IntegrationTime_t it);
  void     setGain(tcs34725Gain_t gain);
  void     getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
  uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
  uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b);
  void     write8 (uint8_t reg, uint32_t value);
  uint8_t  read8 (uint8_t reg);
  uint16_t read16 (uint8_t reg);
  void setInterrupt(boolean flag);
  void clearInterrupt(void);
  void setIntLimits(uint16_t l, uint16_t h);
  void     enable(void);

 private:
  boolean _tcs34725Initialised;
  tcs34725Gain_t _tcs34725Gain;
  tcs34725IntegrationTime_t _tcs34725IntegrationTime; 
  
  void     disable(void);
};

#endif

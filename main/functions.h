#pragma once

#include "Arduino.h"

constexpr uint32_t zigzag(uint32_t time, uint32_t period, uint32_t amplitude)
{
    uint32_t halfPeriod = period / 2;
    uint32_t timeMod = time % period;

    return (timeMod < halfPeriod)
               ? (timeMod * amplitude) / halfPeriod
               : amplitude - ((timeMod - halfPeriod) * amplitude) / halfPeriod;
}
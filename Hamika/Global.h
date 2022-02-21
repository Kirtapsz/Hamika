#pragma once

#include <KIR\AL\KIR5_event_engine.h>

extern std::shared_ptr<KIR5::EventEngine> eventEngine;

constexpr float CPS = KIR5_60_CPS;
constexpr float CA = 1 / CPS;
constexpr int blockSizeInPixel = 64;
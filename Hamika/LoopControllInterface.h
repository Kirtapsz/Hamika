#pragma once

#include <KIR/KIR4_time.h>

class LoopControllerInterface
{
	public: unsigned long long loopCounter;
	public: unsigned long long currentTimeMS;

	private: KIR4::timer timer;

	public: void loopStart();
	public: void loopRoll();
};
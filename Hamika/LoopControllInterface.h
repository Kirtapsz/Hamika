#pragma once

#include <KIR/KIR4_time.h>

class LoopControllerInterface
{
	public: virtual void failure(const std::string &msg) = 0;

	public: unsigned long long loopCounter;
	public: unsigned long long currentTimeMS;

	private: KIR4::timer timer;

	public: void loopStart();
	public: void loopRoll();
};
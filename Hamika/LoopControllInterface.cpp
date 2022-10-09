#include "LoopControllInterface.h"

void LoopControllerInterface::loopStart()
{
	loopCounter = 0;
	currentTimeMS = 0;
	timer.start();
}

void LoopControllerInterface::loopRoll()
{
	++loopCounter;
	currentTimeMS = static_cast<unsigned long long>(timer.get_elapsed_time_now_ms());
}

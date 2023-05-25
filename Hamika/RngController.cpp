#include "RngController.h"
#include "Tools.h"

#include <KIR/KIR4_console.h>

#include <tuple>

RngController::RngController(LoopControllerInterface &loopControllerInterface):
	loopControllerInterface(loopControllerInterface)
{

}
bool RngController::rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec)
{
	std::uniform_int_distribution<unsigned> distribution(0, (int)(CPS * 100000));
	if (distribution(generator) <= (unsigned)(chancePerSec * 100000))
	{
		return true;
	}
	else
	{
		return false;
	}
}

RngRecord::RngRecord(LoopControllerInterface &loopControllerInterface, Res::Logger &logger):
	RngController(loopControllerInterface), logger(logger)
{

}
bool RngRecord::rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec)
{
	bool ret = RngController::rollTrigger(rootID, typeID, chancePerSec);
	if (ret)
	{
		logger.record<Res::Log::RNGenerator>(Res::Log::RNGenerator::ACTION_ROLL_TRIGGER, rootID, typeID, chancePerSec);
	}
	return ret;
}



RngReplay::RngReplay(LoopControllerInterface &loopControllerInterface, Res::Logger &logger):
	RngController(loopControllerInterface)
{
	rows = logger.getLogs<Res::Log::RNGenerator>();
	it = rows.begin();
}
bool RngReplay::rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec)
{
	while (true)
	{
		if (it != rows.end())
		{
			auto &loopCounter = std::get<Res::Log::RNGenerator::loopCounter>(**it);
			if (loopCounter < loopControllerInterface.loopCounter)
			{
				++it;
				loopControllerInterface.failure("RngReplay loop was jumped!");
				continue;
			}
			else if (loopCounter == loopControllerInterface.loopCounter &&
					 std::get<Res::Log::RNGenerator::rootId>(**it) == rootID &&
					 std::get<Res::Log::RNGenerator::typeId>(**it) == typeID &&
					 std::get<Res::Log::RNGenerator::chancePerSec>(**it) == chancePerSec)
			{
				++it;
				return true;
			}
		}
		break;
	}
	return false;
}




RngValidator::RngValidator(LoopControllerInterface &loopControllerInterface, Res::Logger &logger):
	RngController(loopControllerInterface)
{
	rows = logger.getLogs<Res::Log::RNGenerator>();
	it = rows.begin();
}
bool RngValidator::rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec)
{
	while (true)
	{
		if (it != rows.end())
		{
			auto &loopCounter = std::get<Res::Log::RNGenerator::loopCounter>(**it);
			if (loopCounter < loopControllerInterface.loopCounter)
			{
				++it;
				loopControllerInterface.failure(concatenate("RngValidator loop was jumped at ", loopCounter, " (",
												std::get<Res::Log::RNGenerator::rootId>(**it), ", ",
												std::get<Res::Log::RNGenerator::typeId>(**it), ", ",
												std::get<Res::Log::RNGenerator::chancePerSec>(**it), ")"));
				continue;
			}
			else if (loopCounter == loopControllerInterface.loopCounter &&
					 std::get<Res::Log::RNGenerator::rootId>(**it) == rootID &&
					 std::get<Res::Log::RNGenerator::typeId>(**it) == typeID &&
					 std::get<Res::Log::RNGenerator::chancePerSec>(**it) == chancePerSec)
			{
				++it;
				return true;
			}
		}
		break;
	}
	return false;
}

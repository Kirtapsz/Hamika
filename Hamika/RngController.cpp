#include "RngController.h"
#include "Tools.h"

#include <KIR/KIR4_console.h>

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

RngRecord::RngRecord(LoopControllerInterface &loopControllerInterface, Logger &logger):
	RngController(loopControllerInterface), logger(logger)
{

}
bool RngRecord::rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec)
{
	bool ret = RngController::rollTrigger(rootID, typeID, chancePerSec);
	if (ret)
	{
		logger.record<RNGeneratorLog>(RNGeneratorLog_::ACTION_ROLL_TRIGGER, rootID, typeID, chancePerSec);
	}
	return ret;
}



RngReplay::RngReplay(LoopControllerInterface &loopControllerInterface, Logger &logger):
	RngController(loopControllerInterface)
{
	auto &rows_ = logger.getLogs(LogID::_RNGenerator);
	rows.resize(rows_.size());

	for (size_t i = 0; i < rows_.size(); ++i)
	{
		rows[i] = static_cast<RNGeneratorLog *>(rows_[i]);
	}

	it = rows.begin();
}
bool RngReplay::rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec)
{
	while (true)
	{
		if (it != rows.end())
		{
			if ((*it)->loopCounter < loopControllerInterface.loopCounter)
			{
				++it;
				clog << KIR4::LRED << "RngReplay loop was jumped!" << KIR4::eol;
				continue;
			}
			else if ((*it)->loopCounter == loopControllerInterface.loopCounter &&
					 std::get<RNGeneratorLog_::_rootIdIndex >((*it)->data) == rootID &&
					 std::get<RNGeneratorLog_::_typeIdIndex >((*it)->data) == typeID &&
					 std::get<RNGeneratorLog_::_chancePerSecIndex >((*it)->data) == chancePerSec)
			{
				++it;
				return true;
			}
		}
		break;
	}
	return false;
}

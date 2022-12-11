#pragma once

#include "LoopControllInterface.h"
#include "Types.h"
#include "Logger.h"

struct RNGeneratorLog_
{
	static constexpr size_t _actionKeyIndex = 0;
	static constexpr size_t _rootIdIndex = 1;
	static constexpr size_t _typeIdIndex = 2;
	static constexpr size_t _chancePerSecIndex = 3;

	enum ACTION_KEY: unsigned __int8
	{
		ACTION_ROLL_TRIGGER = 1,
	};

	typedef std::tuple<ACTION_KEY, Type::ID, unsigned __int16, float> Type;
};
typedef LogBaseData<LogID::_RNGenerator, RNGeneratorLog_::Type> RNGeneratorLog;


struct RngController
{
	LoopControllerInterface &loopControllerInterface;

	RngController(LoopControllerInterface &loopControllerInterface);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

struct RngRecord: RngController
{
	Logger &logger;

	RngRecord(LoopControllerInterface &loopControllerInterface, Logger &logger);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

struct RngReplay: RngController
{
	std::vector<RNGeneratorLog *> rows;
	std::vector<RNGeneratorLog *>::iterator it;

	RngReplay(LoopControllerInterface &loopControllerInterface, Logger &logger);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

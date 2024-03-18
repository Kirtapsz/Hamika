#pragma once

#include "LoopControllInterface.h"
#include "Types.h"
#include "Logger.h"

namespace Res::Log
{
	struct RNGenerator: Base, KIR5::StreamRecords::Record<
		std::uint32_t,
		std::uint32_t,
		std::uint8_t,
		Type::ID,
		std::uint16_t,
		std::float_t
	>
	{
		enum ACTION_KEY: std::uint8_t
		{
			ACTION_ROLL_TRIGGER = 1,
		};

		constexpr static std::size_t loopCounter = 0;
		constexpr static std::size_t currentTimeMS = 1;
		static constexpr std::size_t actionKey = 2;
		static constexpr std::size_t rootId = 3;
		static constexpr std::size_t typeId = 4;
		static constexpr std::size_t chancePerSec = 5;

		constexpr static std::array<const char *, 6> keys{{"LoopCounter", "CurrentTimeMS", "ActionKey", "RootID", "TypeID", "ChancePerSec"}};

		inline RNGenerator()
		{

		}
		inline RNGenerator(std::uint32_t loopCounter_, std::uint32_t currentTimeMS_, std::uint8_t actionKey_,
						   Type::ID rootId_, std::uint16_t typeId_, std::float_t chancePerSec_)
		{
			std::get<loopCounter>(*this) = loopCounter_;
			std::get<currentTimeMS>(*this) = currentTimeMS_;
			std::get<actionKey>(*this) = actionKey_;
			std::get<rootId>(*this) = rootId_;
			std::get<typeId>(*this) = typeId_;
			std::get<chancePerSec>(*this) = chancePerSec_;
		}
	};
}


struct RngController
{
	LoopControllerInterface &loopControllerInterface;

	RngController(LoopControllerInterface &loopControllerInterface);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

struct RngRecord: RngController
{
	Res::Logger &logger;

	RngRecord(LoopControllerInterface &loopControllerInterface, Res::Logger &logger);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

struct RngReplay: RngController
{
	std::vector<Res::Log::RNGenerator *> rows;
	std::vector<Res::Log::RNGenerator *>::iterator it;

	RngReplay(LoopControllerInterface &loopControllerInterface, Res::Logger &logger);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

struct RngValidator: RngController
{
	std::vector<Res::Log::RNGenerator *> rows;
	std::vector<Res::Log::RNGenerator *>::iterator it;

	RngValidator(LoopControllerInterface &loopControllerInterface, Res::Logger &logger);

	virtual bool rollTrigger(Type::ID rootID, unsigned __int16 typeID, float chancePerSec);
};

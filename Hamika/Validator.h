#pragma once

#include "LoopControllInterface.h"
#include "Types.h"
#include "Logger.h"

namespace Res::Log
{
	struct ObjectValidator: Base, KIR5::StreamRecords::Record<
		std::uint32_t,
		std::uint32_t,
		std::uint8_t,
		JsonRecord
	>
	{
		enum OBJECT_TYPE: std::uint8_t
		{
			OBJECT_T = 1,
			REMAIN_T = 2,
			STATUSBAR_T = 3,
			TERMINATE_T = 4,
			RESULT_T = 5,
		};

		constexpr static std::size_t loopCounter = 0;
		constexpr static std::size_t currentTimeMS = 1;
		static constexpr size_t validateType = 2;
		static constexpr size_t objectProperties = 3;

		constexpr static std::array<const char *, 4> keys{{"LoopCounter", "CurrentTimeMS", "ValidateType", "ObjectProperties"}};

		inline ObjectValidator()
		{

		}
		inline ObjectValidator(std::uint32_t loopCounter_, std::uint32_t currentTimeMS_, std::uint8_t validateType_, const ::Json &objectProperties_)
		{
			std::get<loopCounter>(*this) = loopCounter_;
			std::get<currentTimeMS>(*this) = currentTimeMS_;
			std::get<validateType>(*this) = validateType_;
			std::get<objectProperties>(*this) = objectProperties_;
		}
	};
}

struct Validator
{
	struct Interface: public virtual Object::Brick::Interface
	{
		public: enum RESULT: std::uint8_t
		{
			NONE,
			PASSED,
			FAILED,
		};
		private: RESULT result{RESULT::NONE};
		public: inline void setResult(RESULT result_)
		{
			if (result_ != NONE && result == NONE && result != FAILED)
			{
				result = result_;
			}
		}
		public: inline bool isPassed() const
		{
			return result == PASSED;
		}

		public: virtual Json printStatusBar() const = 0;
		public: virtual Json printResult() const = 0;
		public: virtual void terminateScene() = 0;
	};
	LoopControllerInterface &loopControllerInterface;
	Interface &scene;

	inline Validator(LoopControllerInterface &loopControllerInterface, Interface &scene):
		loopControllerInterface(loopControllerInterface),
		scene(scene)
	{

	}
};

struct ValidatorRecord: Validator
{
	Res::Logger &logger;

	inline ValidatorRecord(LoopControllerInterface &loopControllerInterface, Res::Logger &logger, Interface &scene):
		Validator(loopControllerInterface, scene),
		logger(logger)
	{

	}

	void validate(std::uint8_t validateType_, const ::Json &objectProperties_);
};

struct ValidatorReplay: Validator
{
	std::vector<Res::Log::ObjectValidator *> rows;
	std::vector<Res::Log::ObjectValidator *>::iterator it;

	ValidatorReplay(LoopControllerInterface &loopControllerInterface, Res::Logger &logger, Interface &scene);

	void validate();
	void finalize();
};

#include "Logger.h"
#include "Worlds.h"
#include "RngController.h"
#include "KeyboardController.h"
#include "Validator.h"

namespace Res
{
	namespace Log
	{
		Base::~Base()
		{

		}
	}

	void Logger::operator=(const Log::Container &record)
	{
		logContainer = std::move(record);
	}
	Logger::operator Log::Container() const
	{
		return logContainer;
	}
	void Logger::load(const std::string &filename)
	{
		path = filename;
		LoadResource(*this);
		bluePrint = findBluePrint(std::get<logContainer.blueprintHash>(logContainer));
	}
	void Logger::save(const std::string &filename) const
	{
		path = filename;
		SaveResource(*this);
	}

	const std::tuple<
		Handler<Log::Container>
	>
		Logger::handlers{
			{true, nullptr, {0x48, 0x61, 0x6D, 0x53, 0x74, 0x30, 0x30, 0x31}, {0x48, 0x61, 0x6D, 0x53, 0x74, 0x30, 0x30, 0x31}, Hash::sha512_salty1, Crypto::V1_1024},
	};
}

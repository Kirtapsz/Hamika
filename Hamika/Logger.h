#pragma once

#include <KIR/hash/KIR5_sha512.h>

#include "Tools.h"
#include "Types.h"
#include "LoopControllInterface.h"

#include <map>
#include <vector>
#include <tuple>

enum class LogID: unsigned __int8
{
	_None = 0,
	_StandardKeyboard = 1,
	_RNGenerator = 3,
};

struct LogBase
{
	unsigned __int32 loopCounter = 0;
	unsigned __int32 currentTimeMS = 0;

	virtual void write(std::vector<unsigned __int8> &buffer) = 0;
	virtual void read(std::vector<unsigned __int8> &buffer) = 0;
};


template<LogID ID_T, typename T>
struct LogBaseData: LogBase
{
	static constexpr LogID ID = ID_T;
	T data;

	template <typename... _Types>
	inline LogBaseData(_Types... _Args)
	{
		data = std::make_tuple<>(std::forward<_Types>(_Args)...);
	}

	inline LogBaseData()
	{
	}

	virtual void write(std::vector<unsigned __int8> &buffer)
	{
		std::apply([&buffer](auto&&... args)
		{
			size_t totalSize = 0;
			((totalSize += sizeof(args)), ...);
			buffer.resize(totalSize);
		}, data);

		std::apply([&buffer](auto&&... args)
		{
			size_t idx = 0;
			size_t len = 0;
			((
				idx += len,
				len = sizeof(args),
				set(args, buffer, idx, len)
				), ...);
		}, data);
	}
	virtual void read(std::vector<unsigned __int8> &buffer)
	{
		std::apply([&buffer](auto&&... args)
		{
			size_t idx = 0;
			size_t len = 0;
			((
				idx += len,
				len = sizeof(args),
				get(args, buffer, idx, len)
				), ...);
		}, data);
	}
};


struct Logger
{
	static constexpr unsigned __int8 version = 0x01;

	LoopControllerInterface &loopControllerInterface;
	std::map<LogID, std::vector<LogBase *>> logs;
	char commit[7] = {0};
	KIR5::sha512digest bluePrintHash;
	KIR5::sha512digest userHash;

	/*

	version[1] | blank commit version[7] | blue print hash [64] | user hash [64] | records...
	| LogID[1] | LEN[1] | LOOP[4] | MS[4] |

	*/

	void clear();

	Logger(LoopControllerInterface &loopControllerInterface, const KIR5::sha512digest &bluePrintHash, const KIR5::sha512digest &userHash);
	~Logger();

	template<typename L, typename... _Types>
	void record(_Types... _Args);

	const std::vector< LogBase *> &getLogs(LogID ID);

	bool load(const std::string &filename);
	bool save(const std::string &filename) const;
};


template<typename L, typename... _Types>
void Logger::record(_Types... _Args)
{
	LogBase *log = new L(std::forward<_Types>(_Args)...);
	log->loopCounter = static_cast<unsigned __int32>(loopControllerInterface.loopCounter);
	log->currentTimeMS = static_cast<unsigned __int32>(loopControllerInterface.currentTimeMS);
	logs[L::ID].push_back(log);
}
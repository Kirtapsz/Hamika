#include "Logger.h"

#include "RngController.h"
#include "KeyboardController.h"

void Logger::clear()
{
	for (auto &map : logs)
	{
		for (auto &it : map.second)
		{
			delete it;
		}
		map.second.clear();
	}
	logs.clear();
}

Logger::Logger(LoopControllerInterface &_loopControllerInterface, const KIR5::sha512digest &_bluePrintHash, const KIR5::sha512digest &_userHash):
	loopControllerInterface(_loopControllerInterface)
{
	bluePrintHash = _bluePrintHash;
	userHash = _userHash;
}

Logger::~Logger()
{
	clear();
}

const std::vector< LogBase *> &Logger::getLogs(LogID ID)
{
	return logs[ID];
}

bool Logger::load(const std::string &filename)
{
	clear();

	std::vector<unsigned __int8> buffer;
	std::vector<unsigned __int8> headBuffer(2 + sizeof(unsigned __int32) + sizeof(unsigned __int32));
	std::vector<unsigned __int8> rowBuffer;

	if (ReadFile(filename, buffer))
	{
		size_t idx = 1 + 7 + KIR5::SHA512_DIGEST_SIZE + KIR5::SHA512_DIGEST_SIZE;
		size_t idxLast;

		if (buffer.size() >= idx)
		{
			if (buffer[0] != version)
			{
				return false;
			}
			memcpy(commit, &buffer[1], 7);
			memcpy(bluePrintHash.data(), &buffer[8], KIR5::SHA512_DIGEST_SIZE);
			memcpy(userHash.data(), &buffer[8 + KIR5::SHA512_DIGEST_SIZE], KIR5::SHA512_DIGEST_SIZE);

			while (idx < buffer.size())
			{
				idxLast = idx + 2 + sizeof(unsigned __int32) + sizeof(unsigned __int32);
				if (buffer.size() < idxLast)
				{
					return false;
				}
				std::copy(buffer.begin() + idx, buffer.begin() + idxLast, headBuffer.begin());

				LogID ID = static_cast<LogID>(headBuffer[0]);
				unsigned __int8 len = headBuffer[1];
				unsigned __int32 loopCounter = 0;
				unsigned __int32 currentTimeMS = 0;
				get(loopCounter, headBuffer, 2, sizeof(unsigned __int32));
				get(currentTimeMS, headBuffer, 6, sizeof(unsigned __int32));

				idx = idxLast;
				idxLast = idx + len;
				if (buffer.size() < idxLast)
				{
					return false;
				}
				rowBuffer.resize(len);
				std::copy(buffer.begin() + idx, buffer.begin() + idxLast, rowBuffer.begin());

				LogBase *log = nullptr;
				switch (ID)
				{
					case LogID::_StandardKeyboard:
					{
						log = new StandardKeyboardLog();
						break;
					}
					case LogID::_RNGenerator:
					{
						log = new RNGeneratorLog();
						break;
					}
				}

				if (log == nullptr)
				{
					return false;
				}

				log->loopCounter = loopCounter;
				log->currentTimeMS = currentTimeMS;
				log->read(rowBuffer);
				logs[ID].push_back(log);

				idx = idxLast;
			}

		}
		else
		{
			return false;
		}
	}


	for (auto &map : logs)
	{
		std::sort(map.second.begin(), map.second.end(), [](const LogBase *l, const LogBase *s)
		{
			return l->loopCounter < s->loopCounter;
		});
	}

	return true;
}

bool Logger::save(const std::string &filename) const
{
	std::vector<unsigned __int8> buffer(1 + 7 + KIR5::SHA512_DIGEST_SIZE + KIR5::SHA512_DIGEST_SIZE);
	std::vector<unsigned __int8> headBuffer(2 + sizeof(unsigned __int32) + sizeof(unsigned __int32));
	std::vector<unsigned __int8> rowBuffer;

	buffer[0] = version;
	memcpy(&buffer[1], commit, 7);
	memcpy(&buffer[8], bluePrintHash.data(), KIR5::SHA512_DIGEST_SIZE);
	memcpy(&buffer[8 + KIR5::SHA512_DIGEST_SIZE], userHash.data(), KIR5::SHA512_DIGEST_SIZE);

	for (auto &map : logs)
	{
		for (auto &it : map.second)
		{
			it->write(rowBuffer);

			headBuffer[0] = static_cast<unsigned __int8>(map.first);
			headBuffer[1] = static_cast<unsigned __int8>(rowBuffer.size());
			set(it->loopCounter, headBuffer, 2, sizeof(unsigned __int32));
			set(it->currentTimeMS, headBuffer, 6, sizeof(unsigned __int32));

			buffer.insert(buffer.end(), headBuffer.begin(), headBuffer.end());
			buffer.insert(buffer.end(), rowBuffer.begin(), rowBuffer.end());
		}
	}

	return WriteFile(filename, buffer);
}
#pragma once

#include "BluePrint.h"

class BluePrintIO
{
	public: virtual std::shared_ptr<BluePrint> read(std::vector<unsigned __int8> &data, size_t &bytesRead) = 0;
	public: virtual std::vector<unsigned __int8> write(const std::shared_ptr<BluePrint> &bluePrint, size_t &bytesWrite) = 0;
};

class OriginalLevelBluePrintIO
{

	private: unsigned __int8 getOriginalID(Type::ID id);

	public: virtual std::shared_ptr<BluePrint> read(std::vector<unsigned __int8> &data, size_t &bytesRead);
	public: virtual std::vector<unsigned __int8> write(const std::shared_ptr<BluePrint> &bluePrint, size_t &bytesWrite);
};

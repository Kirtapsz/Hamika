#include "BluePrints.h"
#include "BluePrintIO.h"

std::vector<unsigned __int8> readFileIntoBuffer(const std::string &filename)
{
	std::vector<unsigned __int8> buffer;
	std::ifstream file(filename, std::fstream::binary);
	if (file.is_open())
	{
		file.seekg(0, file.end);
		size_t numberOfBytes = file.tellg();
		file.seekg(0, file.beg);

		buffer.resize(numberOfBytes);
		file.read((char *)buffer.data(), numberOfBytes);
	}
	file.close();
	return buffer;
}

void writeFileFromBuffer(const std::string &filename, std::vector<unsigned __int8> &buffer)
{
	std::ofstream file(filename, std::fstream::binary);
	if (file.is_open())
	{
		file.write((char *)buffer.data(), buffer.size());
	}
	file.close();
}


void OriginalLevelWorldIO::Read(const std::string filename)
{
	std::vector<unsigned __int8> buffer = readFileIntoBuffer(filename);
	levels.clear();
	if (buffer.size() % 0x600 == 0)
	{
		OriginalLevelBluePrintIO io;
		size_t bytesRead;
		int numberOfBluePrints = buffer.size() / 0x600;
		for (int i = 0; i < numberOfBluePrints; ++i)
		{
			levels.push_back(io.read(buffer, bytesRead));
			buffer = std::vector<unsigned __int8>(buffer.begin() + bytesRead, buffer.end());
		}
	}
}
void OriginalLevelWorldIO::Write(const std::string filename) const
{
	std::vector<unsigned __int8> buffer;
	OriginalLevelBluePrintIO io;
	size_t bytesWrite;
	for (auto &it : levels)
	{
		auto b_ = io.write(it, bytesWrite);
		buffer.insert(buffer.end(), b_.begin(), b_.end());
	}

	writeFileFromBuffer(filename, buffer);
}

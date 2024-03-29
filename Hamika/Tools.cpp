#include <fstream>
#include <cstdint>

#include "Tools.h"

std::vector<std::string> split(const std::string &s, char separator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(separator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));
		output.push_back(substring);
		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos));

	return output;
}

std::map<std::string, std::string> parse(const std::string &s)
{
	std::map<std::string, std::string> output;

	for (auto &it : split(s, '|'))
	{
		std::string::size_type pos = it.find(':');
		if (pos != std::string::npos)
		{
			std::string key(it.substr(0, pos));
			std::string value(it.substr(pos + 1));

			output[key] = value;
		}
	}

	return output;
}



bool WriteFile(const std::string &filename, const std::uint8_t *src, std::size_t size)
{
	std::ofstream f(filename, std::ios::binary);
	if (f.is_open())
	{
		f.write(reinterpret_cast<const char *>(src), size);
		return !f.bad();
	}
	else
	{
		return false;
	}
}
bool WriteFile(const std::string &filename, const std::vector<unsigned char> &buffer)
{
	return WriteFile(filename, reinterpret_cast<const std::uint8_t *>(buffer.data()), buffer.size());
}
bool WriteFile(const std::string &filename, const KIR5::DynamicStream &stream)
{
	return WriteFile(filename, &*stream.begin(), stream.size());
}


bool ReadFile(const std::string &filename, std::vector<unsigned char> &buffer)
{
	std::ifstream f(filename, std::ios::binary);
	if (f.is_open())
	{
		buffer = std::move(std::vector<unsigned char>(std::istreambuf_iterator<char>(f), {}));
		return !f.bad();
	}
	else
	{
		return false;
	}
}
bool ReadFile(const std::string &filename, KIR5::DynamicStream &stream)
{
	std::vector<unsigned char> buffer;
	if (ReadFile(filename, buffer))
	{
		stream = std::move(buffer);
		return true;
	}
	else
	{
		return false;
	}
}
std::string uuid()
{
	static std::size_t counter = 0;
	srand((unsigned int)time(NULL));

	char FileCode[20];

	//A:
	KIR4::to_num_sys(KIR4::time().get(), sizeof(KIR4::AsciiCodesBig) / sizeof(KIR4::AsciiCodesBig[0]), KIR4::AsciiCodesBig, &FileCode[0], 7, KIR4::AsciiCodesBig[0], true);

	//D:
	FileCode[7] = KIR4::AsciiCodesBig[rand() % sizeof(KIR4::AsciiCodesBig)];
	for (int i = 11; i < 20; i++)
		FileCode[i] = KIR4::AsciiCodesBig[rand() % sizeof(KIR4::AsciiCodesBig)];

	//C:
	FileCode[8] = 'L';

	//B:
	KIR4::to_num_sys(counter, sizeof(KIR4::AsciiCodesBig) / sizeof(KIR4::AsciiCodesBig[0]), KIR4::AsciiCodesBig, &FileCode[9], 2, KIR4::AsciiCodesBig[0], true);
	if (++counter >= 1296)
	{
		counter = 0;
	}


	FileCode[19] = '\0';
	return FileCode;
}
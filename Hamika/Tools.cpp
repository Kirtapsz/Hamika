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

#include "Global.h"

#include <KIR\AL\KIR5_event_engine.h>

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
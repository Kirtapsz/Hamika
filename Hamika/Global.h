#pragma once

#include <KIR\AL\KIR5_event_engine.h>

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

extern std::shared_ptr<KIR5::EventEngine> eventEngine;

extern std::random_device rd;
extern std::default_random_engine generator;

constexpr float CPS = KIR5_60_CPS;
constexpr float CA = 1 / CPS;
constexpr int blockSizeInPixel = 64;

inline std::string &toLower(std::string &in)
{
	std::transform(in.begin(), in.end(), in.begin(),
				   [](unsigned char c)
	{
		return std::tolower(c);
	});
	return in;
}

inline std::string toLower(const std::string &in_)
{
	std::string in = in_;
	std::transform(in.begin(), in.end(), in.begin(),
				   [](unsigned char c)
	{
		return std::tolower(c);
	});
	return in;
}

std::vector<std::string> split(const std::string &s, char separator);
std::map<std::string, std::string> parse(const std::string &s);
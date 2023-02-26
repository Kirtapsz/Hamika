#pragma once

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR/KIR5_stream.h>

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

extern std::shared_ptr<KIR5::Display> display;
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

template <typename T>
inline T &reach(std::shared_ptr<T> &map)
{
	return *map;
}

template <typename T>
inline const T &reach(const std::shared_ptr<T> &map)
{
	return *map;
}


template <typename T>
constexpr T limiter(const T &l, const T &h, const T &v)
{
	return (std::max)(l, (std::min)(h, v));
}


bool WriteFile(const std::string &filename, const std::uint8_t *src, std::size_t size);
bool WriteFile(const std::string &filename, const std::vector<unsigned char> &buffer);
bool WriteFile(const std::string &filename, const KIR5::DynamicStream &stream);

bool ReadFile(const std::string &filename, std::vector<unsigned char> &buffer);
bool ReadFile(const std::string &filename, KIR5::DynamicStream &stream);

template <typename T, typename A>
void get(T &ret, const A &data, size_t idx, size_t len)
{
	memcpy(&ret, &data[idx], len);
}
template <typename A>
void get(std::string &ret, const A &data, size_t idx, size_t len)
{
	ret.resize(len + 1, 0);
	memcpy(&ret[0], &data[idx], len);
	ret.resize(strlen(ret.c_str()));
	ret.shrink_to_fit();
}
template <typename T, typename A>
void getDx(typename T::type &ret, const A &data, size_t idxShift = 0)
{
	size_t requiredSize = T::idx + idxShift + T::len;
	if (data.size() < requiredSize)
	{
		return;
	}
	get(ret, data, T::idx + idxShift, T::len);
}


template <typename T, typename A>
void set(const T &val, A &data, size_t idx, size_t len)
{
	memcpy(&data[idx], &val, len);
}
template <typename A>
void set(const std::string &val, A &data, size_t idx, size_t len)
{
	memset(&data[idx], 0, len);
	if (val.length())
	{
		memcpy(&data[idx], &val[0], (std::min)(val.length(), len));
	}
}
template<class T, std::size_t S, typename A>
void set(const std::array<T, S> &val, A &data, size_t idx, size_t len)
{
	memcpy(&data[idx], &val[0], len);
}
template <typename T, typename A>
void setDx(const typename T::type &val, A &data, size_t idxShift = 0)
{
	size_t requiredSize = T::idx + idxShift + T::len;
	if (data.size() < requiredSize)
	{
		data.resize(requiredSize);
	}
	set(val, data, T::idx + idxShift, T::len);
}

template <typename T>
inline constexpr size_t tail()
{
	return T::idx + T::len;
}



struct ErrorMsgs
{
	std::uint16_t err_c;
	const char *msg;
};

template<std::size_t size_>
const char *translateErrorCode(std::uint16_t err_c, const std::array<ErrorMsgs, size_> &errorMsgs)
{
	for (auto &it : errorMsgs)
	{
		if (it.err_c == err_c)
		{

		}
	}
	return "Error message is not defined!";
}
#pragma once

#include <KIR\AL\KIR5_event_engine.h>
#include <KIR/KIR5_stream.h>
#include <KIR/KIR4_algorithm.h>
#include <KIR/KIR4_time.h>

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

#include <fstream>
#include <nlohmann/json.hpp>
using Json = nlohmann::json;

template <template <typename...> class base, typename derived>
struct is_base_of_template_impl
{
	template<typename... Ts>
	static constexpr std::true_type  testValue(const base<Ts...> *);
	static constexpr std::false_type testValue(...);
	using value = decltype(testValue(std::declval<derived *>()));

	template<typename... Ts>
	static constexpr base<Ts...> testType(const base<Ts...> *);
	static constexpr derived testType(...);
	using type = decltype(testType(std::declval<derived *>()));
};

template <template <typename...> class base, typename derived>
using is_base_of_template = typename is_base_of_template_impl<base, derived>::value;

template <template <typename...> class base, typename derived>
using type_of_base_template = typename is_base_of_template_impl<base, derived>::type;


template <typename derived, template <typename...> class... base>
struct is_base_of_templates_impl;

template <typename derived, template <typename...> class base>
struct is_base_of_templates_impl<derived, base>
{
	using type = type_of_base_template<base, derived>;
};
template <typename derived, template <typename...> class base, template <typename...>class... _rest>
struct is_base_of_templates_impl<derived, base, _rest...>
{
	using type = typename type_of_base_template<base, typename is_base_of_templates_impl<derived, _rest...>::type>;
};

template <typename derived, template <typename...>class... base>
using type_of_base_templates = typename is_base_of_templates_impl<derived, base...>::type;


extern std::shared_ptr<KIR5::Display> display;
extern std::shared_ptr<KIR5::EventEngine> eventEngine;

extern std::random_device rd;
extern std::default_random_engine generator;
extern int processRet;

constexpr float CPS = KIR5_60_CPS;
constexpr float CA = 1 / CPS;
constexpr int blockSizeInPixel = 64;


template<typename O>
inline void concatenate_(std::ostringstream &os, const O &o)
{
	os << o;
}

template<typename O, typename ...T>
inline void concatenate_(std::ostringstream &os, const O &o, const T... t)
{
	os << o;
	concatenate_(os, t...);
}

template<typename ...T>
inline std::string concatenate(T... t)
{
	std::ostringstream os;
	concatenate_(os, t...);
	return os.str();
}


inline bool endingWith(const std::string &data, const std::string &ending)
{
	if (ending.size() > data.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), data.rbegin());
}

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
std::string dayStopper(T value)
{
	std::time_t ms = value % 1000;

	value /= 1000;
	std::time_t sec = value % 60;

	value /= 60;
	std::time_t min = value % 60;

	value /= 60;
	std::time_t hour = value % 24;

	value /= 24;
	std::time_t day = value;

	char buffer[32]{};
	sprintf_s(buffer, "%03lld %02lld:%02lld:%02lld.%lld", day, hour, min, sec, ms);
	return buffer;
}

template <typename T>
std::string hourStopper(T value)
{
	std::time_t ms = value % 1000;

	value /= 1000;
	std::time_t sec = value % 60;

	value /= 60;
	std::time_t min = value % 60;

	value /= 60;
	std::time_t hour = value;

	char buffer[32]{};
	sprintf_s(buffer, "%02lld:%02lld:%02lld.%lld", hour, min, sec, ms);

	return buffer;
}

template <typename T>
constexpr T &reach(std::shared_ptr<T> &map)
{
	return *map;
}

template <typename T>
constexpr const T &reach(const std::shared_ptr<T> &map)
{
	return *map;
}

template <typename T>
constexpr T CPStime(T _value, T _cps, T _base)
{
	return _value * (_base / _cps);
}

template <typename T, typename F>
constexpr T ifSync(T &original, const F &checkVal)
{
	if (original != static_cast<T>(checkVal))
	{
		original = static_cast<T>(checkVal);
		return true;
	}
	return false;
}


template <typename T>
constexpr T limiter(const T &l, const T &h, const T &v)
{
	return (std::max)(l, (std::min)(h, v));
}
template <typename T>
constexpr T range(const T &l, const T &h, const T &v)
{
	return (std::max)(l, (std::min)(h, v));
}


bool WriteFile(const std::string &filename, const std::uint8_t *src, std::size_t size);
bool WriteFile(const std::string &filename, const std::vector<unsigned char> &buffer);
bool WriteFile(const std::string &filename, const KIR5::DynamicStream &stream);

bool ReadFile(const std::string &filename, std::vector<unsigned char> &buffer);
bool ReadFile(const std::string &filename, KIR5::DynamicStream &stream);


extern std::string uuid();
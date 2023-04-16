#pragma once

#include <array>
#include <string>
#include <functional>
#include <tuple>
#include <map>

#include <KIR/KIR5_stream.h>
#include <KIR/hash/KIR5_sha512.h>
#include <KIR/sys/KIR5_system.h>

#include "Types.h"
#include "Hash.h"
#include "Crypto.h"

namespace Res
{
	template<class... T>
	class Record: public std::tuple<T...>
	{
		public: using std::tuple<T...>::tuple;
	};
	template<class... T>
	class HashRecord: public std::tuple<T...>
	{
		public: KIR5::sha512digest hash{};
		public: using std::tuple<T...>::tuple;
	};

	template<typename I, class T>
	struct VectorRecord: std::vector<T>
	{
		using std::vector<T>::vector;
	};
	template<typename I, class T>
	struct MatrixRecord: std::vector<std::vector<T>>
	{
		using std::vector<std::vector<T>>::vector;
		using std::vector<std::vector<T>>::operator[];

		inline T &operator[](const Type::Coord &coord)
		{
			return (*this)[coord.x][coord.y];
		}
		inline const T &operator[](const Type::Coord &coord) const
		{
			return (*this)[coord.x][coord.y];
		}
		inline void resize(Type::Size size)
		{
			std::vector<std::vector<T>>::resize(size.width);
			for (auto &it : *this)
			{
				it.resize(size.height);
			}
		}
		public: inline operator Type::Size() const
		{
			return {
				static_cast<Type::Size::base>(size()),
				static_cast<Type::Size::base>(size() == 0 ? 0 : (*this)[0].size())
			};
		}
	};

	template<class T, std::size_t COUNT>
	struct FixedVectorRecord: std::array<T, COUNT>
	{
		using std::array<T, COUNT>::array;
	};
	template<class T, std::size_t COUNT>
	struct HeapVectorRecord: std::vector<T>
	{
		using std::vector<T>::operator[];

		HeapVectorRecord()
		{
			resize(COUNT);
		}
	};
	template<class T, std::size_t WIDTH, std::size_t HEIGHT>
	struct FixedMatrixRecord: std::array<std::array<T, HEIGHT>, WIDTH>
	{
		using std::array<std::array<T, HEIGHT>, WIDTH>::array;
		using std::array<std::array<T, HEIGHT>, WIDTH>::operator[];

		inline T &operator[](const Type::Coord &coord)
		{
			return (*this)[coord.x][coord.y];
		}
		inline const T &operator[](const Type::Coord &coord) const
		{
			return (*this)[coord.x][coord.y];
		}
		public: inline operator Type::Size() const
		{
			return {
				static_cast<Type::Size::base>(size()),
				static_cast<Type::Size::base>(size() == 0 ? 0 : (*this)[0].size())
			};
		}
	};



	template<class T>
	struct EofVectorRecord: std::vector<T>
	{
		using std::vector<T>::vector;
	};

	template<std::size_t COUNT>
	class TerminatedStringRecord: public std::string
	{
		public: using std::string::string;
		public: using std::string::operator=;
		public: inline operator std::string &()
		{
			return *this;
		}
		public: inline operator const std::string &() const
		{
			return *this;
		}
	};
	template<std::size_t COUNT>
	class FixedStringRecord: public std::string
	{
		public: using std::string::string;
		public: using std::string::operator=;
		public: inline operator std::string &()
		{
			return *this;
		}
		public: inline operator const std::string &() const
		{
			return *this;
		}
	};
}

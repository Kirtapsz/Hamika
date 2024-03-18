#pragma once

#include <array>
#include <string>
#include <functional>
#include <tuple>
#include <map>

#include <KIR/hash/KIR5_sha512.h>
#include <KIR/sys/KIR5_system.h>
#include <KIR/stream/KIR5_dynamic_stream.h>
#include <KIR/stream/KIR5_stream_io.h>
#include <KIR/stream/KIR5_stream_records.h>

#include "Types.h"
#include "Hash.h"
#include "Crypto.h"
#include "Tools.h"

namespace Res
{
	enum record_t: KIR5::StreamRecords::RecordType
	{
		HashRecord_t = KIR5::StreamRecords::record_t::Reserved,
		MatrixRecord_t,
		JsonRecord_t,
	};

	template<class... T>
	struct HashRecord: public KIR5::StreamRecords::Record<T...>
	{
		constexpr static KIR5::StreamRecords::RecordType type{record_t::HashRecord_t};

		KIR5::sha512digest hash{};
		using KIR5::StreamRecords::Record<T...>::Record;

		template<typename S>
		static inline void read(KIR5::StreamReader<S> &reader, HashRecord<T...> &t)
		{
			std::apply([&reader, &t](auto&&... args)
			{
				auto &stream = reader.stream();
				std::size_t startIdx = stream.data() - stream.raw();
				(KIR5::StreamRecordsIO::read(reader, args), ...);
				std::size_t endIdx = stream.data() - stream.raw();
				KIR5::sha512(t.hash, stream.raw() + startIdx, endIdx - startIdx);
			}, static_cast<std::tuple<T...> &>(t)
				);
		}
	};

	template<typename T_SIZE, class T>
	struct MatrixRecord: public KIR5::StreamRecords::Matrix<T_SIZE, T>
	{
		constexpr static KIR5::StreamRecords::RecordType type{record_t::MatrixRecord_t};

		using KIR5::StreamRecords::Matrix<T_SIZE, T>::Matrix;
		using KIR5::StreamRecords::Matrix<T_SIZE, T>::operator[];

		inline T &operator[](const Type::Coord &coord)
		{
			return (*this)[coord.x()][coord.y()];
		}
		inline const T &operator[](const Type::Coord &coord) const
		{
			return (*this)[coord.x()][coord.y()];
		}
		inline void resize(Type::Size size)
		{
			KIR5::StreamRecords::Matrix<T_SIZE, T>::resize(size.width());
			for (auto &it : *this)
			{
				it.resize(size.height());
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

	template<class T, std::size_t WIDTH, std::size_t HEIGHT>
	struct FixMatrixRecord: public KIR5::StreamRecords::FixMatrix< WIDTH, HEIGHT, T>
	{
		constexpr static KIR5::StreamRecords::RecordType type{KIR5::StreamRecords::record_t::FixMatrix_t};

		using KIR5::StreamRecords::FixMatrix< WIDTH, HEIGHT, T>::FixMatrix;
		using KIR5::StreamRecords::FixMatrix< WIDTH, HEIGHT, T>::operator[];

		inline T &operator[](const Type::Coord &coord)
		{
			return (*this)[coord.x()][coord.y()];
		}
		inline const T &operator[](const Type::Coord &coord) const
		{
			return (*this)[coord.x()][coord.y()];
		}
		public: inline operator Type::Size() const
		{
			return {
				static_cast<Type::Size::base>(size()),
				static_cast<Type::Size::base>(size() == 0 ? 0 : (*this)[0].size())
			};
		}
	};

	struct JsonRecord: public KIR5::StreamRecords::Base, public ::Json
	{
		constexpr static KIR5::StreamRecords::RecordType type{record_t::JsonRecord_t};

		using ::Json::Json;
		using ::Json::operator[];
		using ::Json::operator=;

		template<typename S>
		static inline void read(KIR5::StreamReader<S> &reader, JsonRecord &t)
		{
			std::string str;
			reader.pop(str);
			t = ::Json::parse(str);
		}

		template<typename S>
		static inline void write(KIR5::StreamWriter<S> &writer, const JsonRecord &t)
		{
			std::ostringstream o;
			o << t;
			writer.push(o.str());
		}
		static inline std::size_t scount(const JsonRecord &t)
		{
			std::ostringstream o;
			o << t;
			return KIR5::StreamCounter::scount(o.str());
		}
	};
}

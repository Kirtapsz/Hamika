#ifndef RES_HANDLER_HPP
#define RES_HANDLER_HPP

template < template <typename...> class base, typename derived>
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

template < template <typename...> class base, typename derived>
using is_base_of_template = typename is_base_of_template_impl<base, derived>::value;

template < template <typename...> class base, typename derived>
using type_of_base_template = typename is_base_of_template_impl<base, derived>::type;

namespace Res
{
	template<typename T>
	inline void ReadFromStream_(KIR5::Stream &stream, T &t)
	{
		stream.pop<T>(t);
	}
	template<typename T>
	inline void WriteToStream_(KIR5::Stream &stream, const T &t)
	{
		stream.push<T>(t);
	}

	template<std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, TerminatedStringRecord<COUNT> &t)
	{
		std::size_t count = strnlen_s(reinterpret_cast<const char *>(stream.data()), COUNT);
		t = std::string(reinterpret_cast<const char *>(stream.data()), count);
		stream.pop(count + 1);
	}
	template<std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const TerminatedStringRecord<COUNT> &t)
	{
		std::size_t count = (std::min)(t.length(), COUNT);
		stream.push(t.data(), count);
		stream.push<std::uint8_t>(0);
	}

	template<std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, FixedStringRecord<COUNT> &t)
	{
		std::size_t count = strnlen_s(reinterpret_cast<const char *>(stream.data()), COUNT);
		t = std::string(reinterpret_cast<const char *>(stream.data()), count);
		stream.pop(COUNT);
	}
	template<std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const FixedStringRecord<COUNT> &t)
	{
		std::size_t count = (std::min)(t.length(), COUNT);
		stream.push(t.data(), count);
		if (count > 0)
		{
			stream.push<std::uint8_t>(0);
		}
		if (count > 0)
		{
			stream.push(0);
		}
	}


	template<typename T, std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, FixedVectorRecord<T, COUNT> &t)
	{
		for (auto &it : t)
		{
			ReadFromStream_(stream, static_cast<type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
		}
	}
	template<typename T, std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const FixedVectorRecord<T, COUNT> &t)
	{
		for (auto &it : t)
		{
			WriteToStream_(stream, static_cast<const type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
		}
	}

	template<typename I, typename T>
	inline void ReadFromStream_(KIR5::Stream &stream, VectorRecord<I, T> &t)
	{
		I count = 0;
		stream.pop<I>(count);
		t.resize(static_cast<std::size_t>(count));

		for (auto &it : t)
		{
			ReadFromStream_(stream, static_cast<type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
		}
	}
	template<typename I, typename T>
	inline void WriteToStream_(KIR5::Stream &stream, const VectorRecord<I, T> &t)
	{
		stream.push<I>(static_cast<I>(t.size()));

		for (auto &it : t)
		{
			WriteToStream_(stream, static_cast<const type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
		}
	}

	template<typename I, typename T>
	inline void ReadFromStream_(KIR5::Stream &stream, MatrixRecord<I, T> &t)
	{
		I width = 0;
		I height = 0;
		stream.pop<I>(width);
		stream.pop<I>(height);
		t.resize({
			static_cast<std::size_t>(width),
			static_cast<std::size_t>(height)
				 });

		for (auto &c : t)
		{
			for (auto &it : c)
			{
				ReadFromStream_(stream, static_cast<type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
			}
		}
	}
	template<typename I, typename T>
	inline void WriteToStream_(KIR5::Stream &stream, const MatrixRecord<I, T> &t)
	{
		stream.push<I>(static_cast<I>(t.size()));
		if (t.size())
		{
			stream.push<I>(static_cast<I>(t[0].size()));
		}
		else
		{
			stream.push<I>(0);
		}

		for (auto &c : t)
		{
			for (auto &it : c)
			{
				WriteToStream_(stream, static_cast<const type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
			}
		}
	}

	template<class T, std::size_t WIDTH, std::size_t HEIGHT>
	inline void ReadFromStream_(KIR5::Stream &stream, FixedMatrixRecord<T, WIDTH, HEIGHT> &t)
	{
		for (auto &c : t)
		{
			for (auto &it : c)
			{
				ReadFromStream_(stream, static_cast<type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
			}
		}
	}
	template<class T, std::size_t WIDTH, std::size_t HEIGHT>
	inline void WriteToStream_(KIR5::Stream &stream, const FixedMatrixRecord<T, WIDTH, HEIGHT> &t)
	{
		for (auto &c : t)
		{
			for (auto &it : c)
			{
				WriteToStream_(stream, static_cast<const type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
			}
		}
	}

	template<class T>
	inline void ReadFromStream_(KIR5::Stream &stream, EofVectorRecord<T> &t)
	{
		while (stream.size() > 0)
		{
			ReadFromStream_(stream, static_cast<type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(t.emplace_back()));
		}
	}
	template<class T>
	inline void WriteToStream_(KIR5::Stream &stream, const EofVectorRecord<T> &t)
	{
		for (auto &it : t)
		{
			WriteToStream_(stream, static_cast<const type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(it));
		}
	}

	template<typename ...T>
	inline void ReadFromStream_(KIR5::Stream &stream, HashRecord<T...> &t)
	{
		std::apply([&stream, &t](auto&&... args)
		{
			std::size_t startIdx = stream.data() - stream.raw();
			((ReadFromStream_(stream,
							  static_cast<type_of_base_template<Record,
							  std::remove_reference<decltype(args)>::type
							  >&>(args)
			)), ...);
			std::size_t endIdx = stream.data() - stream.raw();
			KIR5::sha512(t.hash, stream.raw() + startIdx, endIdx - startIdx);
		}, static_cast<std::tuple<T...> &>(t)
			);
	}
	template<typename ...T>
	inline void WriteToStream_(KIR5::Stream &stream, const HashRecord<T...> &t)
	{
		std::apply([&stream](auto&&... args)
		{
			((WriteToStream_(stream,
			static_cast<const type_of_base_template<Record,
			std::remove_reference<decltype(args)>::type
			>&>(args)
			)), ...);
		}, static_cast<const std::tuple<T...> &>(t)
			);
	}

	template<typename ...T>
	inline void ReadFromStream_(KIR5::Stream &stream, Record<T...> &t)
	{
		std::apply([&stream](auto&&... args)
		{
			((ReadFromStream_(stream,
			static_cast<type_of_base_template<Record,
			std::remove_reference<decltype(args)>::type
			>&>(args)
			)), ...);
		}, static_cast<std::tuple<T...> &>(t)
			);
	}
	template<typename ...T>
	inline void WriteToStream_(KIR5::Stream &stream, const Record<T...> &t)
	{
		std::apply([&stream](auto&&... args)
		{
			((WriteToStream_(stream,
			static_cast<const type_of_base_template<Record,
			std::remove_reference<decltype(args)>::type
			>&>(args)
			)), ...);
		}, static_cast<const std::tuple<T...> &>(t)
			);
	}

	template<typename T>
	inline void ReadFromStream(KIR5::DynamicStream &stream, T &t)
	{
		ReadFromStream_(stream,
						static_cast<type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(t)
		);
	}
	template<typename T>
	inline void WriteToStream(KIR5::DynamicStream &stream, const T &t)
	{
		WriteToStream_(stream,
					   static_cast<const type_of_base_template<Record, type_of_base_template<HashRecord, T>>&>(t)
		);
	}


	enum
	{
		RESOURCE_OK,
		RESOURCE_MISSMATCH,
		RESOURCE_INVALID
	};

	template<typename T, typename H>
	int LoadResource(T &resource, KIR5::DynamicStream &stream, H &handler)
	{
		if (handler.isType && !handler.isType(stream))
		{
			return RESOURCE_MISSMATCH;
		}
		if (handler.fingerprintL.size())
		{
			if (stream.size() < handler.fingerprintL.size())
			{
				return RESOURCE_MISSMATCH;
			}
			if (!std::equal(stream.begin(), stream.begin() + handler.fingerprintL.size(), handler.fingerprintL.begin()))
			{
				return RESOURCE_MISSMATCH;
			}
			stream.pop(handler.fingerprintL.size());
		}

		Crypto::descript(stream, handler.cryptoType);

		if (!Hash::check(stream, handler.hashType))
		{
			return RESOURCE_INVALID;
		}

		if (handler.fingerprintR.size())
		{
			if (stream.size() < handler.fingerprintR.size())
			{
				return RESOURCE_INVALID;
			}
			if (!std::equal(stream.begin(), stream.begin() + handler.fingerprintR.size(), handler.fingerprintR.begin()))
			{
				return RESOURCE_INVALID;
			}
			stream.pop(handler.fingerprintR.size());
		}

		H::RECORD_T record;
		ReadFromStream(stream, record);
		static_cast<T &>(resource) = record;
		return RESOURCE_OK;
	}
	template<typename T, typename H>
	int SaveResource(T &resource, KIR5::DynamicStream &stream, H &handler)
	{
		KIR5::DynamicStream streamData;
		KIR5::DynamicStream streamHashedData;

		if (handler.fingerprintL.size())
		{
			stream.push(handler.fingerprintL.data(), handler.fingerprintL.size());
		}

		if (handler.fingerprintR.size())
		{
			streamData.push(handler.fingerprintR.data(), handler.fingerprintR.size());
		}
		WriteToStream(streamData, static_cast<const H::RECORD_T &>(resource));

		Hash::engrave(streamHashedData, streamData, handler.hashType);
		streamHashedData.push(&*streamData.begin(), streamData.size());
		streamData.flush();

		Crypto::encrypt(streamHashedData, handler.cryptoType);

		stream.push(&*streamHashedData.begin(), streamHashedData.size());
		streamHashedData.flush();

		return RESOURCE_OK;
	}

	template<typename T, typename H>
	void LoadResource(T &resource, KIR5::DynamicStream &stream, H &handler, bool &done, bool &ret)
	{
		if (!done)
		{
			int _ret = LoadResource(resource, stream, handler);
			if (_ret != RESOURCE_MISSMATCH)
			{
				done = true;
			}
			if (_ret == RESOURCE_OK)
			{
				ret = true;
			}
		}
	}
	template<typename T, typename H>
	void SaveResource(T &resource, KIR5::DynamicStream &stream, H &handler, bool &done, bool &ret)
	{
		if (!done)
		{
			int _ret = SaveResource(resource, stream, handler);
			if (_ret != RESOURCE_MISSMATCH)
			{
				done = true;
			}
			if (_ret == RESOURCE_OK)
			{
				ret = true;
			}
		}
	}

	template<typename T>
	bool LoadResource(T &resource)
	{
		KIR5::DynamicStream stream;
		if (!ReadFile(resource.path, stream))
		{
			return false;
		}

		bool ret = false;
		std::apply([&resource, &stream, &ret](auto&&... args)
		{
			bool done = false;
			((LoadResource(resource, stream, args, done, ret)), ...);
		}, T::handlers);
		return ret;
	}
	template<typename T>
	bool SaveResource(T &resource)
	{
		KIR5::DynamicStream stream;

		bool ret = false;
		std::apply([&resource, &stream, &ret](auto&&... args)
		{
			bool done = false;
			((SaveResource(resource, stream, args, done, ret)), ...);
		}, T::handlers);

		return ret && WriteFile(resource.path, stream);
	}
}

#endif
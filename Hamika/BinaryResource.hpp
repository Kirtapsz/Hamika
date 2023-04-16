#ifndef RES_HANDLER_HPP
#define RES_HANDLER_HPP

#include <exception>
#include "Tools.h"

namespace Res
{
	namespace Exception
	{
		struct StreamIO: public std::exception
		{
			const char *what() const throw ()
			{
				return "Failed to read / write stream.";
			}
		};
		struct Missmatch: public std::exception
		{
			const char *what() const throw ()
			{
				return "File type is invalid.";
			}
		};
		struct FileIO: public std::exception
		{
			const char *what() const throw ()
			{
				return "File cannot be accessed.";
			}
		};
		struct Corrupted: public std::exception
		{
			const char *what() const throw ()
			{
				return "Content of the file is corrupted.";
			}
		};
		struct MissingHandler: public std::exception
		{
			const char *what() const throw ()
			{
				return "File type is invalid.";
			}
		};
	}

	template<typename C, typename T>
	inline void ReadFromStream_(KIR5::Stream &stream, T &t)
	{
		stream.pop<T>(t);
	}
	template<typename C, typename T>
	inline void WriteToStream_(KIR5::Stream &stream, const T &t)
	{
		stream.push<T>(t);
	}

	template<typename C>
	inline void ReadFromStream_(KIR5::Stream &stream, ::Json &t)
	{
		std::string str;
		stream.pop<std::string>(str);
		t = ::Json::parse(str);
	}
	template<typename C>
	inline void WriteToStream_(KIR5::Stream &stream, const ::Json &t)
	{
		std::ostringstream o;
		o << t;
		stream.push<std::string>(o.str());
	}

	template<typename C, std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, TerminatedStringRecord<COUNT> &t)
	{
		std::size_t count = strnlen_s(reinterpret_cast<const char *>(stream.data()), COUNT);
		t = std::string(reinterpret_cast<const char *>(stream.data()), count);
		stream.popSkip(count + 1);
	}
	template<typename C, std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const TerminatedStringRecord<COUNT> &t)
	{
		std::size_t count = (std::min)(t.length(), COUNT);
		stream.push(t.data(), count);
		stream.push<std::uint8_t>(0);
	}

	template<typename C, std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, FixedStringRecord<COUNT> &t)
	{
		std::size_t count = strnlen_s(reinterpret_cast<const char *>(stream.data()), COUNT);
		t = std::string(reinterpret_cast<const char *>(stream.data()), count);
		stream.popSkip(COUNT);
	}
	template<typename C, std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const FixedStringRecord<COUNT> &t)
	{
		std::size_t count = (std::min)(t.length(), COUNT);
		stream.push(t.data(), count);
		if (COUNT - count > 0)
		{
			stream.pushSkip(COUNT - count);
		}
	}


	template<typename C, typename T, std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, FixedVectorRecord<T, COUNT> &t)
	{
		for (auto &it : t)
		{
			ReadFromStream(stream, it);
		}
	}
	template<typename C, typename T, std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const FixedVectorRecord<T, COUNT> &t)
	{
		for (auto &it : t)
		{
			WriteToStream(stream, it);
		}
	}

	template<typename C, typename T, std::size_t COUNT>
	inline void ReadFromStream_(KIR5::Stream &stream, HeapVectorRecord<T, COUNT> &t)
	{
		for (auto &it : t)
		{
			ReadFromStream(stream, it);
		}
	}
	template<typename C, typename T, std::size_t COUNT>
	inline void WriteToStream_(KIR5::Stream &stream, const HeapVectorRecord<T, COUNT> &t)
	{
		for (auto &it : t)
		{
			WriteToStream(stream, it);
		}
	}

	template <typename C, class BASE, typename... Args> struct TypeList
	{
		static constexpr BASE *ReadFromStream__(KIR5::Stream &stream)
		{
			std::int8_t type = 0;
			stream.pop<std::int8_t>(type);
			return ReadFromStream__<Args...>(stream, type);
		}

		template <typename T>
		static constexpr BASE *ReadFromStream__(KIR5::Stream &stream, std::int8_t &type)
		{
			if (type == 0)
			{
				T *t = new T();
				ReadFromStream(stream, *t);
				return t;
			}
			else
			{
				--type;
			}
			return nullptr;
		}

		template <typename T, typename N, typename... Args>
		static constexpr BASE *ReadFromStream__(KIR5::Stream &stream, std::int8_t &type)
		{
			BASE *base = ReadFromStream__<T>(stream, type);
			if (base == nullptr)
			{
				base = ReadFromStream__<N, Args...>(stream, type);
			}
			return base;
		}


		static constexpr void WriteToStream__(KIR5::Stream &stream, BASE *base)
		{
			std::int8_t type = 0;
			WriteToStream__<Args...>(stream, type, base);
		}

		template <typename T>
		static constexpr void *WriteToStream__(KIR5::Stream &stream, std::int8_t &type, BASE *base)
		{
			T *t = dynamic_cast<T *>(base);
			if (t != nullptr)
			{
				stream.push<std::int8_t>(type);
				WriteToStream(stream, *t);
			}
			else
			{
				++type;
			}
			return t;
		}

		template <typename T, typename N, typename... Args>
		static constexpr void WriteToStream__(KIR5::Stream &stream, std::int8_t &type, BASE *base)
		{
			if (WriteToStream__<T>(stream, type, base) == nullptr)
			{
				WriteToStream__<N, Args...>(stream, type, base);
			}
		}
	};

	template<typename C, class BASE, class I, class... T>
	inline void ReadFromStream_(KIR5::Stream &stream, SwitchVectorRecord<BASE, I, T...> &t)
	{
		I count = 0;
		stream.pop<I>(count);
		t.resize(static_cast<std::size_t>(count));

		for (auto &it : t)
		{
			it = std::shared_ptr<BASE>(TypeList<C, BASE, T...>::ReadFromStream__(stream));
		}

	}
	template<typename C, class BASE, class I, class... T>
	inline void WriteToStream_(KIR5::Stream &stream, const SwitchVectorRecord<BASE, I, T...> &t)
	{
		stream.push<I>(static_cast<I>(t.size()));

		for (auto &it : t)
		{
			TypeList<C, BASE, T...>::WriteToStream__(stream, it.get());
		}
	}

	template<typename C, typename I, typename T>
	inline void ReadFromStream_(KIR5::Stream &stream, VectorRecord<I, T> &t)
	{
		I count = 0;
		stream.pop<I>(count);
		t.resize(static_cast<std::size_t>(count));

		for (auto &it : t)
		{
			ReadFromStream(stream, it);
		}
	}
	template<typename C, typename I, typename T>
	inline void WriteToStream_(KIR5::Stream &stream, const VectorRecord<I, T> &t)
	{
		stream.push<I>(static_cast<I>(t.size()));

		for (auto &it : t)
		{
			WriteToStream(stream, it);
		}
	}

	template<typename C, typename I, typename T>
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
				ReadFromStream(stream, it);
			}
		}
	}
	template<typename C, typename I, typename T>
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
				WriteToStream(stream, it);
			}
		}
	}

	template<typename C, class T, std::size_t WIDTH, std::size_t HEIGHT>
	inline void ReadFromStream_(KIR5::Stream &stream, FixedMatrixRecord<T, WIDTH, HEIGHT> &t)
	{
		for (auto &c : t)
		{
			for (auto &it : c)
			{
				ReadFromStream(stream, it);
			}
		}
	}
	template<typename C, class T, std::size_t WIDTH, std::size_t HEIGHT>
	inline void WriteToStream_(KIR5::Stream &stream, const FixedMatrixRecord<T, WIDTH, HEIGHT> &t)
	{
		for (auto &c : t)
		{
			for (auto &it : c)
			{
				WriteToStream(stream, it);
			}
		}
	}

	template<typename C, class T>
	inline void ReadFromStream_(KIR5::Stream &stream, EofVectorRecord<T> &t)
	{
		while (stream.size() > 0)
		{
			ReadFromStream(stream, t.emplace_back());
		}
	}
	template<typename C, class T>
	inline void WriteToStream_(KIR5::Stream &stream, const EofVectorRecord<T> &t)
	{
		for (auto &it : t)
		{
			WriteToStream(stream, it);
		}
	}

	template<typename C, typename ...T>
	inline void ReadFromStream_(KIR5::Stream &stream, HashRecord<T...> &t)
	{
		std::apply([&stream, &t](auto&&... args)
		{
			std::size_t startIdx = stream.data() - stream.raw();
			(ReadFromStream(stream, args), ...);
			std::size_t endIdx = stream.data() - stream.raw();
			KIR5::sha512(t.hash, stream.raw() + startIdx, endIdx - startIdx);
		}, static_cast<std::tuple<T...> &>(t)
			);
	}
	template<typename C, typename ...T>
	inline void WriteToStream_(KIR5::Stream &stream, const HashRecord<T...> &t)
	{
		std::apply([&stream](auto&&... args)
		{
			(WriteToStream(stream, args), ...);
		}, static_cast<const std::tuple<T...> &>(t)
			);
	}

	template<typename C, typename ...T>
	inline void ReadFromStream_(KIR5::Stream &stream, Record<T...> &t)
	{
		std::apply([&stream](auto&&... args)
		{
			(ReadFromStream(stream, args), ...);
		}, static_cast<std::tuple<T...> &>(t)
			);
	}
	template<typename C, typename ...T>
	inline void WriteToStream_(KIR5::Stream &stream, const Record<T...> &t)
	{
		std::apply([&stream](auto&&... args)
		{
			(WriteToStream(stream, args), ...);
		}, static_cast<const std::tuple<T...> &>(t)
			);
	}

	template<typename C>
	inline void ReadFromStream(KIR5::Stream &stream, C &t)
	{
		ReadFromStream_<C>(stream, static_cast<record_cast<C>&>(t));
	}
	template<typename C>
	inline void WriteToStream(KIR5::Stream &stream, const C &t)
	{
		WriteToStream_<C>(stream, static_cast<const record_cast<C>&>(t));
	}



	template<typename T, typename H>
	void LoadResource(T &resource, KIR5::DynamicStream &stream, H &handler)
	{
		if (handler.isType && !handler.isType(stream))
		{
			throw Exception::Missmatch();
		}
		if (handler.fingerprintL.size())
		{
			if (stream.size() < handler.fingerprintL.size())
			{
				throw Exception::Missmatch();
			}
			if (!std::equal(stream.begin(), stream.begin() + handler.fingerprintL.size(), handler.fingerprintL.begin()))
			{
				throw Exception::Missmatch();
			}
			stream.popSkip(handler.fingerprintL.size());
		}

		Crypto::descript(stream, handler.cryptoType);

		if (!Hash::check(stream, handler.hashType))
		{
			throw Exception::Corrupted();
		}

		if (handler.fingerprintR.size())
		{
			if (stream.size() < handler.fingerprintR.size())
			{
				throw Exception::Corrupted();
			}
			if (!std::equal(stream.begin(), stream.begin() + handler.fingerprintR.size(), handler.fingerprintR.begin()))
			{
				throw Exception::Corrupted();
			}
			stream.popSkip(handler.fingerprintR.size());
		}

		H::RECORD_T record;
		ReadFromStream(stream, record);
		static_cast<T &>(resource) = record;
	}
	template<typename T, typename H>
	void SaveResource(T &resource, KIR5::DynamicStream &stream, H &handler)
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
	}


	template<typename T, typename H>
	void SaveResource(T &resource, KIR5::DynamicStream &stream, H &handler, bool &done)
	{
		if (!done)
		{
			try
			{
				SaveResource(resource, stream, handler);
				done = true;
			}
			catch (const Exception::Missmatch &)
			{

			}
		}
	}


	template<typename T, typename H>
	void LoadResource(T &resource, KIR5::DynamicStream &stream, H &handler, bool &done)
	{
		if (!done)
		{
			try
			{
				LoadResource(resource, stream, handler);
				done = true;
			}
			catch (const Exception::Missmatch &)
			{
			}
		}
	}

	template<typename T>
	void LoadResource_(T &resource)
	{
		KIR5::DynamicStream stream;
		if (ReadFile(resource.path, stream))
		{
			std::apply([&resource, &stream](auto&&... args)
			{
				bool done = false;
				((LoadResource(resource, stream, args, done)), ...);
				if (!done)
				{
					throw Exception::MissingHandler();
				}
			}, T::handlers);
		}
		else
		{
			if ((resource.mode & Base::FILE_MISSING_ALLOWED) == 0)
			{
				throw Exception::FileIO();
			}
		}
	}
	template<typename T>
	void SaveResource_(T &resource)
	{
		KIR5::DynamicStream stream;

		std::apply([&resource, &stream](auto&&... args)
		{
			bool done = false;
			((SaveResource(resource, stream, args, done)), ...);
			if (!done)
			{
				throw Exception::MissingHandler();
			}
		}, T::handlers);

		if (!WriteFile(resource.path, stream))
		{
			throw Exception::FileIO();
		}
	}
}

#endif
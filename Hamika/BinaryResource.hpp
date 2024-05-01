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
			stream.skip(handler.fingerprintL.size());
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
			stream.skip(handler.fingerprintR.size());
		}

		H::RECORD_T record;
		KIR5::DynamicStream::Reader reader(stream);
		KIR5::StreamReader stream_reader(reader);
		KIR5::StreamRecordsIO::read(stream_reader, record);
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

		KIR5::DynamicStream::Writer writer(streamData);
		KIR5::StreamWriter stream_writer(writer);
		KIR5::StreamRecordsIO::write(stream_writer, static_cast<const H::RECORD_T &>(resource));

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
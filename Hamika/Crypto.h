#pragma once

#include <KIR/stream/KIR5_static_stream.h>

namespace Res
{
	namespace Crypto
	{
		enum Type
		{
			none,
			V1_1024,
		};


		void encrypt(KIR5::StaticStream &stream, Type type);
		inline void descript(KIR5::StaticStream &stream, Type type)
		{
			encrypt(stream, type);
		}
	}
}
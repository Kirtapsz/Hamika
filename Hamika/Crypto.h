#pragma once

#include <KIR/KIR5_stream.h>

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
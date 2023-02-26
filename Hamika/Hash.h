#pragma once

#include <vector>

#include <KIR/KIR5_stream.h>

namespace Res
{
	namespace Hash
	{
		enum Type
		{
			none,
			sha512_simple1,
			sha512_salty1,
		};

		void engrave(KIR5::StaticStream &streamDst, KIR5::StaticStream &streamSrc, Type type);
		bool check(KIR5::StaticStream &stream, Type type);
	}
}
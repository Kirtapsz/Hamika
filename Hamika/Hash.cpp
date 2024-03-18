#include "Hash.h"

#include <KIR/hash/KIR5_sha512.h>
#include <KIR/stream/KIR5_stream.h>

#include <functional>
#include <array>

namespace Res
{
	namespace Hash
	{
		void sha512_simple1_hash(std::uint8_t *dst, const std::uint8_t *src, std::size_t size)
		{
			KIR5::SHA512 ctx;
			ctx.init();
			ctx.update(src, size);
			ctx.final(dst);
		}
		void sha512_simple1_engrave(KIR5::StaticStream &streamDst, KIR5::StaticStream &streamSrc)
		{
			std::vector<std::uint8_t> calculatedHash(KIR5::SHA512_DIGEST_SIZE);
			sha512_simple1_hash(calculatedHash.data(), &*streamSrc.begin(), streamSrc.size());
			streamDst.push(calculatedHash.data(), calculatedHash.size());
		}
		bool sha512_simple1_check(KIR5::StaticStream &stream)
		{
			std::vector<std::uint8_t> storedHash(KIR5::SHA512_DIGEST_SIZE);
			std::vector<std::uint8_t> calculatedHash(storedHash.size());
			stream.pop(storedHash.data(), storedHash.size());
			sha512_simple1_hash(calculatedHash.data(), &*stream.begin(), stream.size());
			return storedHash == calculatedHash;
		}

		constexpr std::array<std::uint8_t, 16> salty1_data{0x14,0x1c,0xaf,0x29,0xa9,0xb4,0x59,0x4f,0xd8,0xc8,0x4c,0xee,0x33,0x84,0x1c,0x4f};
		void sha512_salty1_hash(std::uint8_t *dst, const std::uint8_t *src, std::size_t size)
		{
			KIR5::SHA512 ctx;
			ctx.init();
			ctx.update(salty1_data.data(), salty1_data.size());
			ctx.update(src, size);
			ctx.final(dst);
		}
		void sha512_salty1_engrave(KIR5::StaticStream &streamDst, KIR5::StaticStream &streamSrc)
		{
			std::vector<std::uint8_t> calculatedHash(KIR5::SHA512_DIGEST_SIZE);
			sha512_salty1_hash(calculatedHash.data(), &*streamSrc.begin(), streamSrc.size());
			streamDst.push(calculatedHash.data(), calculatedHash.size());
		}
		bool sha512_salty1_check(KIR5::StaticStream &stream)
		{
			std::vector<std::uint8_t> storedHash(KIR5::SHA512_DIGEST_SIZE);
			std::vector<std::uint8_t> calculatedHash(storedHash.size());
			stream.pop(storedHash.data(), storedHash.size());
			sha512_salty1_hash(calculatedHash.data(), &*stream.begin(), stream.size());
			return storedHash == calculatedHash;
		}

		struct Algo
		{
			Type type;
			std::function<void(KIR5::StaticStream &streamDst, KIR5::StaticStream &streamSrc)> engrave;
			std::function<bool(KIR5::StaticStream &stream)> check;
		};

		std::array<Algo, 2> algos{{
			{sha512_simple1, sha512_simple1_engrave, sha512_simple1_check},
			{sha512_salty1, sha512_salty1_engrave, sha512_salty1_check}
		}};

		Algo *findAlgo(Type type)
		{
			for (auto &algo : algos)
			{
				if (algo.type == type)
				{
					return &algo;
				}
			}
			return nullptr;
		}

		void engrave(KIR5::StaticStream &streamDst, KIR5::StaticStream &streamSrc, Type type)
		{
			Algo *algo = findAlgo(type);
			if (algo)
			{
				algo->engrave(streamDst, streamSrc);
			}
		}
		bool check(KIR5::StaticStream &stream, Type type)
		{
			Algo *algo = findAlgo(type);
			if (algo)
			{
				return algo->check(stream);
			}
			return true;
		}
	}
}
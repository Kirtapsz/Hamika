#pragma once

#include "World.h"

namespace Res
{
	struct Worlds: public Base, public std::vector<std::shared_ptr<World>>
	{
		using Base::Base;

		public: virtual bool initialize(std::uint32_t mode);
		public: std::shared_ptr<BluePrint> find(const KIR5::sha512digest &hash_) const;
	};

	extern Worlds worlds;
	extern Worlds testWorlds;

	std::shared_ptr<BluePrint> findBluePrint(const KIR5::sha512digest &hash_);
}
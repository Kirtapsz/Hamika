#pragma once

#include "World.h"

namespace Res
{
	struct Worlds: public Base, public std::vector<std::shared_ptr<World>>
	{
		using Base::Base;

		public: virtual bool initialize(std::uint32_t mode);
	};


	extern Worlds worlds;
}
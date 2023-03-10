#include "Worlds.h"

#include <KIR/sys/KIR5_files.h>

namespace Res
{
	bool Worlds::initialize(std::uint32_t mode)
	{
		KIR5::FindFiles<> ff(path, "\\*.dat");
		for (auto &it : ff)
		{
			KIR5::Shared<World> world(KIR5::pathCombine(path, it.getName()));
			if (!world->thumbnail.load(KIR5::pathCombine(path, it.getNameOnly()) + ".png"))
			{
				return false;
			}
			if (!world->initialize(mode))
			{
				return false;
			}
			push_back(world);
		}
		return true;
	}

	Worlds worlds{"Hamika\\worlds"};
}
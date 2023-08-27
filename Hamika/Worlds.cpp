#include "Worlds.h"

#include <KIR/sys/KIR5_files.h>

namespace Res
{
	bool Worlds::initialize(std::uint32_t mode)
	{
		KIR5::FindFiles<> ff(path, "*.*");
		for (auto &it : ff)
		{
			if (it.getExtension() == "dat" || it.getExtension() == "json")
			{
				KIR5::Shared<World> world(KIR5::pathCombine(path, it.getName()));
				if (!world->thumbnail.load(KIR5::pathCombine(path, it.getNameOnly()) + ".png"))
				{
					if (!(mode & ITEST || mode & TEST))
					{
						return false;
					}
				}
				if (!world->initialize(mode))
				{
					return false;
				}
				push_back(world);
			}
		}
		return true;
	}

	std::shared_ptr<BluePrint> Worlds::find(const KIR5::sha512digest &hash_) const
	{
		for (auto &world : *this)
		{
			for (auto &bluePrint : world->bluePrints)
			{
				if (bluePrint->hash == hash_)
				{
					return bluePrint;
				}
			}
		}
		return nullptr;
	}

	void Worlds::print() const
	{
		unsigned count = 0;
		for (auto &world : *this)
		{
			clog << count++ << ". =========" << KIR4::eol;
			world->print();
		}
	}

	Worlds worlds{"Hamika\\worlds"};
	Worlds testWorlds{"Hamika\\multitest\\worlds"};


	std::shared_ptr<BluePrint> findBluePrint(const KIR5::sha512digest &hash_)
	{
		std::shared_ptr<BluePrint> bluePrint = worlds.find(hash_);
		if (!bluePrint)
		{
			bluePrint = testWorlds.find(hash_);
		}
		return bluePrint;
	}
}
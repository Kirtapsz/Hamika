#pragma once

#include "World.h"

namespace Res
{
	void World::operator=(const World::IO_HamSt1 &record)
	{
		bluePrints.clear();
		title = std::get<IO_HamSt1::title>(record);
		for (auto &it : std::get<IO_HamSt1::blueprints>(record))
		{
			bluePrints.push_back(std::shared_ptr<BluePrint>(new BluePrint(it)));
		}
	}
	World::operator World::IO_HamSt1() const
	{
		IO_HamSt1 record;
		std::get<IO_HamSt1::title>(record) = title;
		for (auto &it : bluePrints)
		{
			std::get<IO_HamSt1::blueprints>(record).push_back(*it);
		}
		return record;
	}

	void World::operator=(const World::IO_HamBC &record)
	{
		bluePrints.clear();
		for (auto &it : std::get<IO_HamBC::blueprints>(record))
		{
			bluePrints.push_back(std::shared_ptr<BluePrint>(new BluePrint(it)));
		}
	}
	World::operator World::IO_HamBC() const
	{
		IO_HamBC record;
		for (auto &it : bluePrints)
		{
			std::get<IO_HamBC::blueprints>(record).push_back(*it);
		}
		return record;
	}


	void World::operator=(const World::IO_Orig &record)
	{
		bluePrints.clear();
		for (auto &it : std::get<IO_Orig::blueprints>(record))
		{
			bluePrints.push_back(std::shared_ptr<BluePrint>(new BluePrint(it)));
		}
	}
	World::operator World::IO_Orig() const
	{
		IO_Orig record;
		for (auto &it : bluePrints)
		{
			std::get<IO_Orig::blueprints>(record).push_back(*it);
		}
		return record;
	}

	const std::tuple<
		World::_Handler<World::IO_HamSt1>,
		World::_Handler<World::IO_Orig>,
		World::_Handler<World::IO_HamBC>
	>
		World::handlers{
			{true, nullptr, {0x48, 0x61, 0x6D, 0x53, 0x74, 0x2E, 0x2E, 0x31}, {0x48, 0x61, 0x6D, 0x53, 0x74, 0x2E, 0x2E, 0x31}, Hash::sha512_salty1, Crypto::V1_1024, "Star one"},

			{true, 	[](const KIR5::DynamicStream &stream) -> bool
	{
	return stream.size() % BluePrint::IO_Orig::MAP_SIZE == 0;
	   } , {}, {}, Hash::none, Crypto::none, "Back to 91s"},

			{true, nullptr, {}, {}, Hash::none, Crypto::none, "Beta version"},
	};


	bool World::initialize(std::uint32_t mode)
	{
		return LoadResource(*this);
	}

	World world1{"Hamika\\LEVELS.DAT"};
}
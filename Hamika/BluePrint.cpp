#include "BluePrint.h"

namespace Res
{
	BluePrint::BluePrint(const BluePrint::IO_HamSt1 &record)
	{
		hash = record.hash;

		speed = std::get<IO_HamSt1::speed>(record);
		title = std::get<IO_HamSt1::title>(record);
		scoreToUnlock = std::get<IO_HamSt1::scoreToUnlock>(record);
		globalGravity = std::get<IO_HamSt1::globalGravity>(record);
		cameraSize = {
			std::get<IO_HamSt1::cameraWidth>(record),
			std::get<IO_HamSt1::cameraHeight>(record)};
		auto &blockRecords = std::get<IO_HamSt1::blocks>(record);

		blocks.resize(blockRecords);
		blocks.foreach([&blockRecords](Type::Coord &coord, Block &block)
		{
			block.id = std::get<IO_HamSt1_Block::id>(blockRecords[coord]);
			block.rotation = std::get<IO_HamSt1_Block::rotation>(blockRecords[coord]);
			block.flags = std::get<IO_HamSt1_Block::flags>(blockRecords[coord]);
		});
	}
	BluePrint::operator BluePrint::IO_HamSt1() const
	{
		IO_HamSt1 record;

		std::get<IO_HamSt1::speed>(record) = speed;
		std::get<IO_HamSt1::title>(record) = title;
		std::get<IO_HamSt1::scoreToUnlock>(record) = static_cast<std::uint16_t>(scoreToUnlock);
		std::get<IO_HamSt1::globalGravity>(record) = globalGravity;
		std::get<IO_HamSt1::cameraWidth>(record) = cameraSize.width();
		std::get<IO_HamSt1::cameraHeight>(record) = cameraSize.height();
		auto &blockRecords = std::get<IO_HamSt1::blocks>(record);

		blockRecords.resize(blocks);
		blocks.foreach([&blockRecords](Type::Coord &coord, const Block &block)
		{
			std::get<IO_HamSt1_Block::id>(blockRecords[coord]) = block.id;
			std::get<IO_HamSt1_Block::rotation>(blockRecords[coord]) = block.rotation;
			std::get<IO_HamSt1_Block::flags>(blockRecords[coord]) = block.flags;
		});

		return record;
	}

	BluePrint::BluePrint(const BluePrint::IO_HamBC &record)
	{
		hash = record.hash;

		title = std::get<IO_HamBC::title>(record);
		scoreToUnlock = std::get<IO_HamBC::scoreToUnlock>(record);
		speed = std::get<IO_HamBC::speed>(record);
		cameraSize = {BluePrint::cameraSize1to1, BluePrint::cameraSize1to1};

		auto fillID = std::get<IO_HamBC::fillId>(record);

		const auto &id = std::get<IO_HamBC::id>(record);
		const auto &rotation = std::get<IO_HamBC::rotation>(record);
		const auto &explode = std::get<IO_HamBC::explode>(record);
		const auto &gravitation = std::get<IO_HamBC::gravitation>(record);
		blocks.resize({IO_HamBC::FIX_WIDTH, IO_HamBC::FIX_HEIGHT});
		blocks.foreach([&id, &rotation, &explode, &gravitation, &fillID](Type::Coord &coord, Block &block)
		{
			if (block.id == ObjectID::Murphy)
			{
				block.id = IO_HamBC::transition(fillID);
				block.flags |= GridFlags::SpawnPoint;
			}
			else
			{
				block.id = IO_HamBC::transition(id[coord]);
			}
			block.rotation = rotation[coord];
			if (explode[coord])
			{
				block.flags |= GridFlags::Detonate;
			}
			if (gravitation[coord])
			{
				block.flags |= GridFlags::Gravity;
			}
		});
	}
	BluePrint::operator BluePrint::IO_HamBC() const
	{
		IO_HamBC record;

		std::get<IO_HamBC::title>(record) = title;
		std::get<IO_HamBC::scoreToUnlock>(record) = static_cast<std::uint16_t>(scoreToUnlock);
		std::get<IO_HamBC::speed>(record) = speed;

		Type::ID fillID = ObjectID::Space;
		blocks.foreach([&fillID](Type::Coord &coord, const Block &block)
		{
			if (block.flags | GridFlags::SpawnPoint)
			{
				fillID = block.id;
				return;
			}
		});

		auto &id = std::get<IO_HamBC::id>(record);
		auto &rotation = std::get<IO_HamBC::rotation>(record);
		auto &explode = std::get<IO_HamBC::explode>(record);
		auto &gravitation = std::get<IO_HamBC::gravitation>(record);
		blocks.foreach([&id, &rotation, &explode, &gravitation, &fillID](Type::Coord &coord, const Block &block)
		{
			if (block.flags | GridFlags::SpawnPoint)
			{
				id[coord] = IO_HamBC::detransition(ObjectID::Murphy);
			}
			else
			{
				id[coord] = IO_HamBC::detransition(block.id);
			}
			rotation[coord] = block.rotation;
			if (block.flags | GridFlags::Detonate)
			{
				explode[coord] = 1;
			}
			if (block.flags | GridFlags::Gravity)
			{
				gravitation[coord] = 1;
			}
		});

		return record;
	}

	BluePrint::BluePrint(const BluePrint::IO_Orig &record)
	{
		hash = record.hash;

		title = std::get<IO_Orig::title>(record);
		scoreToUnlock = std::get<IO_Orig::scoreToUnlock>(record);
		globalGravity = std::get<IO_Orig::globalGravity>(record);

		const std::uint8_t *id = std::get<IO_Orig::id>(record).data();
		blocks.resize({IO_Orig::FIX_WIDTH, IO_Orig::FIX_HEIGHT});
		Type::Coord coord;
		for (coord.y() = 0; coord.y() < ((Type::Size)blocks).height(); ++coord.y())
		{
			for (coord.x() = 0; coord.x() < ((Type::Size)blocks).width(); ++coord.x())
			{
				if (*id == 3)
				{
					blocks[coord].flags |= GridFlags::SpawnPoint;
				}
				blocks[coord].id = IO_Orig::transition(*id++);
			}
		}
	}
	BluePrint::operator BluePrint::IO_Orig() const
	{
		IO_Orig record;

		std::get<IO_Orig::title>(record) = title;
		std::get<IO_Orig::scoreToUnlock>(record) = static_cast<std::uint8_t>(scoreToUnlock);
		std::get<IO_Orig::globalGravity>(record) = globalGravity;

		std::uint8_t *id = const_cast<std::uint8_t *>(std::get<IO_Orig::id>(record).data());
		Type::Coord coord;
		for (coord.y() = 0; coord.y() < ((Type::Size)blocks).height(); ++coord.y())
		{
			for (coord.x() = 0; coord.x() < ((Type::Size)blocks).width(); ++coord.x())
			{
				if (blocks[coord].flags & GridFlags::SpawnPoint)
				{
					*id++ = IO_Orig::SpawnID;
				}
				else
				{
					*id++ = IO_Orig::detransition(blocks[coord].id);
				}
			}
		}

		return record;
	}
}
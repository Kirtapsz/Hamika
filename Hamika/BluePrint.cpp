#include "BluePrint.h"

#include "Tools.h"

bool BluePrint::IO_HamSt1::save(std::vector<unsigned char> &buffer, const std::shared_ptr<BluePrint> &bluePrint)
{
	if (((Type::Size)bluePrint->blocks).width <= 0 ||
		((Type::Size)bluePrint->blocks).height <= 0)
	{
		return false;
	}
	buffer.resize(
		tail<_headTail>() +
		(((Type::Size)bluePrint->blocks).width * ((Type::Size)bluePrint->blocks).height) * tail<_blockDataTail>()
	);

	/* DATA */
	{
		std::size_t idx = tail<_headTail>();
		Type::Coord coord = {0};
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
			{
				setDx<_blockID>(bluePrint->blocks[coord].id, buffer, idx);
				setDx<_rotation>(bluePrint->blocks[coord].rotation, buffer, idx);
				setDx<_flags>(bluePrint->blocks[coord].flags, buffer, idx);

				idx += tail<_flags>();
			}
		}
	}

	/* HEAD */
	setDx<_scoreToUnlock>(bluePrint->scoreToUnlock, buffer);
	setDx<_globalGravity>(bluePrint->globalGravity, buffer);
	setDx<_murphySpeed>(bluePrint->speed, buffer);
	setDx<_titleOfBluePrint>(bluePrint->title, buffer);
	setDx<_width>(((Type::Size)bluePrint->blocks).width, buffer);
	setDx<_height>(((Type::Size)bluePrint->blocks).height, buffer);
	setDx<_cameraWidth>(((Type::CameraSize)bluePrint->cameraSize).width, buffer);
	setDx<_cameraHeight>(((Type::CameraSize)bluePrint->cameraSize).height, buffer);

	KIR5::sha512(bluePrint->hash, &buffer[tail<_HASH521>()], buffer.size() - tail<_HASH521>());
	setDx<_HASH521>(bluePrint->hash, buffer);

	return true;
}
bool BluePrint::IO_HamSt1::load(const std::vector<unsigned char> &buffer, const std::shared_ptr<BluePrint> &bluePrint)
{
	/* HEAD */
	{
		_scoreToUnlock::type scoreToUnlock = 0;
		getDx<_scoreToUnlock>(scoreToUnlock, buffer);
		bluePrint->scoreToUnlock = scoreToUnlock;

		_globalGravity::type globalGravity = 0;
		getDx<_globalGravity>(globalGravity, buffer);
		bluePrint->globalGravity = globalGravity;

		_murphySpeed::type speed = 0;
		getDx<_murphySpeed>(speed, buffer);
		bluePrint->speed = speed;

		_titleOfBluePrint::type title;
		getDx<_titleOfBluePrint>(title, buffer);
		bluePrint->title = title;

		_width::type width = 0;
		getDx<_width>(width, buffer);

		_height::type height = 0;
		getDx<_height>(height, buffer);

		_cameraWidth::type cameraWidth = BluePrint::cameraSize1to1;
		getDx<_cameraWidth>(cameraWidth, buffer);

		_cameraHeight::type cameraHeight = BluePrint::cameraSize1to1;
		getDx<_cameraHeight>(cameraHeight, buffer);

		bluePrint->cameraSize = {cameraWidth, cameraHeight};

		if (width <= 0 || height <= 0)
		{
			return false;
		}

		bluePrint->blocks.resize({width,height});
	}


	/* DATA */
	{
		std::size_t idx = _headTail::idx;
		Type::Coord coord = {0};
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
			{
				_blockID::type id = 0;
				_rotation::type rotation = 0;
				_flags::type flags = 0;

				getDx<_blockID>(id, buffer, idx);
				getDx<_rotation>(rotation, buffer, idx);
				getDx<_flags>(flags, buffer, idx);

				idx += _rotation::type(tail<_flags>());

				bluePrint->blocks[coord].id = id;
				bluePrint->blocks[coord].rotation = Type::Rotations::getRotationOfIndex(rotation);
				bluePrint->blocks[coord].flags = flags;
			}
		}
	}

	KIR5::sha512digest origHash;
	KIR5::sha512(bluePrint->hash, &buffer[tail<_HASH521>()], buffer.size() - tail<_HASH521>());
	getDx<_HASH521>(origHash, buffer);
	if (memcmp(bluePrint->hash, origHash, KIR5::SHA512_DIGEST_SIZE) != 0)
	{
		return false;
	}

	return true;
}

bool BluePrint::IO_HamBC::save(std::vector<unsigned char> &buffer, const std::shared_ptr<BluePrint> &bluePrint)
{
	KIR5::sha512(bluePrint->hash, buffer.data(), buffer.size());

	return false;
}
bool BluePrint::IO_HamBC::load(const std::vector<unsigned char> &buffer, const std::shared_ptr<BluePrint> &bluePrint)
{
	if (buffer.size() != MAP_SIZE)
	{
		return false;
	}

	bluePrint->blocks.resize({FIX_WIDTH,FIX_HEIGHT});

	size_t idx = 0;
	Type::Coord coord = {0};

	idx = 0;
	for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
	{
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			_blockID::type blockID;
			getDx<_blockID>(blockID, buffer, idx);
			idx += _blockID::len;
			bluePrint->blocks[coord].id = transition(blockID);
		}
	}

	idx = 0;
	for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
	{
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			_rotationOfBlock::type rotationOfBlock;
			getDx<_rotationOfBlock>(rotationOfBlock, buffer, idx);
			idx += _rotationOfBlock::len;
			bluePrint->blocks[coord].rotation = rotationOfBlock;
		}
	}

	idx = 0;
	for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
	{
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			_isExplode::type isExplode;
			getDx<_isExplode>(isExplode, buffer, idx);
			idx += _isExplode::len;
			if (isExplode)
			{
				bluePrint->blocks[coord].flags |= GridFlags::Detonate;
			}
		}
	}

	idx = 0;
	for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
	{
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			_isGravitation::type isGravitation;
			getDx<_isGravitation>(isGravitation, buffer, idx);
			idx += _isGravitation::len;
			if (isGravitation)
			{
				bluePrint->blocks[coord].flags |= GridFlags::Gravity;
			}
		}
	}

	_scoreToUnlock::type scoreToUnlock = 0;
	getDx<_scoreToUnlock>(scoreToUnlock, buffer);
	bluePrint->scoreToUnlock = scoreToUnlock;

	_murphySpeed::type murphySpeed = 0;
	getDx<_murphySpeed>(murphySpeed, buffer);
	bluePrint->speed = murphySpeed;

	_murphySpawnFillBlockID::type murphySpawnFillBlockID = 0;
	getDx<_murphySpawnFillBlockID>(murphySpawnFillBlockID, buffer);
	for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
	{
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
		{
			if (bluePrint->blocks[coord].id == ObjectID::Murphy)
			{
				bluePrint->blocks[coord].id = transition(murphySpawnFillBlockID);
				bluePrint->blocks[coord].flags |= GridFlags::SpawnPoint;
			}
		}
	}

	_titleOfBluePrint::type titleOfBluePrint;
	getDx<_titleOfBluePrint>(titleOfBluePrint, buffer);
	bluePrint->title = titleOfBluePrint;

	bluePrint->globalGravity = 0;

	bluePrint->cameraSize = {BluePrint::cameraSize1to1, BluePrint::cameraSize1to1};

	KIR5::sha512(bluePrint->hash, buffer.data(), buffer.size());

	return true;
}

bool BluePrint::IO_Orig::save(std::vector<unsigned char> &buffer, const std::shared_ptr<BluePrint> &bluePrint)
{
	buffer.resize(MAP_SIZE);

	size_t idx = 0;
	Type::Coord coord = {0};
	for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
	{
		for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
		{
			if (bluePrint->blocks[coord].flags & GridFlags::SpawnPoint)
			{
				setDx<_blockID>(3, buffer, idx);
			}
			else
			{
				setDx<_blockID>(_blockID::type(detransition(bluePrint->blocks[coord].id)), buffer, idx);
			}
			idx += _blockID::len;
		}
	}

	setDx<_titleOfBluePrint>(bluePrint->title, buffer);

	setDx<_scoreToUnlock>(bluePrint->scoreToUnlock, buffer);

	setDx<_scoreToUnlock>(bluePrint->globalGravity, buffer);

	KIR5::sha512(bluePrint->hash, buffer.data(), buffer.size());

	return true;
}
bool BluePrint::IO_Orig::load(const std::vector<unsigned char> &buffer, const std::shared_ptr<BluePrint> &bluePrint)
{
	if (buffer.size() != MAP_SIZE)
	{
		return false;
	}

	bluePrint->blocks.resize({FIX_WIDTH,FIX_HEIGHT});

	size_t idx = 0;
	Type::Coord coord = {0};
	for (coord.y = 0; coord.y < ((Type::Size)bluePrint->blocks).height; ++coord.y)
	{
		for (coord.x = 0; coord.x < ((Type::Size)bluePrint->blocks).width; ++coord.x)
		{
			_blockID::type blockID;
			getDx<_blockID>(blockID, buffer, idx);
			idx += _blockID::len;
			bluePrint->blocks[coord].id = transition(blockID);
			if (blockID == 3)
			{
				bluePrint->blocks[coord].flags |= GridFlags::SpawnPoint;
			}
		}
	}

	_titleOfBluePrint::type titleOfBluePrint;
	getDx<_titleOfBluePrint>(titleOfBluePrint, buffer);
	bluePrint->title = titleOfBluePrint;

	_scoreToUnlock::type scoreToUnlock = 0;
	getDx<_scoreToUnlock>(scoreToUnlock, buffer);
	bluePrint->scoreToUnlock = scoreToUnlock;

	_globalGravity::type globalGravity = 0;
	getDx<_globalGravity>(globalGravity, buffer);
	bluePrint->globalGravity = globalGravity;

	bluePrint->cameraSize = {VISIBLE_WIDTH, VISIBLE_HEIGHT};

	KIR5::sha512(bluePrint->hash, buffer.data(), buffer.size());

	return true;
}

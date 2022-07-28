#include "BluePrint.h"

#include "Tools.h"

BluePrint::IO_HamSt1::IO_HamSt1(BluePrint &bluePrint):
	bluePrint(bluePrint)
{

}
bool BluePrint::IO_HamSt1::encode(std::vector<unsigned char> &data)
{
	if (((Type::Size)bluePrint.blocks).width <= 0 ||
		((Type::Size)bluePrint.blocks).height <= 0)
	{
		return false;
	}
	data.resize(
		KIR5::SHA512_DIGEST_SIZE +
		4 +
		128 +
		2 +
		2 +
		2 +
		((Type::Size)bluePrint.blocks).width * ((Type::Size)bluePrint.blocks).height
	);

	/* DATA */
	{
		std::size_t pos = _data;
		Type::Coord coord;
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint.blocks).height; ++coord.y)
		{
			for (coord.x = 0; coord.x < ((Type::Size)bluePrint.blocks).width; ++coord.x)
			{
				unsigned __int16 id = bluePrint.blocks[coord].id;
				unsigned __int8 idx = Type::Rotations::getIndexOfRotation(bluePrint.blocks[coord].rotation);
				unsigned __int8 flags = bluePrint.blocks[coord].flags;
				memcpy(&data[pos], &id, 2); pos += 2;
				memcpy(&data[pos], &idx, 1); pos += 1;
				memcpy(&data[pos], &flags, 1); pos += 1;
			}
		}
	}

	/* HEAD */
	{
		unsigned __int16 scoreToUnlock = bluePrint.scoreToUnlock;
		memcpy(&data[_scoreToUnlock], &scoreToUnlock, 2);

		float speed = bluePrint.speed;
		memcpy(&data[_speed], &speed, 4);

		memset(&data[_title], 0, 128);
		if (bluePrint.getTitle().length() > 0)
		{
			memcpy(&data[_title], bluePrint.getTitle().c_str(), (std::min)(bluePrint.getTitle().length(), 128u));
		}
		else
		{
			return false;
		}

		unsigned __int16 width = ((Type::Size)bluePrint.blocks).width;
		memcpy(&data[_width], &width, 2);

		unsigned __int16 height = ((Type::Size)bluePrint.blocks).height;
		memcpy(&data[_height], &height, 2);
	}

	KIR5::sha512digest hash;
	KIR5::sha512(hash, &data[KIR5::SHA512_DIGEST_SIZE], data.size() - KIR5::SHA512_DIGEST_SIZE);

	return true;
}
bool BluePrint::IO_HamSt1::decode(const std::vector<unsigned char> &data)
{
	/* HEAD */
	{
		unsigned __int16 scoreToUnlock = 0;
		memcpy(&scoreToUnlock, &data[_scoreToUnlock], 2);
		bluePrint.scoreToUnlock = scoreToUnlock;

		float speed = 0;
		memcpy(&speed, &data[_speed], 4);
		bluePrint.speed = speed;

		char title[129] = {0};
		memcpy(title, &data[_title], 128u);
		bluePrint.title = title;


		unsigned __int16 width = 0;
		memcpy(&width, &data[_width], 2);

		unsigned __int16 height = 0;
		memcpy(&height, &data[_height], 2);

		if (width <= 0 || height <= 0)
		{
			return false;
		}

		bluePrint.blocks.resize({width,height});
	}


	/* DATA */
	{
		std::size_t pos = _data;
		Type::Coord coord;
		for (coord.y = 0; coord.y < ((Type::Size)bluePrint.blocks).height; ++coord.y)
		{
			for (coord.x = 0; coord.x < ((Type::Size)bluePrint.blocks).width; ++coord.x)
			{
				unsigned __int16 id = 0;
				unsigned __int8 idx = 0;
				unsigned __int8 flags = 0;
				memcpy(&id, &data[pos], 2); pos += 2;
				memcpy(&idx, &data[pos], 1); pos += 1;
				memcpy(&flags, &data[pos], 1); pos += 1;

				bluePrint.blocks[coord].id = id;
				bluePrint.blocks[coord].rotation = Type::Rotations::getRotationOfIndex(idx);
				bluePrint.blocks[coord].flags = flags;
			}
		}
	}


	KIR5::sha512digest hash;
	KIR5::sha512(hash, &data[KIR5::SHA512_DIGEST_SIZE], data.size() - KIR5::SHA512_DIGEST_SIZE);

	memcpy(&bluePrint.hash, &data[_HASH521], KIR5::SHA512_DIGEST_SIZE);

	if (memcmp(hash, bluePrint.hash, KIR5::SHA512_DIGEST_SIZE) != 0)
	{
		return false;
	}

	return true;
}

BluePrint::IO_HamBC::IO_HamBC(BluePrint &bluePrint):
	bluePrint(bluePrint)
{

}
bool BluePrint::IO_HamBC::encode(std::vector<unsigned char> &data)
{
}
bool BluePrint::IO_HamBC::decode(const std::vector<unsigned char> &data)
{
}

BluePrint::IO_Orig::IO_Orig(BluePrint &bluePrint):
	bluePrint(bluePrint)
{

}
bool BluePrint::IO_Orig::encode(std::vector<unsigned char> &data)
{
	data.resize(0x600);

	unsigned char *cur = data.data();

	//LAYOUT
	Type::Coord coord;
	for (coord.y = 0; coord.y < ((Type::Size)bluePrint.blocks).height; ++coord.y)
	{
		for (coord.x = 0; coord.x < ((Type::Size)bluePrint.blocks).width; ++coord.x)
		{
			if (bluePrint.blocks[coord].flags & GridFlags::SpawnPoint)
			{
				*cur++ = 3;
			}
			else
			{
				*cur++ = 
					getOriginalID((*bluePrint)[Type::Coord{x ,y}].id);
			}
		}
	}

	//GAP
	for (int i = 0; i < 6; i++)
	{
		cur++;
	}

	//TITLE
	std::string title = bluePrint.title;
	title.resize(23, ' ');
	for (int i = 0; i < 23; i++)
	{
		*cur++ = title[i];
	}

	//GAP
	cur++;

	//INFOTRONS TO COLLECT
	*cur++ = bluePrint.scoreToUnlock;

	return true;
}
bool BluePrint::IO_Orig::decode(const std::vector<unsigned char> &data)
{
	if (data.size() != 0x600)
	{
		return false;
	}

	bluePrint.blocks.resize({60,24});

	const unsigned char *cur = data.data();

	//LAYOUT
	Type::Coord coord;
	for (coord.y = 0; coord.y < ((Type::Size)bluePrint.blocks).height; ++coord.y)
	{
		for (coord.x = 0; coord.x < ((Type::Size)bluePrint.blocks).width; ++coord.x)
		{
			if (*cur == 3)
			{
				bluePrint.blocks[coord].flags |= GridFlags::SpawnPoint;
			}
			bluePrint.blocks[coord].id = transition[*cur++];
		}
	}

	//GAP
	for (int i = 0; i < 6; i++)
	{
		cur++;
	}

	//TITLE
	bluePrint.title.clear();
	for (int i = 0; i < 23; i++)
	{
		bluePrint.title += *cur++;
	}

	//GAP
	cur++;

	//INFOTRONS TO COLLECT
	bluePrint.scoreToUnlock = *cur++;

	return true;
}

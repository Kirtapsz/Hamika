#include "BluePrintIO.h"

unsigned __int8 OriginalLevelBluePrintIO::getOriginalID(Type::ID id)
{
	for (unsigned __int8 i = 0; i < sizeof(transition) / sizeof(transition[0]); ++i)
	{
		if (transition[i] == id)
		{
			return i;
		}
	}
	return 0;
}

std::shared_ptr<BluePrint> OriginalLevelBluePrintIO::read(std::vector<unsigned __int8> &buffer, size_t &bytesRead)
{
	bytesRead = 0;
	if (buffer.size() < 0x600)
		return nullptr;

	bytesRead = 0x600;
	std::shared_ptr<BluePrint> bluePrint(new BluePrint());
	bluePrint->resize({60,24});

	unsigned __int8 *cur = buffer.data();

	//LAYOUT
	for (int y = 0; y < 24; y++)
	{
		for (int x = 0; x < 60; x++)
		{
			if (*cur == 3)
			{
				(*bluePrint)[Type::Coord{x ,y}].flags |= GridFlags::SpawnPoint;
			}
			(*bluePrint)[Type::Coord{x ,y}].id = transition[*cur++];
		}
	}

	//GAP
	for (int i = 0; i < 6; i++)
	{
		cur++;
	}

	//TITLE
	std::string title;
	for (int i = 0; i < 23; i++)
	{
		title += *cur++;
	}
	bluePrint->SetName(title);

	//GAP
	cur++;

	//INFOTRONS TO COLLECT
	bluePrint->SetAim(*cur++);

	return bluePrint;
}

std::vector<unsigned __int8> OriginalLevelBluePrintIO::write(const std::shared_ptr<BluePrint> &bluePrint, size_t &bytesWrite)
{
	std::vector<unsigned __int8> buffer(0x600, 0);

	bytesWrite = 0x600;

	unsigned __int8 *cur = buffer.data();

	//LAYOUT
	for (int y = 0; y < 24; y++)
	{
		for (int x = 0; x < 60; x++)
		{
			if ((*bluePrint)[Type::Coord{x ,y}].flags & GridFlags::SpawnPoint)
			{
				*cur++ = 3;
			}
			else
			{
				*cur++ = getOriginalID((*bluePrint)[Type::Coord{x ,y}].id);
			}
		}
	}

	//GAP
	for (int i = 0; i < 6; i++)
	{
		cur++;
	}

	//TITLE
	std::string title = bluePrint->GetName();
	title.resize(23, ' ');
	for (int i = 0; i < 23; i++)
	{
		*cur++ = title[i];
	}

	//GAP
	cur++;

	//INFOTRONS TO COLLECT
	*cur++ = bluePrint->GetAim();

	return buffer;
}
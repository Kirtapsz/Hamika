#pragma once

#include "Types.h"
#include "Array2D.h"

#include "Objects.h"

#include <string>
#include <fstream>
#include <vector>

struct BluePrintBlock
{
	Type::ID id = 0;
	Type::Rotation rotation = 0;
	Type::Flags flags = 0;
};

class BluePrint:public Array2D<BluePrintBlock>
{
	Type::Code code;
	Type::Speed speed = 0.08f;
	std::string name = "UNKNOWN";
	int aim = 0;
	public:
	using Array2D<BluePrintBlock>::Array2D;
	void SetName(const std::string &new_name);
	void SetAim(int new_aim);
	void SetSpeed(Type::Speed new_speed);

	Type::Speed GetSpeed() const;
	int GetAim() const;
	std::string GetName() const;
};

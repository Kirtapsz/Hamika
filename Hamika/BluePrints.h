#pragma once

#include "Types.h"
#include "Array2D.h"

#include "Objects.h"

#include <string>
#include <fstream>
#include <vector>

#include "BluePrint.h"

struct WorldIO
{
	std::vector<std::shared_ptr<BluePrint>> levels;

	virtual void Read(const std::string filename) = 0;
	virtual void Write(const std::string filename) const = 0;
};

struct OriginalLevelWorldIO: WorldIO
{
	virtual void Read(const std::string filename);
	virtual void Write(const std::string filename) const;
};

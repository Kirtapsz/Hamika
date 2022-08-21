#pragma once

#include "Types.h"
#include "Array2D.h"

#include "Objects.h"

#include <string>
#include <fstream>
#include <vector>
#include <array>

#include "BluePrint.h"

#include <KIR/hash/KIR5_sha512.h>

class World
{
	public: enum VType
	{
		T_HamSt1,
		T_HamBC,
		T_Orig,
	};

	private: std::string title = "UNKNOWN";
	private: std::vector<std::shared_ptr<BluePrint>> bluePrints;

	public: bool load(const std::string &filename);
	public: bool save(const std::string &filename, World::VType type);

	public: inline const std::vector<std::shared_ptr<BluePrint>> &getBluePrints() const
	{
		return bluePrints;
	}
	public: inline void setBluePrints(const std::vector<std::shared_ptr<BluePrint>> &bluePrints)
	{
		this->bluePrints = bluePrints;
	}
	public: inline const std::string &getTitle() const
	{
		return title;
	}
	public: inline void setTitle(const std::string &title)
	{
		this->title = title;
	}
};

struct WorldIO
{
	const std::string name;
	World::VType type;
	std::function<bool(const std::vector<unsigned char> &data)> isType;
	std::function<bool(const std::vector<unsigned char> &data, World &world)> load;
	std::function<bool(std::vector<unsigned char> &data, const World &world)> save;
};
extern std::array<WorldIO, 3> worldIOs;
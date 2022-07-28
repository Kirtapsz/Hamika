#pragma once

#include "Types.h"
#include "Array2D.h"
#include "IO.h"
#include "Objects.h"

#include <string>
#include <fstream>
#include <vector>

#include <KIR/hash/KIR5_sha512.h>

class BluePrint
{
	public: struct Block
	{
		Type::ID id = 0;
		Type::Rotation rotation = 0;
		Type::Flags flags = 0;
	};

	private: KIR5::sha512digest hash;
	private: Array2D<Block> blocks;
	private: Type::Speed speed = 0.08f;
	private: std::string title = "UNKNOWN";
	private: unsigned int scoreToUnlock;

	private: inline const KIR5::sha512digest &getHash() const
	{
		return hash;
	}
	private: inline const Array2D<Block> &getBlocks() const
	{
		return blocks;
	}
	private: inline Type::Speed getSpeed() const
	{
		return speed;
	}
	private: inline const std::string &getTitle() const
	{
		return title;
	}
	private: inline  unsigned int getScoreToUnlock() const
	{
		return scoreToUnlock;
	}

	public: struct IO_HamSt1: public IO::Stream
	{
		/*
		64  : HASH521       (binary)
		4   : speed         (float)
		128 : title         (string without null terminate)
		2   : scoreToUnlock (unsigned int16)
		2   : width         (unsigned int16 > 0)
		2   : height        (unsigned int16 > 0)
			<-- rows * N -->
			2 : id          (unsigned int16)
			1 : rotation    (0-up, 1-right, 2-down, 3-left)
			1 : flags       (unsigned int8)
		*/

		enum
		{
			_HASH521 = 0,
			_speed = 64,
			_title = 68,
			_scoreToUnlock = 196,
			_width = 198,
			_height = 200,
			_data = 202,
		};

		BluePrint &bluePrint;
		IO_HamSt1(BluePrint &bluePrint);
		bool decode(const std::vector<unsigned char> &data);
		bool encode(std::vector<unsigned char> &data);
	};


	public: struct IO_HamBC: public IO::Stream
	{
		static constexpr int MapCodeNONE = 0;
		static constexpr char MapCodeRANDOM = 'R';
		static constexpr char MapCodeMANUAL = 'M';

		static constexpr unsigned OLDKIRW = 63;
		static constexpr unsigned OLDKIRH = 53;

		struct TranslateData
		{
			Type::ID Old;
			Type::ID New;
		};

		static constexpr TranslateData translate[] = {
			{0,-123},{1,},{2,},{3,0},{4,1},{5,32},{6,14},{7,15},{8,29},{9,31},{10,30},{11,28},{12,20},{13,21},{14,19},{15,22},{16,18},{17,17},
			{18,},{19,},{20,},{21,},{22,},{23,},{24,},{25,},{26,16},{27,27},{28,13},{29,},{30,},{31,},{32,},{33,},{34,},{35,},{36,},
			{37,25},{38,23},{39,},{40,},{41,},{42,},{43,},{44,24},{45,26},{46,},{47,},{48,},{49,},{50,},{51,},{52,},{53,4},{54,},{55,},
			{56,},{57,},{58,},{59,},{60,2},{61,},{62,},{63,},{64,},{65,},{66,},{67,},{68,},{69,},{70,},{71,},{72,},{73,},{74,6},{75,3},
			{76,7},{77,10},{78,8},{79,12},{80,9},{81,5},{82,11},{83,},{84,},{85,},{86,},{87,},{88,},{89,},
		};

		BluePrint &bluePrint;
		IO_HamBC(BluePrint &bluePrint);
		bool decode(const std::vector<unsigned char> &data);
		bool encode(std::vector<unsigned char> &data);
	};

	public: struct IO_Orig: public IO::Stream
	{
		static constexpr Type::ID transition[]
		{
			ObjectID::Space,
			ObjectID::Zonk,
			ObjectID::BaseX,
			ObjectID::Murphy,
			ObjectID::Infotron,
			ObjectID::RAMChipsMini,
			ObjectID::HardwareBrick1,
			ObjectID::Exit,
			ObjectID::Utility1,
			ObjectID::OneWayPortRight,
			ObjectID::OneWayPortDown,
			ObjectID::OneWayPortLeft,
			ObjectID::OneWayPortUp,
			ObjectID::OneWayPortRightGrav,
			ObjectID::OneWayPortDownGrav,
			ObjectID::OneWayPortLeftGrav,
			ObjectID::OneWayPortUpGrav,
			ObjectID::SnikSnakMove,
			ObjectID::Utility3,
			ObjectID::Terminal,
			ObjectID::Utility2,
			ObjectID::DualPortVertical,
			ObjectID::DualPortHorizontal,
			ObjectID::CrossPort,
			ObjectID::Electrons,
			ObjectID::Bug,
			ObjectID::RAMChipsLeft,
			ObjectID::RAMChipsRight,
			ObjectID::HardwareSwitch,
			ObjectID::HardwareGreenLed,
			ObjectID::HardwareBlueLed,
			ObjectID::HardwareRedLed,
			ObjectID::HardwareStripLine,
			ObjectID::RAMChipsTop,
			ObjectID::RAMChipsBottom,
			ObjectID::HardwareResource,
			ObjectID::HardwareRedCapacitor,
			ObjectID::HardwareResistance,
			ObjectID::HardwareBlueResistance,
			ObjectID::HardwareYellowResistance,
		};

		BluePrint &bluePrint;
		IO_Orig(BluePrint &bluePrint);
		bool decode(const std::vector<unsigned char> &data);
		bool encode(std::vector<unsigned char> &data);
	};
};


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


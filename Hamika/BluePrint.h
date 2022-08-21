#pragma once

#include "Types.h"
#include "Array2D.h"
#include "Tools.h"
#include "Objects.h"

#include <string>
#include <fstream>
#include <vector>

#include <KIR/hash/KIR5_sha512.h>

struct BluePrint
{
	struct Block
	{
		Type::ID id = 0;
		Type::Rotation rotation = 0;
		Type::Flags flags = 0;
	};

	static constexpr Type::Move::Type cameraSize1to1 = 0.f;
	Type::CameraSize cameraSize = {cameraSize1to1, cameraSize1to1}; // number of blocks visible
	KIR5::sha512digest hash;
	Array2D<Block> blocks;
	Type::Speed speed = 0.08f;
	std::string title = "UNKNOWN";
	bool globalGravity = false;
	unsigned int scoreToUnlock;

	inline Block &operator[](Type::Coord coord)
	{
		return blocks[coord];
	}

	inline const Block &operator[](Type::Coord coord) const
	{
		return blocks[coord];
	}

	public: struct IO_HamSt1
	{
		/*
		64  : HASH521       (binary)
		4   : speed         (float)
		128 : title         (string without null terminate)
		2   : scoreToUnlock (unsigned int16)
		1   : globalGravity (unsigned int8)
		2   : width         (unsigned int16 > 0)
		2   : height        (unsigned int16 > 0)
			<-- rows * N -->
			2 : id          (unsigned int16)
			1 : rotation    (0-up, 1-right, 2-down, 3-left)
			1 : flags       (unsigned int8)
		*/
		struct _HASH521
		{
			constexpr static size_t idx = 0;
			typedef KIR5::sha512digest type;
			constexpr static size_t len = std::extent<type>::value;
		};
		struct _murphySpeed
		{
			constexpr static size_t idx = tail<_HASH521>();
			typedef float type;
			constexpr static size_t len = sizeof(type);
		};
		struct _titleOfBluePrint
		{
			constexpr static size_t idx = tail<_murphySpeed>();
			constexpr static size_t len = 128;
			typedef std::string type;
		};
		struct _scoreToUnlock
		{
			constexpr static size_t idx = tail<_titleOfBluePrint>();
			typedef unsigned __int16 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _globalGravity
		{
			constexpr static size_t idx = tail<_scoreToUnlock>();
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _width
		{
			constexpr static size_t idx = tail<_globalGravity>();
			typedef unsigned __int16 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _height
		{
			constexpr static size_t idx = tail<_width>();
			typedef unsigned __int16 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _cameraWidth
		{
			constexpr static size_t idx = tail<_height>();
			typedef float type;
			constexpr static size_t len = sizeof(type);
		};
		struct _cameraHeight
		{
			constexpr static size_t idx = tail<_cameraWidth>();
			typedef float type;
			constexpr static size_t len = sizeof(type);
		};
		struct _headTail
		{
			constexpr static size_t idx = tail<_cameraHeight>();
			constexpr static size_t len = 0;
		};

		struct _blockID
		{
			constexpr static size_t idx = 0;
			typedef unsigned __int16 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _rotation
		{
			constexpr static size_t idx = tail<_blockID>();
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _flags
		{
			constexpr static size_t idx = tail<_blockID>();
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _blockDataTail
		{
			constexpr static size_t idx = tail<_flags>();
			constexpr static size_t len = 0;
		};

		bool load(const std::vector<unsigned char> &data, const std::shared_ptr<BluePrint> &bluePrint);
		bool save(std::vector<unsigned char> &data, const std::shared_ptr<BluePrint> &bluePrint);
	};


	public: struct IO_HamBC
	{
		static constexpr int MAP_SIZE = 0x41A0;

		static constexpr unsigned FIX_WIDTH = 63;
		static constexpr unsigned FIX_HEIGHT = 53;

		/*
		<-- 63 * 53 -->
			1 : block ID                (unsigned __int8)
		<-- 63 * 53 -->
			2 : rotation of block       (unsigned __int16)
		<-- 63 * 53 -->
			1 : is explode              (bool)
		<-- 63 * 53 -->
			1 : is gravitation          (bool)
		2  : score to unlock              (unsigned __int16)
		4  : murphy speed                 (float)
		1  : murphy spawn fill block ID   (unsigned __int8)
		98 : title of blue print          (null terminated string)
		*/

		struct _blockID
		{
			constexpr static size_t idx = 0;
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _rotationOfBlock
		{
			constexpr static size_t idx = _blockID::idx + (FIX_WIDTH * FIX_HEIGHT) * _blockID::len;
			typedef unsigned __int16 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _isExplode
		{
			constexpr static size_t idx = _rotationOfBlock::idx + (FIX_WIDTH * FIX_HEIGHT) * _rotationOfBlock::len;
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _isGravitation
		{
			constexpr static size_t idx = _isExplode::idx + (FIX_WIDTH * FIX_HEIGHT) * _isExplode::len;
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _scoreToUnlock
		{
			constexpr static size_t idx = _isGravitation::idx + (FIX_WIDTH * FIX_HEIGHT) * _isGravitation::len;
			typedef unsigned __int16 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _murphySpeed
		{
			constexpr static size_t idx = tail<_scoreToUnlock>();
			typedef float type;
			constexpr static size_t len = sizeof(type);
		};
		struct _murphySpawnFillBlockID
		{
			constexpr static size_t idx = tail<_murphySpeed>();
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _titleOfBluePrint
		{
			constexpr static size_t idx = tail<_murphySpawnFillBlockID>();
			constexpr static size_t len = 98;
			typedef std::string type;
		};

		struct TranslateData
		{
			Type::ID oldID;
			Type::ID newID;
		};

		static constexpr TranslateData translate[] = {
			{0,ObjectID::Murphy},
			//{1,ObjectID::}, orange speed zonk
			//{2,ObjectID::}, panel with screw
			{3,ObjectID::Space},
			{4,ObjectID::Zonk},
			{5,ObjectID::Explosion},
			{6,ObjectID::Infotron},
			{7,ObjectID::Exit},
			{8,ObjectID::Utility1},
			{9,ObjectID::Utility3},
			{10,ObjectID::Utility2},
			{11,ObjectID::Terminal},
			{12,ObjectID::OneWayPortLeft},
			{13,ObjectID::OneWayPortRight},
			{14,ObjectID::OneWayPortDown},
			{15,ObjectID::OneWayPortUp},
			{16,ObjectID::DualPortVertical},
			{17,ObjectID::DualPortHorizontal},
			//{18,ObjectID::}, Portal, Up:in Right:in Down:out Left:out
			//{19,ObjectID::}, Portal, iooi

			//{20,ObjectID::}, Portal,ooii
			//{21,ObjectID::}, Portal,oiio
			//{22,ObjectID::}, Portal,ioii
			//{23,ObjectID::}, Portal,iiio
			//{24,ObjectID::}, Portal,iioi
			//{25,ObjectID::}, Portal,oiii
			{26,ObjectID::CrossPort},
			{27,ObjectID::SnikSnakMove},
			{28,ObjectID::Bug},
			//{29,ObjectID::}, Ram, Up:wall Right:wall Down:wall Left:pins

			//{30,ObjectID::}, Ram, pwww
			//{31,ObjectID::}, Ram, wpww
			//{32,ObjectID::}, Ram, wwpw
			//{33,ObjectID::}, Ram, ppww
			//{34,ObjectID::}, Ram, pwwp
			//{35,ObjectID::}, Ram, wwpp
			//{36,ObjectID::}, Ram, wppw
			//{37,ObjectID::}, Ram, wpwp
			//{38,ObjectID::}, Ram, pwpw
			{39,ObjectID::RAMChipsLeft},
			{40,ObjectID::RAMChipsRight},
			{41,ObjectID::RAMChipsTop},
			{42,ObjectID::RAMChipsBottom},
			//{43,ObjectID::}, Ram, wwww
			{44,ObjectID::RAMChipsMini},
			{45,ObjectID::Electrons},
			//{46,ObjectID::}, utility purple big explosion
			//{47,ObjectID::}, utility green gas explosion
			//{48,ObjectID::}, gas
			//{49,ObjectID::}, big infotron

			//{50,ObjectID::}, solid infotron
			//{51,ObjectID::}, utility pink infotron explosion
			{52,ObjectID::HardwareBrick1},
			//{53,ObjectID::}, HardwareBrick layout 2
			//{54,ObjectID::}, HardwareBrick layout 3
			//{55,ObjectID::}, base led blue
			//{56,ObjectID::}, base led red
			//{57,ObjectID::}, base led green
			//{58,ObjectID::}, base resistor with processor
			//{59,ObjectID::}, base with transistor

			{60,ObjectID::BaseX},
			//{61,ObjectID::}, empty base
			//{62,ObjectID::}, frozen base
			//{63,ObjectID::}, heat base
			//{64,ObjectID::}, base led cyan
			//{65,ObjectID::}, base led yellow
			//{66,ObjectID::}, base led pink
			//{67,ObjectID::}, base led purple
			//{68,ObjectID::}, base with active vent
			//{69,ObjectID::}, utility rainbow

			//{70,ObjectID::}, green upfall zonk
			//{71,ObjectID::}, wooden box
			//{72,ObjectID::}, Murphy2
			{73,ObjectID::HardwareSwitch},
			{74,ObjectID::HardwareGreenLed},
			{75,ObjectID::HardwareBlueLed},
			{76,ObjectID::HardwareRedLed},
			{77,ObjectID::HardwareStripLine},
			{78,ObjectID::HardwareResource},
			{79,ObjectID::HardwareRedCapacitor},

			{80,ObjectID::HardwareResistance},
			{81,ObjectID::HardwareBlueResistance},
			{82,ObjectID::HardwareYellowResistance},
			//{83,ObjectID::}, Hardware green panel empty
			//{84,ObjectID::}, Hardware leds panel base empty
			//{85,ObjectID::}, HardwareBrick red
			//{86,ObjectID::}, HardwareBrick orange
			//{87,ObjectID::}, HardwareBrick yellow
			//{88,ObjectID::}, HardwareBrick green
			//{89,ObjectID::}, HardwareBrick cyan

			//{90,ObjectID::}, HardwareBrick pink
			//{91,ObjectID::}, HardwareBrick purple
			//{92,ObjectID::}, HardwareBrick blue
			//{93,ObjectID::}, Hardware square rock with rainbow
		};

		inline static Type::ID transition(Type::ID id)
		{
			for (size_t i = 0; i < std::size(translate); ++i)
			{
				if (translate[i].oldID == id)
				{
					return translate[i].newID;
				}
			}
			return ObjectID::Space;
		}
		inline static Type::ID detransition(Type::ID id)
		{
			for (size_t i = 0; i < std::size(translate); ++i)
			{
				if (translate[i].newID == id)
				{
					return translate[i].oldID;
				}
			}
			return 3;
		}

		bool load(const std::vector<unsigned char> &data, const std::shared_ptr<BluePrint> &bluePrint);
		bool save(std::vector<unsigned char> &data, const std::shared_ptr<BluePrint> &bluePrint);
	};

	public: struct IO_Orig
	{
		static constexpr int MAP_SIZE = 0x600;

		static constexpr unsigned FIX_WIDTH = 60;
		static constexpr unsigned FIX_HEIGHT = 24;

		struct _blockID
		{
			constexpr static size_t idx = 0x000;
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _titleOfBluePrint
		{
			constexpr static size_t idx = 0x5A6;
			constexpr static size_t len = 23;
			typedef std::string type;
		};
		struct _scoreToUnlock
		{
			constexpr static size_t idx = 0x5BE;
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};
		struct _globalGravity
		{
			constexpr static size_t idx = 0x5A4;
			typedef unsigned __int8 type;
			constexpr static size_t len = sizeof(type);
		};

		static constexpr float VISIBLE_WIDTH = 20.f;
		static constexpr float VISIBLE_HEIGHT = 11.f;

		static constexpr Type::ID translate[]
		{
			ObjectID::Space,
			ObjectID::Zonk,
			ObjectID::BaseX,
			ObjectID::Space,
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
		inline static Type::ID transition(size_t idx)
		{
			return translate[idx];
		}
		inline static size_t detransition(Type::ID id)
		{
			for (size_t i = 0; i < std::size(translate); ++i)
			{
				if (translate[i] == id)
				{
					return i;;
				}
			}
			return 0;
		}

		bool load(const std::vector<unsigned char> &data, const std::shared_ptr<BluePrint> &bluePrint);
		bool save(std::vector<unsigned char> &data, const std::shared_ptr<BluePrint> &bluePrint);
	};
};

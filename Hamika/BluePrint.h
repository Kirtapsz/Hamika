#pragma once

#include "Matrix.h"
#include "Resource.h"
#include "Objects.h"

namespace Res
{
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
		KIR5::sha512digest hash{};
		Matrix<Block> blocks;
		Type::Speed speed = 0.08f;
		std::string title = "UNKNOWN";
		bool globalGravity = false;
		std::size_t scoreToUnlock = 0;

		inline BluePrint()
		{

		}

		inline Block &operator[](Type::Coord coord)
		{
			return blocks[coord];
		}

		inline const Block &operator[](Type::Coord coord) const
		{
			return blocks[coord];
		}

		struct IO_HamSt1_Block: public Record<
			std::uint16_t,                // id
			std::uint8_t,                 // rotation (0-up, 1-right, 2-down, 3-left)
			std::uint8_t                  // flags
		>
		{
			constexpr static std::size_t id = 0;
			constexpr static std::size_t rotation = 1;
			constexpr static std::size_t flags = 2;

			constexpr static std::array<const char*, 3> keys{{"BlockID", "Rotation", "Flags"}};
		};

		struct IO_HamSt1: HashRecord <
			std::float_t,                 // speed
			FixedStringRecord<128>,       // title
			std::uint16_t,                // scoreToUnlock
			std::uint8_t,                 // globalGravity
			std::float_t,                 // camera width
			std::float_t,                 // camera height
			MatrixRecord<std::uint16_t, IO_HamSt1_Block>
		>
		{
			constexpr static std::size_t speed = 0;
			constexpr static std::size_t title = 1;
			constexpr static std::size_t scoreToUnlock = 2;
			constexpr static std::size_t globalGravity = 3;
			constexpr static std::size_t cameraWidth = 4;
			constexpr static std::size_t cameraHeight = 5;
			constexpr static std::size_t blocks = 6;

			constexpr static std::array<const char *, 7> keys{{"Speed", "Title", "ScoreToUnlock", "Gravity", "CameraWidth", "CameraHeight", "Blocks"}};
		};
		BluePrint(const IO_HamSt1 &record);
		operator IO_HamSt1() const;

		struct IO_HamBC: HashRecord <
			FixedMatrixRecord<std::uint8_t, 63, 53>,  // block ID
			FixedMatrixRecord<std::uint16_t, 63, 53>, // rotation of block
			FixedMatrixRecord<std::uint8_t, 63, 53>,  // is explode
			FixedMatrixRecord<std::uint8_t, 63, 53>,  // is gravitation
			std::uint16_t,                            // scoreToUnlock
			std::float_t,                             // speed
			std::uint8_t,                             // murphy spawn fill block ID
			TerminatedStringRecord<98>                // title of blue print
		>
		{
			constexpr static std::size_t id = 0;
			constexpr static std::size_t rotation = 1;
			constexpr static std::size_t explode = 2;
			constexpr static std::size_t gravitation = 3;
			constexpr static std::size_t scoreToUnlock = 4;
			constexpr static std::size_t speed = 5;
			constexpr static std::size_t fillId = 6;
			constexpr static std::size_t title = 7;

			constexpr static std::array<const char *, 8> keys{{"BlockID", "Rotation", "Explode", "Gravity", "ScoreToUnlock", "Speed", "FillID", "Title"}};

			static constexpr std::size_t MAP_SIZE = 0x41A0;

			static constexpr std::size_t FIX_WIDTH = 63;
			static constexpr std::size_t FIX_HEIGHT = 53;

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
		};
		BluePrint(const IO_HamBC &record);
		operator IO_HamBC() const;

		struct IO_Orig: HashRecord <
			HeapVectorRecord<std::uint8_t, 60 * 24>,  // block ID
			FixedVectorRecord<std::uint8_t, 4>,       // unused
			std::uint8_t,                             // gravity
			FixedVectorRecord<std::uint8_t, 1>,       // unused
			FixedStringRecord<23>,                    // title
			FixedVectorRecord<std::uint8_t, 1>,       // unused
			std::uint8_t,                             // score to unlock
			FixedVectorRecord<std::uint8_t, 65>       // unused
		>
		{
			constexpr static std::size_t id = 0;
			constexpr static std::size_t globalGravity = 2;
			constexpr static std::size_t title = 4;
			constexpr static std::size_t scoreToUnlock = 6;

			constexpr static std::array<const char *, 8> keys{{"BlockID", "Unused1", "Gravity", "Unused2", "Title", "Unused3", "ScoreToUnlock", "Unused4"}};

			static constexpr std::size_t MAP_SIZE = 0x600;

			static constexpr std::size_t FIX_WIDTH = 60;
			static constexpr std::size_t FIX_HEIGHT = 24;

			static constexpr float VISIBLE_WIDTH = 20.f;
			static constexpr float VISIBLE_HEIGHT = 11.f;

			static constexpr std::uint8_t SpawnID = 3;

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
			inline static Type::ID transition(std::uint8_t idx)
			{
				return translate[idx];
			}
			inline static std::uint8_t detransition(Type::ID id)
			{
				for (std::uint8_t i = 0; i < std::size(translate); ++i)
				{
					if (translate[i] == id)
					{
						return i;
					}
				}
				return 0;
			}
		};
		BluePrint(const IO_Orig &record);
		operator IO_Orig() const;
	};
}
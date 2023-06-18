#pragma once

#include "Resource.h"

#include <KIR/AL/KIR5_bitmap.h>
#include <KIR/AL/KIR5_color.h>

namespace Res
{
	class Slides
	{
		private: std::vector<KIR5::SubBitmap> bitmaps;
		private: KIR5::SubBitmap errorBitmap;

		public: Slides();

		public: void initialize(KIR5::Bitmap &bitmap, KIR5::SubBitmap &errorBitmap);
		public: void initialize(KIR5::SubBitmap &bitmap, KIR5::SubBitmap &errorBitmap);
		public: int getCount() const;
		public: int getDrawNumber(std::float_t i) const;
		public: KIR5::SubBitmap &operator[](size_t i);
	};

	class BitmapBox: public Base
	{
		private: KIR5::Bitmap bitmap;
		private: KIR5::SubBitmap subBitmap;

		public: using Base::Base;

		public: static KIR5::Bitmap generate(int width, int height, KIR5::Color color);

		public: virtual bool initialize(std::uint32_t mode);
		public: virtual void shutdown();

		public: inline operator KIR5::SubBitmap &()
		{
			return subBitmap;
		}
	};

	class StickyBitmap: public StaticSkeleton<const char *, KIR5::Bitmap>
	{
		public: using StaticSkeleton<const char *, KIR5::Bitmap>::StaticSkeleton;
		public: using StaticSkeleton<const char *, KIR5::Bitmap>::operator[];

		protected: virtual KIR5::Bitmap get(const char *&key);
		protected: virtual void vadlidate(const char *&key) const;
	};
	class UIElements: public StickyBitmap
	{
		public: using StickyBitmap::StickyBitmap;
		public: using StickyBitmap::operator[];

		protected: virtual KIR5::Bitmap get(const char *&key);

		public: static constexpr char *Add = "Add";
		public: static constexpr char *ArrowTiltedUpLeft = "ArrowTiltedUpLeft";
		public: static constexpr char *ArrowTiltedUpRight = "ArrowTiltedUpRight";
		public: static constexpr char *ArrowTiltedDownRight = "ArrowTiltedDownRight";
		public: static constexpr char *ArrowTiltedDownLeft = "ArrowTiltedDownLeft";
		public: static constexpr std::array<const char *, 4> ArrowTiltedUp{ArrowTiltedUpLeft, ArrowTiltedUpRight, ArrowTiltedDownRight, ArrowTiltedDownLeft};
		public: static constexpr char *ArrowUp = "ArrowUp";
		public: static constexpr char *ArrowRight = "ArrowRight";
		public: static constexpr char *ArrowDown = "ArrowDown";
		public: static constexpr char *ArrowLeft = "ArrowLeft";
		public: static constexpr std::array<const char *, 4> Arrow{ArrowUp, ArrowRight, ArrowDown, ArrowLeft};
		public: static constexpr char *Ball = "Ball";
		public: static constexpr char *Cursor = "Cursor";
		public: static constexpr char *Delete = "Delete";
		public: static constexpr char *Detonate = "Detonate";
		public: static constexpr char *Edit = "Edit";
		public: static constexpr char *Execute = "Execute";
		public: static constexpr char *Grav = "Grav";
		public: static constexpr char *Play = "Play";
		public: static constexpr char *RotationDown = "RotationDown";
		public: static constexpr char *RotationLeft = "RotationLeft";
		public: static constexpr char *RotationRight = "RotationRight";
		public: static constexpr char *RotationUp = "RotationUp";
		public: static constexpr std::array<const char *, 4> Rotation{RotationDown, RotationLeft, RotationRight, RotationUp};
		public: static constexpr char *Set = "Set";
		public: static constexpr char *Locked = "Locked";
		public: static constexpr char *SpawnPoint = "SpawnPoint";
		public: static constexpr char *StatusBar = "StatusBar";
		public: static constexpr char *StickUp = "StickUp";
		public: static constexpr char *StickRight = "StickRight";
		public: static constexpr char *StickDown = "StickDown";
		public: static constexpr char *StickLeft = "StickLeft";
		public: static constexpr std::array<const char *, 4> Stick{StickUp, StickRight, StickDown, StickLeft};
		public: static constexpr char *Unset = "Unset";
		public: static constexpr char *Settings = "Settings";
	};

	class BitmapPool: public Base
	{
		public: static constexpr char *Fallback = "Fallback";

		private: struct Param
		{
			const char *name;
			int x, y, w, h;
			KIR5::SubBitmap subBitmap;
		};
		protected: int maxBitmapSize = 0;
		private: KIR5::Bitmap poolBitmap;
		private: KIR5::SubBitmap fallbackBitmap;
		private: std::vector<std::vector<Param>> layout;

		public: using Base::Base;

		private: Param createNewPool(const KIR5::Bitmap &bmp, const char *name);
		private: Param extendPool(const KIR5::Bitmap &bmp, const char *name);
		private: Param getParamForBitmap(const KIR5::Bitmap &bmp, const char *name);

		public: void addBitmap(const KIR5::Bitmap &bmp, const char *name);
		public: void finalise(KIR5::Bitmap &fallbackBitmap);
		public: KIR5::SubBitmap &operator[](const char *name);

		public: virtual void shutdown();
	};
	class Tiles: public BitmapPool
	{
		public: using BitmapPool::BitmapPool;
		public: using BitmapPool::operator[];

		public: virtual bool initialize(std::uint32_t mode);

		public: static constexpr char *_Player_Base = "Player-Base";
		public: static constexpr char *_Player_PlaceBase = "Player-BasePlace";
		public: static constexpr char *_Player_Suck = "Player-Suck";
		public: static constexpr char *_Player_Push = "Player-Push";
		public: static constexpr char *_Player_MovingUp = "Player-MovingUp";
		public: static constexpr char *_Player_MovingRight = "Player-MovingRight";
		public: static constexpr char *_Player_MovingDown = "Player-MovingDown";
		public: static constexpr char *_Player_MovingLeft = "Player-MovingLeft";
		public: static constexpr char *_Player_PassInDown = "Player-PassInDown";
		public: static constexpr char *_Player_PassInLeft = "Player-PassInLeft";
		public: static constexpr char *_Player_PassInUp = "Player-PassInUp";
		public: static constexpr char *_Player_PassInRight = "Player-PassInRight";
		public: static constexpr char *_Player_PassOutDown = "Player-PassOutDown";
		public: static constexpr char *_Player_PassOutLeft = "Player-PassOutLeft";
		public: static constexpr char *_Player_PassOutUp = "Player-PassOutUp";
		public: static constexpr char *_Player_PassOutRight = "Player-PassOutRight";
		public: static constexpr char *_001_ZonkMoveHorizontal = "001-ZonkMoveHorizontal";
		public: static constexpr char *_002_BaseX = "002-BaseX";
		public: static constexpr char *_002_BaseX_square_disappear = "002-BaseX.square-disappear";
		public: static constexpr char *_003_HardwareBlueLed = "003-HardwareBlueLed";
		public: static constexpr char *_004_HardwareBrick1 = "004-HardwareBrick1";
		public: static constexpr char *_005_HardwareBlueResistance = "005-HardwareBlueResistance";
		public: static constexpr char *_006_HardwareGreenLed = "006-HardwareGreenLed";
		public: static constexpr char *_007_HardwareRedLed = "007-HardwareRedLed";
		public: static constexpr char *_008_HardwareResource = "008-HardwareResource";
		public: static constexpr char *_009_HardwareResistance = "009-HardwareResistance";
		public: static constexpr char *_010_HardwareStripLine = "010-HardwareStripLine";
		public: static constexpr char *_011_HardwareYellowResistance = "011-HardwareYellowResistance";
		public: static constexpr char *_012_HardwareRedCapacitor = "012-HardwareRedCapacitor";
		public: static constexpr char *_013_Bug = "013-Bug";
		public: static constexpr char *_014_InfotronDisappear = "014-InfotronDisappear";
		public: static constexpr char *_014_InfotronMoveHorizontal = "014-InfotronMoveHorizontal";
		public: static constexpr char *_015_Exit = "015-Exit";
		public: static constexpr char *_016_CrossPort = "016-CrossPort";
		public: static constexpr char *_017_DualPortHorizontal = "017-DualPortHorizontal";
		public: static constexpr char *_018_DualPortVertical = "018-DualPortVertical";
		public: static constexpr char *_019_OneWayPortDown = "019-OneWayPortDown";
		public: static constexpr char *_020_OneWayPortLeft = "020-OneWayPortLeft";
		public: static constexpr char *_021_OneWayPortRight = "021-OneWayPortRight";
		public: static constexpr char *_022_OneWayPortUp = "022-OneWayPortUp";
		public: static constexpr char *_023_RAMChipsMini = "023-RAMChipsMini";
		public: static constexpr char *_024_RAMChipsLeft = "024-RAMChipsLeft";
		public: static constexpr char *_025_RAMChipsRight = "025-RAMChipsRight";
		public: static constexpr char *_026_Electrons = "026-Electrons";
		public: static constexpr char *_027_SnikSnakMoveUp = "027-SnikSnakMoveUp";
		public: static constexpr char *_027_SnikSnakMoveRight = "027-SnikSnakMoveRight";
		public: static constexpr char *_027_SnikSnakMoveDown = "027-SnikSnakMoveDown";
		public: static constexpr char *_027_SnikSnakMoveLeft = "027-SnikSnakMoveLeft";
		public: static constexpr char *_027_SnikSnakRotate = "027-SnikSnakRotate";
		public: static constexpr char *_028_Terminal = "028-Terminal";
		public: static constexpr char *_029_Utility1 = "029-Utility1";
		public: static constexpr char *_030_Utility2 = "030-Utility2";
		public: static constexpr char *_030_Utility2Activated = "030-Utility2Activated";
		public: static constexpr char *_031_Utility3 = "031-Utility3";
		public: static constexpr char *_032_Explosion = "032-Explosion";
		public: static constexpr char *_035_RAMChipsTop = "035-RAMChipsTop";
		public: static constexpr char *_036_RAMChipsBottom = "036-RAMChipsBottom";
		public: static constexpr char *_037_HardwareSwitch = "037-HardwareSwitch";
		public: static constexpr char *_038_OneWayPortDownGrav = "038-OneWayPortDownGrav";
		public: static constexpr char *_039_OneWayPortLeftGrav = "039-OneWayPortLeftGrav";
		public: static constexpr char *_040_OneWayPortRightGrav = "040-OneWayPortRightGrav";
		public: static constexpr char *_041_OneWayPortUpGrav = "041-OneWayPortUpGrav";
	};

	extern BitmapBox bitmapBox;
	extern Tiles tiles;
	extern UIElements uielements;
}

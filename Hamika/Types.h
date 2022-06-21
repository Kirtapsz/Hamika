#pragma once

#include <ostream>
#include <sstream>

namespace Type
{
	typedef __int64 Flags;
	typedef float Rotation;
	namespace Rotations
	{
		constexpr Rotation _0 = 0;
		constexpr Rotation _45 = 45;
		constexpr Rotation _90 = 90;
		constexpr Rotation _135 = 135;
		constexpr Rotation _180 = 180;
		constexpr Rotation _225 = 225;
		constexpr Rotation _270 = 270;
		constexpr Rotation _315 = 315;
		constexpr Rotation _360 = 360;

		constexpr Rotation Up = 0;
		constexpr Rotation Right = 90;
		constexpr Rotation Down = 180;
		constexpr Rotation Left = 270;

		constexpr int getRotationIndex(const Rotation &rotation)
		{
			if (rotation == Up)
			{
				return 0;
			}
			if (rotation == Right)
			{
				return 1;
			}
			if (rotation == Down)
			{
				return 2;
			}
			if (rotation == Left)
			{
				return 3;
			}
			return 0;
		}
	}
	typedef __int32 ID;
	typedef float Speed;
	typedef __int8 Code;

	struct Size
	{
		typedef __int32 Type;
		Type width;
		Type height;
		inline bool operator==(const Size &size) const
		{
			return width == size.width && height == size.height;
		}
		inline bool operator!=(const Size &size) const
		{
			return width != size.width || height != size.height;
		}
		static const Size Invalid;
		inline operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << width << " * " << height << " )";
			return os;
		}
	};
	struct Coord
	{
		typedef __int32 Type;
		Type x;
		Type y;
		inline bool operator==(const Coord &coord) const
		{
			return x == coord.x && y == coord.y;
		}
		inline bool operator!=(const Coord &coord) const
		{
			return x != coord.x || y != coord.y;
		}
		static const Coord Invalid;
		inline operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << x << " , " << y << " )";
			return os;
		}
	};

	struct Move
	{
		typedef float Type;
		Type x;
		Type y;
		inline bool operator==(const Move &coord) const
		{
			return x == coord.x && y == coord.y;
		}
		inline bool operator!=(const Move &coord) const
		{
			return x != coord.x || y != coord.y;
		}
		static const Move Invalid;
		inline operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << x << " , " << y << " )";
			return os;
		}
	};
}

enum GridFlags:Type::Flags
{
	Gravity = 1 << 0,
	Detonate = 1 << 1,
	SpawnPoint = 1 << 2,
};
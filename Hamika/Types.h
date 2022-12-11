#pragma once

#include <ostream>
#include <sstream>
#include <limits>

namespace Type
{
	typedef unsigned __int8 Flags8;
	typedef unsigned __int16 Flags16;
	typedef unsigned __int32 Flags32;
	typedef unsigned __int64 Flags64;
	typedef Flags64 Flags;

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

		constexpr int getIndexOfRotation(const Rotation &rotation)
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

		constexpr Rotation getRotationOfIndex(int index)
		{
			if (index == 0)
			{
				return Up;
			}
			if (index == 1)
			{
				return Right;
			}
			if (index == 2)
			{
				return Down;
			}
			if (index == 3)
			{
				return Left;
			}
			return Up;
		}
	}

	typedef __int16 ID;
	constexpr ID INVALID_ID = -1;
	typedef float Speed;
	typedef __int8 Code;

	template<typename T>
	struct _Dim
	{
		typedef typename T Type;
		typedef typename T base;
		Type width;
		Type height;
		inline bool operator==(const _Dim &size) const
		{
			return width == size.width && height == size.height;
		}
		inline bool operator!=(const _Dim &size) const
		{
			return width != size.width || height != size.height;
		}
		static const _Dim<T> Invalid;
		inline operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << width << " * " << height << " )";
			return os;
		}
	};
	template<typename T>
	const _Dim<T> _Dim<T>::Invalid = {std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()};

	template<typename T>
	struct _Pos
	{
		typedef typename T Type;
		typedef typename T base;
		Type x;
		Type y;
		inline bool operator==(const _Pos &pos) const
		{
			return x == pos.x && y == pos.y;
		}
		inline bool operator!=(const _Pos &pos) const
		{
			return x != pos.x || y != pos.y;
		}
		static const _Pos<T> Invalid;
		inline operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << x << " * " << y << " )";
			return os;
		}
	};
	template<typename T>
	const _Pos<T> _Pos<T>::Invalid = {std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()};

	typedef _Dim<__int32> Size;
	typedef _Pos<__int32> Coord;
	typedef _Pos<float> Move;
	typedef _Pos<float> Camera;
	typedef _Dim<float> CameraSize;
}

enum GridFlags: Type::Flags8
{
	Gravity = 1 << 0,
	Detonate = 1 << 1,
	SpawnPoint = 1 << 2,
};
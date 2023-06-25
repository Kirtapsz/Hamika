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


	typedef std::uint8_t Direction;
	namespace Directions
	{
		constexpr Direction up = 0;
		constexpr Direction right = 1;
		constexpr Direction down = 2;
		constexpr Direction left = 3;
	}

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

	template<typename T, typename U>
	struct ArithmeticType
	{
		using ADDITION = decltype(std::declval<T>() / std::declval<U>());
		using SUBTRACTION = decltype(std::declval<T>() / std::declval<U>());
		using MULTIPLICATION = decltype(std::declval<T>() / std::declval<U>());
		using DIVISION = decltype(std::declval<T>() / std::declval<U>());
		using MODULO = decltype(std::declval<T>() / std::declval<U>());
	};

	template<typename T>
	struct _Dim
	{
		typedef typename T base;

		base width;
		base height;

		inline _Dim()
		{

		}

		template<typename F>
		inline _Dim(_Dim<F> _dim):
			width((T)_dim.width), height((T)_dim.height)
		{

		}
		template<typename F>
		inline auto operator+(_Dim<F> _dim) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::ADDITION
			>(width + _dim.width, height + _dim.height);
		}
		template<typename F>
		inline auto operator-(_Dim<F> _dim) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::SUBTRACTION
			>(width - _dim.width, height - _dim.height);
		}
		template<typename F>
		inline _Dim<F> operator*(_Dim<F> _dim) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::MULTIPLICATION
			>(width * _dim.width, height * _dim.height);
		}
		template<typename F>
		inline _Dim<F> operator/(_Dim<F> _dim) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::DIVISION
			>(width / _dim.width, height / _dim.height);
		}
		template<typename F>
		inline _Dim<F> operator%(_Dim<F> _dim) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::MODULO
			>(width % _dim.width, height % _dim.height);
		}
		template<typename F>
		inline void operator+=(_Dim<F> _dim)
		{
			width += _dim.width;
			height += _dim.height;
		}
		template<typename F>
		inline void operator-=(_Dim<F> _dim)
		{
			width -= _dim.width;
			height -= _dim.height;
		}
		template<typename F>
		inline void operator*=(_Dim<F> _dim)
		{
			width *= _dim.width;
			height *= _dim.height;
		}
		template<typename F>
		inline void operator/=(_Dim<F> _dim)
		{
			width /= _dim.width;
			height /= _dim.height;
		}
		template<typename F>
		inline void operator%=(_Dim<F> _dim)
		{
			width %= _dim.width;
			height %= _dim.height;
		}
		template<typename F>
		inline bool operator==(_Dim<F> _dim) const
		{
			return width == (T)_dim.width && height == (T)_dim.height;
		}
		template<typename F>
		inline bool operator!=(_Dim<F> _dim) const
		{
			return width != (T)_dim.width || height != (T)_dim.height;
		}
		template<typename F>
		inline bool operator<(_Dim<F> _dim) const
		{
			return width < (T)_dim.width && height < (T)_dim.height;
		}
		template<typename F>
		inline bool operator>(_Dim<F> _dim) const
		{
			return width > (T)_dim.width && height > (T)_dim.height;
		}


		template<typename F>
		inline _Dim(F _width, F _height):
			width((T)_width), height((T)_height)
		{

		}
		template<typename F>
		inline auto operator+(F _b) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::ADDITION
			>(width + _b, height + _b);
		}
		template<typename F>
		inline auto operator-(F _b) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::SUBTRACTION
			>(width - _b, height - _b);
		}
		template<typename F>
		inline _Dim<F> operator*(F _b) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::MULTIPLICATION
			>(width * _b, height * _b);
		}
		template<typename F>
		inline _Dim<F> operator/(F _b) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::DIVISION
			>(width / _b, height / _b);
		}
		template<typename F>
		inline _Dim<F> operator%(F _b) const
		{
			return _Dim<
				typename ArithmeticType<T, F>::MODULO
			>(width % _b, height % _b);
		}
		template<typename F>
		inline void operator+=(F _b)
		{
			width += _b;
			height += _b;
		}
		template<typename F>
		inline void operator-=(F _b)
		{
			width -= _b;
			height -= _b;
		}
		template<typename F>
		inline void operator*=(F _b)
		{
			width *= _b;
			height *= _b;
		}
		template<typename F>
		inline void operator/=(F _b)
		{
			width /= _b;
			height /= _b;
		}
		template<typename F>
		inline void operator%=(F _b)
		{
			width %= _b;
			height %= _b;
		}
		template<typename F>
		inline bool operator==(F _b) const
		{
			return width == (T)_b && height == (T)_b;
		}
		template<typename F>
		inline bool operator!=(F _b) const
		{
			return width != (T)_b || height != (T)_b;
		}
		template<typename F>
		inline bool operator<(F _b) const
		{
			return width < (T)_b && height < (T)_b;
		}
		template<typename F>
		inline bool operator>(F _b) const
		{
			return width > (T)_b && height > (T)_b;
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
		typedef typename T base;

		base x;
		base y;

		inline _Pos()
		{

		}

		template<typename F>
		inline _Pos(_Pos<F> _pos):
			x((T)_pos.x), y((T)_pos.y)
		{

		}
		template<typename F>
		inline auto operator+(_Pos<F> _pos) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::ADDITION
			>(x + _pos.x, y + _pos.y);
		}
		template<typename F>
		inline auto operator-(_Pos<F> _pos) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::SUBTRACTION
			>(x - _pos.x, y - _pos.y);
		}
		template<typename F>
		inline _Pos<F> operator*(_Pos<F> _pos) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::MULTIPLICATION
			>(x * _pos.x, y * _pos.y);
		}
		template<typename F>
		inline _Pos<F> operator/(_Pos<F> _pos) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::DIVISION
			>(x / _pos.x, y / _pos.y);
		}
		template<typename F>
		inline _Pos<F> operator%(_Pos<F> _pos) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::MODULO
			>(x % _pos.x, y % _pos.y);
		}
		template<typename F>
		inline void operator+=(_Pos<F> _pos)
		{
			x += _pos.x;
			y += _pos.y;
		}
		template<typename F>
		inline void operator-=(_Pos<F> _pos)
		{
			x -= _pos.x;
			y -= _pos.y;
		}
		template<typename F>
		inline void operator*=(_Pos<F> _pos)
		{
			x *= _pos.x;
			y *= _pos.y;
		}
		template<typename F>
		inline void operator/=(_Pos<F> _pos)
		{
			x /= _pos.x;
			y /= _pos.y;
		}
		template<typename F>
		inline void operator%=(_Pos<F> _pos)
		{
			x %= _pos.x;
			y %= _pos.y;
		}
		template<typename F>
		inline bool operator==(_Pos<F> _pos) const
		{
			return x == (T)_pos.x && y == (T)_pos.y;
		}
		template<typename F>
		inline bool operator!=(_Pos<F> _pos) const
		{
			return x != (T)_pos.x || y != (T)_pos.y;
		}
		template<typename F>
		inline bool operator<(_Pos<F> _pos) const
		{
			return x < (T)_pos.x && y < (T)_pos.y;
		}
		template<typename F>
		inline bool operator>(_Pos<F> _pos) const
		{
			return x > (T)_pos.x && y > (T)_pos.y;
		}


		template<typename F>
		inline _Pos(F _x, F _y):
			x((T)_x), y((T)_y)
		{

		}
		template<typename F>
		inline auto operator+(F _b) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::ADDITION
			>(x + _b, y + _b);
		}
		template<typename F>
		inline auto operator-(F _b) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::SUBTRACTION
			>(x - _b, y - _b);
		}
		template<typename F>
		inline _Pos<F> operator*(F _b) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::MULTIPLICATION
			>(x * _b, y * _b);
		}
		template<typename F>
		inline _Pos<F> operator/(F _b) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::DIVISION
			>(x / _b, y / _b);
		}
		template<typename F>
		inline _Pos<F> operator%(F _b) const
		{
			return _Pos<
				typename ArithmeticType<T, F>::MODULO
			>(x % _b, y % _b);
		}
		template<typename F>
		inline void operator+=(F _b)
		{
			x += _b;
			y += _b;
		}
		template<typename F>
		inline void operator-=(F _b)
		{
			x -= _b;
			y -= _b;
		}
		template<typename F>
		inline void operator*=(F _b)
		{
			x *= _b;
			y *= _b;
		}
		template<typename F>
		inline void operator/=(F _b)
		{
			x /= _b;
			y /= _b;
		}
		template<typename F>
		inline void operator%=(F _b)
		{
			x %= _b;
			y %= _b;
		}
		template<typename F>
		inline bool operator==(F _b) const
		{
			return x == (T)_b && y == (T)_b;
		}
		template<typename F>
		inline bool operator!=(F _b) const
		{
			return x != (T)_b || y != (T)_b;
		}
		template<typename F>
		inline bool operator<(F _b) const
		{
			return x < (T)_b && y < (T)_b;
		}
		template<typename F>
		inline bool operator>(F _b) const
		{
			return x > (T)_b && y > (T)_b;
		}

		static const _Pos<T> Invalid;
		inline operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << x << " * " << y << " )";
			return os;
		}
		inline void limiter(const _Pos &_min, const _Pos &_max)
		{
			x = std::max(_min.x, std::min(_max.x, x));
			y = std::max(_min.y, std::min(_max.y, y));
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
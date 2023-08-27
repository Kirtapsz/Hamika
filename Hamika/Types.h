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

	template<class T>
	struct ModuloType;

	template<>
	struct ModuloType<float>
	{
		typedef typename __int32 TYPE;
	};

	template<class T>
	struct ModuloType
	{
		typedef typename T TYPE;
	};

	template<typename T, typename U>
	struct ArithmeticType
	{
		using ADDITION = decltype(std::declval<T>() + std::declval<U>());
		using SUBTRACTION = decltype(std::declval<T>() - std::declval<U>());
		using MULTIPLICATION = decltype(std::declval<T>() *std::declval<U>());
		using DIVISION = decltype(std::declval<T>() / std::declval<U>());

		using MODULO = decltype(std::declval<typename ModuloType<T>::TYPE>() % std::declval<typename ModuloType<U>::TYPE>());
	};

	template<typename T>
	struct var2D
	{
		typedef typename T base;

		base valuel;
		base valuer;

		constexpr base &x()
		{
			return valuel;
		}
		constexpr base &y()
		{
			return valuer;
		}
		constexpr base &width()
		{
			return valuel;
		}
		constexpr base &height()
		{
			return valuer;
		}

		constexpr const base &x() const
		{
			return valuel;
		}
		constexpr const base &y() const
		{
			return valuer;
		}
		constexpr const base &width() const
		{
			return valuel;
		}
		constexpr const base &height() const
		{
			return valuer;
		}

		constexpr var2D()
		{

		}

		template<typename F>
		constexpr var2D(F _valuel, F _valuer):
			valuel((T)_valuel), valuer((T)_valuer)
		{

		}
		template<typename F>
		constexpr auto operator+(F _b) const
		{
			return var2D<
				typename ArithmeticType<T, F>::ADDITION
			>(valuel + _b, valuer + _b);
		}
		template<typename F>
		constexpr auto operator-(F _b) const
		{
			return var2D<
				typename ArithmeticType<T, F>::SUBTRACTION
			>(valuel - _b, valuer - _b);
		}
		template<typename F>
		constexpr var2D<F> operator*(F _b) const
		{
			return var2D<
				typename ArithmeticType<T, F>::MULTIPLICATION
			>(valuel * _b, valuer * _b);
		}
		template<typename F>
		constexpr var2D<F> operator/(F _b) const
		{
			return var2D<
				typename ArithmeticType<T, F>::DIVISION
			>(valuel / _b, valuer / _b);
		}
		template<typename F>
		constexpr var2D<F> operator%(F _b) const
		{
			return var2D<
				typename ArithmeticType<T, F>::MODULO
			>(valuel % _b, valuer % _b);
		}
		template<typename F>
		constexpr void operator+=(F _b)
		{
			valuel += _b;
			valuer += _b;
		}
		template<typename F>
		constexpr void operator-=(F _b)
		{
			valuel -= _b;
			valuer -= _b;
		}
		template<typename F>
		constexpr void operator*=(F _b)
		{
			valuel *= _b;
			valuer *= _b;
		}
		template<typename F>
		constexpr void operator/=(F _b)
		{
			valuel /= _b;
			valuer /= _b;
		}
		template<typename F>
		constexpr void operator%=(F _b)
		{
			valuel %= _b;
			valuer %= _b;
		}
		template<typename F>
		constexpr bool operator==(F _b) const
		{
			return valuel == (T)_b && valuer == (T)_b;
		}
		template<typename F>
		constexpr bool operator!=(F _b) const
		{
			return valuel != (T)_b || valuer != (T)_b;
		}
		template<typename F>
		constexpr bool operator<(F _b) const
		{
			return valuel < (T)_b && valuer < (T)_b;
		}
		template<typename F>
		constexpr bool operator<=(F _b) const
		{
			return valuel <= (T)_b && valuer <= (T)_b;
		}
		template<typename F>
		constexpr bool operator>(F _b) const
		{
			return valuel > (T)_b && valuer > (T)_b;
		}
		template<typename F>
		constexpr bool operator>=(F _b) const
		{
			return valuel >= (T)_b && valuer >= (T)_b;
		}

		template<typename F>
		constexpr var2D(const var2D<F> &_val):
			valuel((T)_val.valuel), valuer((T)_val.valuer)
		{

		}
		template<typename F>
		constexpr auto operator+(const var2D<F> &_val) const
		{
			return var2D<
				typename ArithmeticType<T, F>::ADDITION
			>(valuel + _val.valuel, valuer + _val.valuer);
		}
		template<typename F>
		constexpr auto operator-(const var2D<F> &_val) const
		{
			return var2D<
				typename ArithmeticType<T, F>::SUBTRACTION
			>(valuel - _val.valuel, valuer - _val.valuer);
		}
		template<typename F>
		constexpr auto operator*(const var2D<F> &_val) const
		{
			return var2D<
				typename ArithmeticType<T, F>::MULTIPLICATION
			>(valuel * _val.valuel, valuer * _val.valuer);
		}
		template<typename F>
		constexpr auto operator/(const var2D<F> &_val) const
		{
			return var2D<
				typename ArithmeticType<T, F>::DIVISION
			>(valuel / _val.valuel, valuer / _val.valuer);
		}
		template<typename F>
		constexpr auto operator%(const var2D<F> &_val) const
		{
			return var2D<
				typename ArithmeticType<T, F>::MODULO
			>(valuel % _val.valuel, valuer % _val.valuer);
		}
		template<typename F>
		constexpr void operator+=(const var2D<F> &_val)
		{
			valuel += _val.valuel;
			valuer += _val.valuer;
		}
		template<typename F>
		constexpr void operator-=(const var2D<F> &_val)
		{
			valuel -= _val.valuel;
			valuer -= _val.valuer;
		}
		template<typename F>
		constexpr void operator*=(const var2D<F> &_val)
		{
			valuel *= _val.valuel;
			valuer *= _val.valuer;
		}
		template<typename F>
		constexpr void operator/=(const var2D<F> &_val)
		{
			valuel /= _val.valuel;
			valuer /= _val.valuer;
		}
		template<typename F>
		constexpr void operator%=(const var2D<F> &_val)
		{
			valuel %= _val.valuel;
			valuer %= _val.valuer;
		}
		template<typename F>
		constexpr bool operator==(const var2D<F> &_val) const
		{
			return valuel == (T)_val.valuel && valuer == (T)_val.valuer;
		}
		template<typename F>
		constexpr bool operator!=(const var2D<F> &_val) const
		{
			return valuel != (T)_val.valuel || valuer != (T)_val.valuer;
		}
		template<typename F>
		constexpr bool operator<(const var2D<F> &_val) const
		{
			return valuel < (T)_val.valuel && valuer < (T)_val.valuer;
		}
		template<typename F>
		constexpr bool operator<=(const var2D<F> &_val) const
		{
			return valuel <= (T)_val.valuel && valuer <= (T)_val.valuer;
		}
		template<typename F>
		constexpr bool operator>(const var2D<F> &_val) const
		{
			return valuel > (T)_val.valuel && valuer > (T)_val.valuer;
		}
		template<typename F>
		constexpr bool operator>=(const var2D<F> &_val) const
		{
			return valuel >= (T)_val.valuel && valuer >= (T)_val.valuer;
		}

		template<typename F>
		constexpr void limiter(const var2D<F> &_min, const var2D<F> &_max)
		{
			valuel = std::max((T)_min.valuel, std::min((T)_max.valuel, valuel));
			valuer = std::max((T)_min.valuer, std::min((T)_max.valuer, valuer));
		}

		template<typename F>
		constexpr void limit(const var2D<F> &_min, const var2D<F> &_max)
		{
			return limiter<F>(_min, _max);
		}


		// open interval [ x , y [
		template<typename F>
		constexpr bool isInside(const var2D<F> &_p1, const var2D<F> &_p2)
		{
			const var2D<F> &p_min{std::min(_p1.valuel, _p2.valuel), std::min(_p1.valuer, _p2.valuer)};
			const var2D<F> &p_max{std::max(_p1.valuel, _p2.valuel), std::max(_p1.valuer, _p2.valuer)};

			return *this >= p_min && *this < p_max;
		}

		static const var2D<T> Invalid;
		constexpr operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "( " << valuel << " * " << valuer << " )";
			return os;
		}

		constexpr std::string str() const
		{
			std::ostringstream os;
			os << "( " << valuel << " ; " << valuer << " )";
			return os.str();
		}
	};
	template<typename T>
	const var2D<T> var2D<T>::Invalid = {std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()};

	typedef var2D<__int32> Size;
	typedef var2D<__int32> Coord;
	typedef var2D<float> Move;
	typedef var2D<float> Camera;
	typedef var2D<float> CameraSize;
}

enum GridFlags: Type::Flags8
{
	Gravity = 1 << 0,
	Detonate = 1 << 1,
	SpawnPoint = 1 << 2,
};
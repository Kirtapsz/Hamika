#pragma once

#include <ostream>
#include <sstream>
#include <limits>

#undef None

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
		using base = T;

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
		constexpr base &w()
		{
			return valuel;
		}
		constexpr base &h()
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
		constexpr const base &w() const
		{
			return valuel;
		}
		constexpr const base &h() const
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
		constexpr var2D<T> &operator=(F _b)
		{
			valuel = _b;
			valuer = _b;
			return *this;
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
		constexpr var2D<T> &operator=(const var2D<F> &_val)
		{
			valuel = _val.valuel;
			valuer = _val.valuer;
			return *this;
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
			valuel = (std::max)((T)_min.valuel, (std::min)((T)_max.valuel, valuel));
			valuer = (std::max)((T)_min.valuer, (std::min)((T)_max.valuer, valuer));
		}

		template<typename F>
		constexpr void limit(const var2D<F> &_min, const var2D<F> &_max)
		{
			return limiter<F>(_min, _max);
		}

		constexpr base(max)() const
		{
			return (std::max)(valuel, valuer);
		}

		constexpr base(min)() const
		{
			return (std::min)(valuel, valuer);
		}

		template<typename F = T>
		constexpr var2D<F> ceil() const
		{
			return var2D<F>(std::ceil(valuel), std::ceil(valuer));
		}

		template<typename F = T>
		constexpr var2D<F> floor() const
		{
			return var2D<F>(std::floor(valuel), std::floor(valuer));
		}

		template<typename F = T>
		constexpr var2D<F> abs() const
		{
			return var2D<F>(std::abs(valuel), std::abs(valuer));
		}

		template<typename F = T>
		constexpr var2D<F> modf() const
		{
			var2D<F> i_part;
			var2D<F> f_part{
				std::modf(valuel, &i_part.valuel),
				std::modf(valuer, &i_part.valuer)};
			return f_part;
		}

		constexpr void wholeUp()
		{
			if (valuel % 2 == 0) ++valuel;
			if (valuer % 2 == 0) ++valuer;
		}

		constexpr void wholeDown()
		{
			if (valuel % 2 == 0) --valuel;
			if (valuer % 2 == 0) --valuer;
		}

		constexpr void oddUp()
		{
			if (valuel % 2 != 0) ++valuel;
			if (valuer % 2 != 0) ++valuer;
		}

		constexpr void oddDown()
		{
			if (valuel % 2 != 0) --valuel;
			if (valuer % 2 != 0) --valuer;
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
			return std::ostringstream(*this).str();
		}

		template <typename Func, typename... Args>
		static auto ref(Func algorithm, Args... values) -> decltype(algorithm(values...))
		{
			return algorithm(values...);
		}

		template <typename F, typename... A>
		static var2D<T> apply(F algorithm, const A&... a)
		{
			return {
				var2D<T>::ref(algorithm, a.valuel...),
				var2D<T>::ref(algorithm, a.valuer...)
			};
		}

		template <typename F, typename... A>
		static void exec(F algorithm, const A&... a)
		{
			var2D<T>::ref(algorithm, a.valuel...);
			var2D<T>::ref(algorithm, a.valuer...);
		}

		template<typename F, typename G>
		static var2D<T>(max)(const var2D<F> &_p, G v)
		{
			return {
				std::max<T>(_p.valuel, v),
				std::max<T>(_p.valuer, v),
			};
		}
		template<typename F, typename G>
		static var2D<T>(max)(const var2D<F> &_p1, const var2D<G> &_p2)
		{
			return {
				std::max<T>(_p1.valuel, _p2.valuel),
				std::max<T>(_p1.valuer, _p2.valuer),
			};
		}

		template<typename F, typename G>
		static var2D<T>(min)(const var2D<F> &_p, G v)
		{
			return {
				std::min<T>(_p.valuel, v),
				std::min<T>(_p.valuer, v),
			};
		}
		template<typename F, typename G>
		static var2D<T>(min)(const var2D<F> &_p1, const var2D<G> &_p2)
		{
			return {
				std::min<T>(_p1.valuel, _p2.valuel),
				std::min<T>(_p1.valuer, _p2.valuer),
			};
		}
	};
	template<typename T>
	const var2D<T> var2D<T>::Invalid = {std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()};

	typedef var2D<__int32> Size;
	typedef var2D<__int32> Pixels;
	typedef var2D<__int32> Coord;
	typedef var2D<float> Move;
	typedef var2D<float> Camera;
	typedef var2D<float> CameraSize;

	template<typename T>
	struct rec2D
	{
		T p1;
		T p2;

		inline bool hasArea() const
		{
			return p2 - p1 > 0;
		}

		template<typename F>
		inline bool isSubset(const rec2D<F> &_rec) const
		{
			return _rec.p1 <= p1 && _rec.p2 >= p2;
		}
		template<typename F>
		inline bool isInside(const var2D<F> &_p) const
		{
			return p1 <= _p && p2 > _p;
		}

		constexpr const typename T::base &x() const
		{
			return p1.valuel;
		}
		constexpr const typename T::base &y() const
		{
			return p1.valuer;
		}
		inline typename T::base width() const
		{
			return p2.valuel - p1.valuel;
		}
		inline typename T::base height() const
		{
			return p2.valuer - p1.valuer;
		}
		inline typename T::base w() const
		{
			return p2.valuel - p1.valuel;
		}
		inline typename T::base h() const
		{
			return p2.valuer - p1.valuer;
		}

		static const var2D<T> Invalid;
		constexpr operator std::ostringstream() const
		{
			std::ostringstream os;
			os << "[ " << p1.str() << " - " << p2.str() << " ; " << (p2 - p1).str();
			return os;
		}

		constexpr std::string str() const
		{
			return std::ostringstream(*this).str();
		}
	};
}

typedef std::uint8_t DrawNumber;

enum UpdateType: std::uint8_t
{
	UPDATE_ASC = 1 << 0,
	UPDATE_DESC = 1 << 1,
	UPDATE_MURPHY = 1 << 2,
};

enum GridFlags: Type::Flags8
{
	Gravity = 1 << 0,
	Detonate = 1 << 1,
	SpawnPoint = 1 << 2,
};

namespace RedrawType
{
	constexpr Type::Flags None = 0;

	constexpr Type::Flags Object = 1 << 0;
	constexpr Type::Flags Shift = 1 << 1;

	constexpr Type::Flags All = Object | Shift;
}

namespace DrawnedType
{
	constexpr Type::Flags None = 0;

	constexpr Type::Flags Cleared = 1 << 0;

	constexpr Type::Flags TopStand = 1 << 1;
	constexpr Type::Flags TopRemain = 1 << 2;
	constexpr Type::Flags StandardMove = 1 << 3;
	constexpr Type::Flags StandardRemain = 1 << 4;
	constexpr Type::Flags StandardGoTo = 1 << 5;

	constexpr Type::Flags Drawned = TopStand | TopRemain | StandardMove | StandardRemain | StandardGoTo;
}
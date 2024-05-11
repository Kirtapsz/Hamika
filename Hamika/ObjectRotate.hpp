#ifndef OBJECT_ROTATE_HPP
#define OBJECT_ROTATE_HPP

#include "ObjectRotate.h"

namespace Object
{
	namespace Module
	{
		namespace Rotate
		{
			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.rotation = Type::Rotations::_0;
				data_.straight_rotation = true;
				data_.rotationSpeed_ = 1.f;
				data_.carryRotation_ = 0.f;
			}
			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["rotation"] = data_.rotation;
				json["rotationSpeed"] = data_.rotationSpeed_;

				return json;
			}

			//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 360 egy teljes k�rbefordul�s 1m�sodperc alatt...
			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetRotationSpeed(Type::Speed speed)
			{
				data_.rotationSpeed_ = speed;
			}
			template<typename MODULES_T>
			void Func<MODULES_T>::doRotate(std::uint8_t _action)
			{
				data_.straight_rotation = false;

				data_.action = _action;
				switch (_action)
				{
					case MODULES_T::DATA_T::ROTATE_LEFT:
					{
						break;
					}
					case MODULES_T::DATA_T::ROTATE_RIGHT:
					{
						break;
					}
				}
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::finishRotate()
			{
				data_.carryRotation_ = 0.f;
				data_.action = MODULES_T::DATA_T::STEADY;
			}

			template<typename MODULES_T>
			inline bool Func<MODULES_T>::isRotate() const
			{
				return !data_.straight_rotation;
			}

			template<typename MODULES_T>
			void Func<MODULES_T>::RotationLeft()
			{
				if (data_.rotation == Type::Rotations::_0)
					data_.rotation = Type::Rotations::_360;
				if (data_.rotation > Type::Rotations::_270)
				{
					data_.rotation -= data_.rotationSpeed_ / CPS;
					if (data_.rotation <= Type::Rotations::_270)
					{
						data_.carryRotation_ = Type::Rotations::_270 - data_.rotation;
						data_.rotation = Type::Rotations::_270;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
				else if (data_.rotation > Type::Rotations::_180)
				{
					data_.rotation -= data_.rotationSpeed_ / CPS;
					if (data_.rotation <= Type::Rotations::_180)
					{
						data_.carryRotation_ = Type::Rotations::_180 - data_.rotation;
						data_.rotation = Type::Rotations::_180;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
				else if (data_.rotation > Type::Rotations::_90)
				{
					data_.rotation -= data_.rotationSpeed_ / CPS;
					if (data_.rotation <= Type::Rotations::_90)
					{
						data_.carryRotation_ = Type::Rotations::_90 - data_.rotation;
						data_.rotation = Type::Rotations::_90;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
				else
				{
					data_.rotation -= data_.rotationSpeed_ / CPS;
					if (data_.rotation <= Type::Rotations::_0)
					{
						data_.carryRotation_ = Type::Rotations::_0 - data_.rotation;
						data_.rotation = Type::Rotations::_0;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
			}
			template<typename MODULES_T>
			void Func<MODULES_T>::RotationRight()
			{
				if (data_.rotation < Type::Rotations::_90)
				{
					data_.rotation += data_.rotationSpeed_ / CPS;
					if (data_.rotation >= Type::Rotations::_90)
					{
						data_.carryRotation_ = data_.rotation - Type::Rotations::_90;
						data_.rotation = Type::Rotations::_90;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
				else if (data_.rotation < Type::Rotations::_180)
				{
					data_.rotation += data_.rotationSpeed_ / CPS;
					if (data_.rotation >= Type::Rotations::_180)
					{
						data_.carryRotation_ = data_.rotation - Type::Rotations::_180;
						data_.rotation = Type::Rotations::_180;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
				else if (data_.rotation < Type::Rotations::_270)
				{
					data_.rotation += data_.rotationSpeed_ / CPS;
					if (data_.rotation >= Type::Rotations::_270)
					{
						data_.carryRotation_ = data_.rotation - Type::Rotations::_270;
						data_.rotation = Type::Rotations::_270;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
				else
				{
					data_.rotation += data_.rotationSpeed_ / CPS;
					if (data_.rotation >= Type::Rotations::_360)
					{
						data_.carryRotation_ = data_.rotation - Type::Rotations::_360;
						data_.rotation = Type::Rotations::_0;
						data_.straight_rotation = true;
						data_.scene->ObjectArrived(func_.GetCoord());
					}
				}
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::SetRotation(Type::Rotation rotation)
			{
				data_.rotation = rotation;
			}
			template<typename MODULES_T>
			inline Type::Rotation Func<MODULES_T>::GetRoundRotation(Type::Rotation rotation)
			{
				rotation += 0.01f/*hibat�r�s*/;

				if (rotation >= Type::Rotations::_315)
					return Type::Rotations::Up;
				else if (rotation >= Type::Rotations::_225)
					return Type::Rotations::Left;
				else if (rotation >= Type::Rotations::_135)
					return Type::Rotations::Down;
				else if (rotation >= Type::Rotations::_45)
					return Type::Rotations::Right;
				else
					return Type::Rotations::Up;
			}
			template<typename MODULES_T>
			inline Type::Rotation Func<MODULES_T>::GetAngel()
			{
				return data_.rotation / Type::Rotations::_360;
			}

			template<typename MODULES_T>
			inline Type::Rotation Func<MODULES_T>::GetRealRotation(Type::Rotation rotation)
			{
				if (rotation < 0)
					rotation += (int)(rotation / Type::Rotations::_360) * Type::Rotations::_360 + Type::Rotations::_360;
				else if (rotation > Type::Rotations::_360)
					rotation -= (int)(rotation / Type::Rotations::_360) * Type::Rotations::_360;
				return rotation;
			}
			template<typename MODULES_T>
			inline Type::Rotation Func<MODULES_T>::GetRotation() const
			{
				return data_.rotation;
			}
		}
	}
}

#endif
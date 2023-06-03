#ifndef OBJECT_ROTATE_HPP
#define OBJECT_ROTATE_HPP

#include "ObjectRotate.h"

namespace Object
{
	namespace Module
	{
		namespace Rotate
		{
			template <typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				rotation = Type::Rotations::_0;
				straight_rotation = true;
				rotationSpeed_ = 1.f;
				carryRotation_ = 0.f;
			}
			template <typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["rotation"] = rotation;
				json["rotationSpeed"] = rotationSpeed_;

				return json;
			}

			//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 360 egy teljes k�rbefordul�s 1m�sodperc alatt...
			template <typename DATA>
			void Func<DATA>::SetRotationSpeed(Type::Speed speed)
			{
				rotationSpeed_ = speed;
			}
			template <typename DATA>
			void Func<DATA>::doRotate(std::uint8_t _action)
			{
				straight_rotation = false;

				action = _action;
				switch (_action)
				{
					case ROTATE_LEFT:
					{
						break;
					}
					case ROTATE_RIGHT:
					{
						break;
					}
				}
			}
			template <typename DATA>
			void Func<DATA>::finishRotate()
			{
				carryRotation_ = 0.f;
				_action = STEADY;
			}

			template <typename DATA>
			bool Func<DATA>::isRotate() const
			{
				return !straight_rotation;
			}

			template <typename DATA>
			void Func<DATA>::RotationLeft()
			{
				if (rotation == Type::Rotations::_0)
					rotation = Type::Rotations::_360;
				if (rotation > Type::Rotations::_270)
				{
					rotation -= rotationSpeed_ / CPS;
					if (rotation <= Type::Rotations::_270)
					{
						carryRotation_ = Type::Rotations::_270 - rotation;
						rotation = Type::Rotations::_270;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
				else if (rotation > Type::Rotations::_180)
				{
					rotation -= rotationSpeed_ / CPS;
					if (rotation <= Type::Rotations::_180)
					{
						carryRotation_ = Type::Rotations::_180 - rotation;
						rotation = Type::Rotations::_180;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
				else if (rotation > Type::Rotations::_90)
				{
					rotation -= rotationSpeed_ / CPS;
					if (rotation <= Type::Rotations::_90)
					{
						carryRotation_ = Type::Rotations::_90 - rotation;
						rotation = Type::Rotations::_90;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
				else
				{
					rotation -= rotationSpeed_ / CPS;
					if (rotation <= Type::Rotations::_0)
					{
						carryRotation_ = Type::Rotations::_0 - rotation;
						rotation = Type::Rotations::_0;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
			}
			template <typename DATA>
			void Func<DATA>::RotationRight()
			{
				if (rotation < Type::Rotations::_90)
				{
					rotation += rotationSpeed_ / CPS;
					if (rotation >= Type::Rotations::_90)
					{
						carryRotation_ = rotation - Type::Rotations::_90;
						rotation = Type::Rotations::_90;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
				else if (rotation < Type::Rotations::_180)
				{
					rotation += rotationSpeed_ / CPS;
					if (rotation >= Type::Rotations::_180)
					{
						carryRotation_ = rotation - Type::Rotations::_180;
						rotation = Type::Rotations::_180;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
				else if (rotation < Type::Rotations::_270)
				{
					rotation += rotationSpeed_ / CPS;
					if (rotation >= Type::Rotations::_270)
					{
						carryRotation_ = rotation - Type::Rotations::_270;
						rotation = Type::Rotations::_270;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
				else
				{
					rotation += rotationSpeed_ / CPS;
					if (rotation >= Type::Rotations::_360)
					{
						carryRotation_ = rotation - Type::Rotations::_360;
						rotation = Type::Rotations::_0;
						straight_rotation = true;
						scene->ObjectArrived(GetCoord());
					}
				}
			}
			template <typename DATA>
			void Func<DATA>::SetRotation(Type::Rotation rotation)
			{
				this->rotation = rotation;
			}
			template <typename DATA>
			Type::Rotation Func<DATA>::GetRoundRotation(Type::Rotation rotation)
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
			template <typename DATA>
			Type::Rotation Func<DATA>::GetAngel()
			{
				return rotation / Type::Rotations::_360;
			}

			template <typename DATA>
			Type::Rotation Func<DATA>::GetRealRotation(Type::Rotation rotation)
			{
				if (rotation < 0)
					rotation += (int)(rotation / Type::Rotations::_360) * Type::Rotations::_360 + Type::Rotations::_360;
				else if (rotation > Type::Rotations::_360)
					rotation -= (int)(rotation / Type::Rotations::_360) * Type::Rotations::_360;
				return rotation;
			}
			template <typename DATA>
			Type::Rotation Func<DATA>::GetRotation() const
			{
				return rotation;
			}
		}
	}
}

#endif
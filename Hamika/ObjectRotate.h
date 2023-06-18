#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#include "ObjectActions.h"
#include "ObjectCoord.h"

namespace Object
{
	namespace Module
	{
		namespace Rotate
		{
			template <typename OBJECT>
			struct Data
			{
				Type::Rotation rotation = 0;
				bool straight_rotation = true;
				Type::Speed rotationSpeed_ = 1;
				Type::Speed carryRotation_ = 1;
			};
			template <typename DATA>
			struct Func: virtual DATA,
				virtual Coord::Func<DATA>
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();

				static Type::Rotation GetRealRotation(Type::Rotation rotation);
				Type::Rotation GetRotation() const;

				//aut�matikusan a megfelel� ir�nyba l�pteti, true visszat�r�s ha meg�rkezett
				//akkor kell megh�vni amikor a ford�st el akarjuk kezdeni, m�g a l�ptet�s el�tt
				void doRotate(std::uint8_t _action);
				void finishRotate();

				bool isRotate() const;

				void RotationLeft();
				void RotationRight();
				//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 360 egy teljes k�rbefordul�s 1m�sodperc alatt...
				void SetRotationSpeed(Type::Speed speed);

				void SetRotation(Type::Rotation rotation);

				Type::Rotation GetAngel();
				Type::Rotation GetRoundRotation(Type::Rotation rotation);
			};
		}
	}
}

#include "ObjectRotate.hpp"
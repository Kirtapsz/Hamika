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

				//autómatikusan a megfelelõ irányba lépteti, true visszatérés ha megérkezett
				//akkor kell meghívni amikor a fordást el akarjuk kezdeni, még a léptetés elött
				void doRotate(std::uint8_t _action);
				void finishRotate();

				bool isRotate() const;

				void RotationLeft();
				void RotationRight();
				//a mértékegység hogy 1 másodperc alatt mennyit haladjon, 360 egy teljes körbefordulás 1másodperc alatt...
				void SetRotationSpeed(Type::Speed speed);

				void SetRotation(Type::Rotation rotation);

				Type::Rotation GetAngel();
				Type::Rotation GetRoundRotation(Type::Rotation rotation);
			};
		}
	}
}

#include "ObjectRotate.hpp"
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
			struct Data
			{
				Type::Rotation rotation = 0;
				bool straight_rotation = true;
				Type::Speed rotationSpeed_ = 1;
				Type::Speed carryRotation_ = 1;
			};

			template<typename MODULES_T>
			struct Func
			{
				private: typename MODULES_T::DATA_T &data_;
				private: typename MODULES_T::FUNC_T &func_;
				public: Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func);

				public: void __init__(Type::ID id, Type::Coord coord);
				public: Json print();

				public: static Type::Rotation GetRealRotation(Type::Rotation rotation);
				public: Type::Rotation GetRotation() const;

				//aut�matikusan a megfelel� ir�nyba l�pteti, true visszat�r�s ha meg�rkezett
				//akkor kell megh�vni amikor a ford�st el akarjuk kezdeni, m�g a l�ptet�s el�tt
				public: void doRotate(std::uint8_t _action);
				public: void finishRotate();

				public: bool isRotate() const;

				public: void RotationLeft();
				public: void RotationRight();
				//a m�rt�kegys�g hogy 1 m�sodperc alatt mennyit haladjon, 360 egy teljes k�rbefordul�s 1m�sodperc alatt...
				public: void SetRotationSpeed(Type::Speed speed);

				public: void SetRotation(Type::Rotation rotation);

				public: Type::Rotation GetAngel();
				public: Type::Rotation GetRoundRotation(Type::Rotation rotation);
			};
		}
	}
}

#include "ObjectRotate.hpp"
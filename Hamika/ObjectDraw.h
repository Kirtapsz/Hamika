#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

#include "ObjectCoord.h"
#include "ObjectMove.h"
#include "ObjectRequests.h"
#include "ObjectStack.h"

namespace Object
{
	namespace Module
	{
		namespace Draw
		{
			template <typename OBJECT>
			struct Data: virtual Stack::Data<OBJECT>
			{
				unsigned long long int DrawNumber = 0;
				static unsigned long long int SObjectDrawCounts;
				unsigned long long int DrawnedCount = 0;
				typedef void(*StatisDrawType)(int, int, int, int);

				struct Interface
				{
					virtual Type::Size GetDrawSize() const = 0;
					virtual Type::Size GetDrawOffSet() const = 0;
				} *drawIef;

				Type::Coord DrawCoord = {0,0};

#define OBJECT_DRAWNER_PARAM Object::Brick::Stack *stack, Type::Coord::Type x,Type::Coord::Type y,Type::Coord::Type w,Type::Coord::Type h
#define OBJECT_DRAWNER_CALL stack, x, y, w, h
				typedef void(*DRAWNER)(Stack *, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h);
				DRAWNER drawnerFnc = nullptr;
				unsigned long long int drawCounter = 0;//ez az objektum h�nyszor lett eddig �jrarajzolva
				unsigned long long int drawAtOnceCounter = 0;//ez az objektum h�nyszor lett �jrarajzolva egy ciklus alatt
				unsigned long long int drawCounterID = 0;//az objektum h�nyadikk�nt lett rajzolva az �sszesb�l
				static unsigned long long int totalDrawCounter;//az �sszes objektum eddigi �jrarajzol�sa
			};

			template <typename DATA>
			struct Func: virtual DATA
			{
				void __init__(Type::ID id, Type::Coord coord);
				Json print();

				void configureDrawOptions();


				virtual void Draw();
				virtual void SDraw();//teszte�s c�lj�b�l hogy a conter m�k�dj�n
				virtual void setOddDrawCoord();
				virtual void setRoundDrawCoord();
				Type::Coord GetDrawCoord();

				//DRAW #####################################################################################

				void RunSDraw();//teszte�s c�lj�b�l hogy a conter m�k�dj�n
			};
		}
	}
}

#include "ObjectDraw.hpp"
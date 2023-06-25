#ifndef OBJECT_DRAW_HPP
#define OBJECT_DRAW_HPP

#include "ObjectDraw.h"

namespace Object
{
	namespace Module
	{
		namespace Draw
		{
			template<typename OBJECT>
			unsigned long long int Data<OBJECT>::SObjectDrawCounts = 0;

			template<typename OBJECT>
			unsigned long long int Data<OBJECT>::totalDrawCounter = 0;


			template<typename DATA>
			void Func<DATA>::__init__(Type::ID id, Type::Coord coord)
			{
				drawnerFnc = nullptr;
				configureDrawOptions();
			}

			template<typename DATA>
			Json Func<DATA>::print()
			{
				Json json;

				json["DrawCoord.x"] = DrawCoord.x;
				json["DrawCoord.y"] = DrawCoord.y;

				return json;
			}

			template<typename DATA>
			void Func<DATA>::configureDrawOptions()
			{
				DrawCoord.x = coord.x * scene->GetDrawSize().width;
				DrawCoord.y = coord.y * scene->GetDrawSize().height;
			}


			template<typename DATA>
			void Func<DATA>::Draw()
			{
				if (drawnerFnc)
				{
					Stack stack(dynamic_cast<typename DATA::OBJECT_T *>(this));
					drawnerFnc(&stack, this->GetDrawCoord().x, this->GetDrawCoord().y, scene->GetDrawSize().width, scene->GetDrawSize().height);
				}
				else
				{
					static_cast<KIR5::SubBitmap &>(Res::bitmapBox).drawScaled(this->GetDrawCoord().x, this->GetDrawCoord().y, scene->GetDrawSize().width, scene->GetDrawSize().height);
				}
			}
			template<typename DATA>
			void Func<DATA>::SDraw()//teszteés céljából hogy a conter mûködjön
			{
				DrawnedCount++;
				if (DrawnedCount >= 2)
				{
					clog << "WARNING! An object has been drawned two times (" << coord.x << "," << coord.y << "):" << KIR4::eol;
				}
				SObjectDrawCounts++;
				Draw();
			}
			template<typename DATA>
			void Func<DATA>::setOddDrawCoord()
			{
				Type::Coord draw_coord = {
					Type::Coord::base((coord.x + move.x) * Type::Move::base(scene->GetDrawSize().width)),
					Type::Coord::base((coord.y + move.y) * Type::Move::base(scene->GetDrawSize().height))
				};

				if (draw_coord != DrawCoord)
				{
					DrawCoord = draw_coord;
					requests.draw = true;
				}
			}
			template<typename DATA>
			void Func<DATA>::setRoundDrawCoord()
			{
				Type::Coord draw_coord = {
					coord.x * scene->GetDrawSize().width,
					coord.y * scene->GetDrawSize().height
				};

				if (draw_coord != DrawCoord)
				{
					DrawCoord = draw_coord;
					requests.draw = true;
				}
			}

			template<typename DATA>
			Type::Coord Func<DATA>::GetDrawCoord()
			{
				return {DrawCoord.x - scene->GetDrawOffSet().width, DrawCoord.y - scene->GetDrawOffSet().height};
			}

			//DRAW #####################################################################################

			template<typename DATA>
			void Func<DATA>::RunSDraw()//teszteés céljából hogy a conter mûködjön
			{
				drawCounter++;
				drawAtOnceCounter++;
				if (drawAtOnceCounter >= 2)
				{
					clog << "WARNING! An object has been drawned two times (" << coord.x << "," << coord.y << "):" << KIR4::eol;
					Print();
				}
				drawCounterID = ++totalDrawCounter;
				Stack stack;
				stack.o = this;
				stack.specific = this->specific;
				drawnerFnc(&stack, this->GetDrawCoord().x, this->GetDrawCoord().y, scene->GetDrawSize().width, scene->GetDrawSize().height);
			}
		}
	}
}

#endif
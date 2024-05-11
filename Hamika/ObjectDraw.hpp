#ifndef OBJECT_DRAW_HPP
#define OBJECT_DRAW_HPP

#include "ObjectDraw.h"

namespace Object
{
	namespace Module
	{
		namespace Draw
		{
			template<typename MODULES_T>
			unsigned long long int Data<MODULES_T>::SObjectDrawCounts = 0;

			template<typename MODULES_T>
			unsigned long long int Data<MODULES_T>::totalDrawCounter = 0;


			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				data_.drawnerFnc = nullptr;
				configureDrawOptions();
			}

			template<typename MODULES_T>
			Json Func<MODULES_T>::print()
			{
				Json json;

				json["DrawCoord.x"] = data_.DrawCoord.x();
				json["DrawCoord.y"] = data_.DrawCoord.y();

				return json;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::configureDrawOptions()
			{
				data_.DrawCoord.x() = data_.coord.x() * data_.scene->GetDrawSize().width();
				data_.DrawCoord.y() = data_.coord.y() * data_.scene->GetDrawSize().height();
			}


			template<typename MODULES_T>
			inline void Func<MODULES_T>::Draw()
			{
				if (data_.drawnerFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					data_.drawnerFnc(*object, GetDrawCoord().x(), GetDrawCoord().y(), data_.scene->GetDrawSize().width(), data_.scene->GetDrawSize().height());
				}
				else
				{
					static_cast<KIR5::SubBitmap &>(Res::bitmapBox).drawScaled(GetDrawCoord().x(), GetDrawCoord().y(), data_.scene->GetDrawSize().width(), data_.scene->GetDrawSize().height());
				}
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::SDraw()//teszteés céljából hogy a conter mûködjön
			{
				data_.DrawnedCount++;
				if (data_.DrawnedCount >= 2)
				{
					clog << "WARNING! An object has been drawned two times (" << data_.coord.x() << "," << data_.coord.y() << "):" << KIR4::eol;
				}
				data_.SObjectDrawCounts++;
				Draw();
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::setOddDrawCoord()
			{
				Type::Coord draw_coord = {
					Type::Coord::base((data_.coord.x() + data_.move.x()) * Type::Move::base(data_.scene->GetDrawSize().width())),
					Type::Coord::base((data_.coord.y() + data_.move.y()) * Type::Move::base(data_.scene->GetDrawSize().height()))
				};

				if (draw_coord != data_.DrawCoord)
				{
					data_.DrawCoord = draw_coord;
					data_.requests.draw = true;
				}
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::setRoundDrawCoord()
			{
				Type::Coord draw_coord = {
					data_.coord.x() * data_.scene->GetDrawSize().width(),
					data_.coord.y() * data_.scene->GetDrawSize().height()
				};

				if (draw_coord != data_.DrawCoord)
				{
					data_.DrawCoord = draw_coord;
					data_.requests.draw = true;
				}
			}

			template<typename MODULES_T>
			inline Type::Coord Func<MODULES_T>::GetDrawCoord()
			{
				return {data_.DrawCoord.x() - data_.scene->GetDrawOffSet().width(), data_.DrawCoord.y() - data_.scene->GetDrawOffSet().height()};
			}

			//DRAW #####################################################################################

			template<typename MODULES_T>
			void Func<MODULES_T>::RunSDraw()//teszteés céljából hogy a conter mûködjön
			{
				drawCounter++;
				drawAtOnceCounter++;
				if (drawAtOnceCounter >= 2)
				{
					clog << "WARNING! An object has been drawned two times (" << data_.coord.x() << "," << data_.coord.y() << "):" << KIR4::eol;
					Print();
				}
				drawCounterID = ++totalDrawCounter;
				if (data_.drawnerFnc)
				{
					Stack stack;
					stack.o = this;
					stack.specific = specific;
					data_.drawnerFnc(&stack, GetDrawCoord().x(), GetDrawCoord().y(), data_.scene->GetDrawSize().width(), data_.scene->GetDrawSize().height());
				}
			}
		}
	}
}

#endif
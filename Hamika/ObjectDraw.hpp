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
				data_._draw_type = DrawType::Standard;
				data_.drawnerFnc = nullptr;
				configureDrawOptions();
			}

			template<typename MODULES_T>
			Json Func<MODULES_T>::print() const
			{
				Json json;

				json["_draw_type"] = data_._draw_type;
				json["DrawCoord.x"] = data_.DrawCoord.x();
				json["DrawCoord.y"] = data_.DrawCoord.y();

				return json;
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::configureDrawOptions()
			{
				data_.DrawCoord = data_.coord * data_.scene->getBlockSize();
			}

			template<typename MODULES_T>
			inline void Func<MODULES_T>::Draw(const Type::Coord &_coord, const Type::Coord &_size)
			{
				data_.SObjectDrawCounts++;
				data_.DrawnedCount++;
				if (data_.DrawnedCount >= 2)
				{
					clog << "WARNING! An object has been drawned two times (" << data_.coord.x() << "," << data_.coord.y() << "):" << KIR4::eol;
				}

				if (data_.drawnerFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					data_.drawnerFnc(*object, _coord.x(), _coord.y(), _size.w(), _size.h());
				}
				else
				{
					static_cast<KIR5::SubBitmap &>(Res::bitmapBox).drawScaled(_coord.x(), _coord.y(), _size.w(), _size.h());
				}
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::setOddDrawCoord()
			{
				Type::Coord draw_coord = (data_.coord + data_.move) * data_.scene->getBlockSize();

				if (draw_coord != data_.DrawCoord)
				{
					data_.DrawCoord = draw_coord;
					data_.requests.draw = true;
				}
			}
			template<typename MODULES_T>
			inline void Func<MODULES_T>::setRoundDrawCoord()
			{
				Type::Coord draw_coord = data_.coord * data_.scene->getBlockSize();

				if (draw_coord != data_.DrawCoord)
				{
					data_.DrawCoord = draw_coord;
					data_.requests.draw = true;
				}
			}
		}
	}
}

#endif
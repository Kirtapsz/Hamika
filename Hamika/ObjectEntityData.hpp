#ifndef OBJECT_ENTITY_DATA_HPP
#define OBJECT_ENTITY_DATA_HPP

#include "ObjectEntityData.h"
#include "OriginalEntities.h"
#include "Murphy.h"


namespace Object
{
	namespace Module
	{
		namespace EntityData
		{
			template <typename OBJECT>
			union Data<OBJECT>::EntityData
			{
				Entity::Murphy::EntityData murphy;

				Entity::Zonk_001::EntityData zonk_001;
				Entity::BaseX_002::EntityData basex_002;
				Entity::Bug_013::EntityData bug_013;
				Entity::Infotron_014::EntityData infotron_014;
				Entity::Exit_015::EntityData exit_015;
				Entity::Electrons_026::EntityData electrons_026;
				Entity::SnikSnakMove_027::EntityData snik_snak_move_027;
				Entity::Terminal_028::EntityData terminal_028;
				Entity::Utility1_029::EntityData utility1_029;
				Entity::Utility2_030::EntityData utility2_030;
				Entity::ExplosionEffect_032::EntityData explosion_effect_032;
				Entity::Explosion_033::EntityData explosion_033;
			};

			template <typename MODULES_T>
			Data<MODULES_T>::~Data()
			{

			}

			template<typename MODULES_T>
			inline Func<MODULES_T>::Func(typename MODULES_T::DATA_T &_data, typename MODULES_T::FUNC_T &_func):
				data_(_data), func_(_func)
			{

			}
			template <typename MODULES_T>
			inline Func<MODULES_T>::~Func()
			{

			}
			template <typename MODULES_T>
			inline void Func<MODULES_T>::__init__(Type::ID id, Type::Coord coord)
			{
				memset(&data_.entity_data, 0, sizeof(data_.entity_data));
			}
			template <typename MODULES_T>
			inline Json Func<MODULES_T>::print()
			{
				if (data_.printFnc)
				{
					typename MODULES_T::BRICK_T *object = static_cast<typename MODULES_T::BRICK_T *>(this);
					return data_.printFnc(*object);
				}
				else
				{
					return Json();
				}
			}
		}
	}
}

#endif
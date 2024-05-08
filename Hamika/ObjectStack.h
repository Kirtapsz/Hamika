#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"


#define OBJECT_CREATER_PARAM Object::Brick &_brick
#define OBJECT_CREATER_CALL _brick

#define OBJECT_INITIALIZER_PARAM

#define OBJECT_UPDATE_PARAM Object::Brick &_brick, Object::Brick::UpdateType updateType
#define OBJECT_UPDATE_CALL _brick, updateType

#define OBJECT_FINALIZE_PARAM Object::Brick &_brick
#define OBJECT_FINALIZE_CALL _brick

#define OBJECT_TIMER_PARAM Object::Brick &_brick
#define OBJECT_TIMER_CALL _brick

namespace Object
{
	namespace Module
	{
		namespace Stack
		{
			template <typename OBJECT>
			struct Data
			{
				struct Stack
				{
					OBJECT *o = nullptr;
					unsigned char *specific = nullptr;
					unsigned int pos = 0;

					inline Stack(OBJECT *object):
						o(object), specific(object->specific)
					{

					}

					template <typename SPECIFIC>
					inline operator SPECIFIC *()
					{
						return (SPECIFIC *)(&specific[pos]);
					}

					inline operator Stack *()
					{
						static_assert(false, "Stack specific cannot cast to itself.");
						return nullptr;
					}

					template <typename SPECIFIC>
					struct Handler
					{
						Stack *stack;
						unsigned int pos;

						inline Handler(Stack &_stack):
							stack(&_stack), pos(_stack.pos)
						{
							stack->pos += sizeof(SPECIFIC);
						}
						inline Handler(Stack *_stack) :
							stack(_stack), pos(_stack->pos)
						{
							stack->pos += sizeof(SPECIFIC);
						}

						inline operator SPECIFIC *()
						{
							return (SPECIFIC *)(&stack->specific[pos]);
						}
					};
				};

				//OBJECT SPECIFIC
				union EntityData;
				EntityData entity_data;

				unsigned char *specific{(unsigned char *)&entity_data};

#define OBJECT_PRINTER_PARAM Object::Brick &_brick
#define OBJECT_PRINTER_CALL _brick
#define OBJECT_PRINTER_RET Json
				typedef Json(*PRINT)(OBJECT &_object);
				PRINT printFnc = nullptr;

				virtual ~Data();
			};
			template <typename DATA>
			struct Func: virtual DATA
			{
				virtual ~Func();
				void __init__(Type::ID id, Type::Coord coord);
				Json print();
			};
		}
	}
}

#include "ObjectStack.hpp"
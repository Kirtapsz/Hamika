#pragma once

#include <fstream>

#include "Types.h"
#include "IDreg.h"
#include "Tools.h"

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
				unsigned char specific[256] = {0};

#define OBJECT_PRINTER_PARAM Object::Brick::Stack *stack
#define OBJECT_PRINTER_CALL stack
#define OBJECT_PRINTER_RET Json
				typedef Json(*PRINT)(Stack *);
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
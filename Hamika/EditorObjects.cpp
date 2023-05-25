#include "Objects.h"
#include "EditorObjects.h"
#include "Space.h"
#include "Bedrock.h"
#include "BaseFunctions.h"
#include "Tools.h"
#include "Font.h"

#include <KIR/KIR4_console.h>

namespace Object::Editor
{
	namespace Object
	{
		const char *name = "EditorObject";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{

		}

		void Drawner(EditorObjectBase::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h)
		{
			DrawObject(stack->o->id, x, y, w, h);
		}
	}
	namespace Remain
	{
		KIR5::Bitmap arrowDown;
		KIR5::Bitmap arrowUp;
		KIR5::Bitmap arrowLeft;
		KIR5::Bitmap arrowRight;
		KIR5::Bitmap gravity;
		KIR5::Bitmap detonate;
		KIR5::Bitmap spawn;

		const char *name = "EditorRemain";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{
			arrowDown = Res::uielements[Res::UIElements::RotationDown];
			arrowUp = Res::uielements[Res::UIElements::RotationUp];
			arrowLeft = Res::uielements[Res::UIElements::RotationLeft];
			arrowRight = Res::uielements[Res::UIElements::RotationRight];
			gravity = Res::uielements[Res::UIElements::Grav];
			detonate = Res::uielements[Res::UIElements::Detonate];
			spawn = Res::uielements[Res::UIElements::SpawnPoint];
		}

		void Drawner(EditorObjectBase::Stack *stack, Type::Coord::Type x, Type::Coord::Type y, Type::Coord::Type w, Type::Coord::Type h)
		{
			EditorObjectBase *obj = stack->o->scene->GetObject(stack->o->GetCoord());

			if (obj->scene->GetBlockFlags(stack->o->GetCoord()) & GridFlags::SpawnPoint)
			{
				spawn.drawScaled(x, y, w, h);
			}
			if (obj->scene->GetBlockFlags(stack->o->GetCoord()) & GridFlags::Detonate)
			{
				detonate.drawScaled(x, y, w, h);
			}
			if (obj->scene->GetBlockFlags(stack->o->GetCoord()) & GridFlags::Gravity)
			{
				gravity.drawScaled(x, y, w, h);
			}

			if (obj->GetRotation() == Type::Rotations::Up)
			{
				arrowUp.drawScaled(x, y, w, h);
			}
			else if (obj->GetRotation() == Type::Rotations::Right)
			{
				arrowRight.drawScaled(x, y, w, h);
			}
			else if (obj->GetRotation() == Type::Rotations::Down)
			{
				arrowDown.drawScaled(x, y, w, h);
			}
			else if (obj->GetRotation() == Type::Rotations::Left)
			{
				arrowLeft.drawScaled(x, y, w, h);
			}

			int selectStatus = obj->scene->selectStatus(stack->o->GetCoord());
			if (selectStatus == 1)
			{
				if (obj->scene->isTarget(stack->o->GetCoord()))
				{
					al_draw_filled_rectangle(x, y, x + w, y + h, KIR5::Color(127, 0, 127, 127).getAlphaColored());
				}
				else
				{
					al_draw_filled_rectangle(x, y, x + w, y + h, KIR5::Color(255, 0, 0, 127).getAlphaColored());
				}
			}
			else
			{
				if (selectStatus & 0x02)
				{
					al_draw_filled_rectangle(x, y, x + w, y + h, KIR5::Color(0, 0, 0, 127).getAlphaColored());
				}
				else if (selectStatus & 0x04)
				{
					al_draw_filled_rectangle(x, y, x + w, y + h, KIR5::Color(255, 255, 255, 127).getAlphaColored());
				}
				else if (obj->scene->isTarget(stack->o->GetCoord()))
				{
					al_draw_filled_rectangle(x, y, x + w, y + h, KIR5::Color(0, 0, 255, 127).getAlphaColored());
				}
				else
				{

				}
			}

			char buffer[32];

			sprintf_s(buffer, "(%03d;%03d)", stack->o->GetCoord().x, stack->o->GetCoord().y);

			KIR5::Font font = Res::Consolas[stack->o->DrawSize().height / 6];

			font.draw(x + 1, y + 1, buffer, al_map_rgb(0, 0, 0), KIR5::LEFT);
			font.draw(x, y, buffer, al_map_rgb(255, 255, 255), KIR5::LEFT);

			sprintf_s(buffer, "(%03d)", obj->id);
			font.draw(x + 1, y + 1 + (stack->o->DrawSize().height / 6), buffer, al_map_rgb(0, 0, 0), KIR5::LEFT);
			font.draw(x, y + (stack->o->DrawSize().height / 6), buffer, al_map_rgb(255, 255, 255), KIR5::LEFT);
		}
	}
}

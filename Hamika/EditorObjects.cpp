#include "Objects.h"
#include "EditorObjects.h"
#include "Space.h"
#include "Bedrock.h"
#include "BaseFunctions.h"
#include "Tools.h"
#include "Font.h"

#include <KIR/KIR4_console.h>

namespace Editor::Object
{
	namespace Back
	{
		const char *name = "EditorBack";

		void Initializer(OBJECT_INITIALIZER_PARAM)
		{

		}

		void Drawner(Brick &_brick, Type::Coord::base x, Type::Coord::base y, Type::Coord::base w, Type::Coord::base h)
		{
			DrawObject(_brick.id, x, y, w, h);
		}
	}
	namespace Front
	{
		KIR5::Bitmap arrowDown;
		KIR5::Bitmap arrowUp;
		KIR5::Bitmap arrowLeft;
		KIR5::Bitmap arrowRight;
		KIR5::Bitmap gravity;
		KIR5::Bitmap detonate;
		KIR5::Bitmap spawn;

		const char *name = "EditorFront";

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

		void Drawner(Brick &_brick, Type::Coord::base x, Type::Coord::base y, Type::Coord::base w, Type::Coord::base h)
		{
			if (_brick.scene->GetBlockFlags(_brick.GetCoord()) & GridFlags::SpawnPoint)
			{
				spawn.drawScaled(x, y, w, h);
			}
			if (_brick.scene->GetBlockFlags(_brick.GetCoord()) & GridFlags::Detonate)
			{
				detonate.drawScaled(x, y, w, h);
			}
			if (_brick.scene->GetBlockFlags(_brick.GetCoord()) & GridFlags::Gravity)
			{
				gravity.drawScaled(x, y, w, h);
			}

			if (_brick.GetRotation() == Type::Rotations::Up)
			{
				arrowUp.drawScaled(x, y, w, h);
			}
			else if (_brick.GetRotation() == Type::Rotations::Right)
			{
				arrowRight.drawScaled(x, y, w, h);
			}
			else if (_brick.GetRotation() == Type::Rotations::Down)
			{
				arrowDown.drawScaled(x, y, w, h);
			}
			else if (_brick.GetRotation() == Type::Rotations::Left)
			{
				arrowLeft.drawScaled(x, y, w, h);
			}

			int selectStatus = _brick.scene->selectStatus(_brick.GetCoord());
			if (selectStatus == 1)
			{
				if (_brick.scene->isTarget(_brick.GetCoord()))
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
				else if (_brick.scene->isTarget(_brick.GetCoord()))
				{
					al_draw_filled_rectangle(x, y, x + w, y + h, KIR5::Color(0, 0, 255, 127).getAlphaColored());
				}
				else
				{

				}
			}

			char buffer[32];

			sprintf_s(buffer, "(%03d;%03d)", _brick.GetCoord().x(), _brick.GetCoord().y());

			KIR5::Font font = Res::Consolas[_brick.scene->GetDrawSize().height() / 6];

			font.draw(x + 1, y + 1, buffer, al_map_rgb(0, 0, 0), KIR5::LEFT);
			font.draw(x, y, buffer, al_map_rgb(255, 255, 255), KIR5::LEFT);

			sprintf_s(buffer, "(%03d)", _brick.id);
			font.draw(x + 1, y + 1 + (_brick.scene->GetDrawSize().height() / 6), buffer, al_map_rgb(0, 0, 0), KIR5::LEFT);
			font.draw(x, y + (_brick.scene->GetDrawSize().height() / 6), buffer, al_map_rgb(255, 255, 255), KIR5::LEFT);
		}
	}
}

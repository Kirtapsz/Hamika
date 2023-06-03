#include "EditorMiniMap.h"
#include "EditorMainEvent.h"

namespace Editor::UI
{
	MiniMap::MiniMap()
	{
		fncLock.push_back([&](FNC_LOCK_PARAMS)->FNC_LOCK_RET
		{
			if (map && map->Exists())
			{
				mouseHold = true;
				setCameraToClick(x_, y_);
			}
		});

		fncMouseAxes.push_back([&](FNC_MOUSE_AXES_PARAMS)->FNC_MOUSE_AXES_RET
		{
			if (map && map->Exists())
			{
				if (mouseHold)
				{
					setCameraToClick(x_, y_);
				}
			}
		});

		fncUnlock.push_back([&](FNC_UNLOCK_PARAMS)->FNC_UNLOCK_RET
		{
			if (mouseHold)
			{
				mouseHold = false;
			}
		});

		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			float w = width() / float(bitmap.width());
			float h = height() / float(bitmap.height());
			float min = (std::min)(w, h);

			w = bitmap.width() * min;
			h = bitmap.height() * min;

			float cx = x_ + (width() - w) / 2.f;
			float cy = y_ + (height() - h) / 2.f;

			bitmap.drawScaled(cx, cy, w, h);

			al_draw_rectangle(
				cx + (int)(camera.x * min - cameraSize.x / 2) + 1,
				cy + (int)(camera.y * min - cameraSize.y / 2) + 1,
				cx + (int)(camera.x * min + cameraSize.x / 2),
				cy + (int)(camera.y * min + cameraSize.y / 2),
				KIR5::Color(255, 255, 255), 1);
		}));
	}
	MiniMap::~MiniMap()
	{

	}

	void MiniMap::setCameraToClick(int x_, int y_)
	{
		float w = width() / float(bitmap.width());
		float h = height() / float(bitmap.height());
		float min = (std::min)(w, h);

		w = bitmap.width() * min;
		h = bitmap.height() * min;

		float cx = x() + (width() - w) / 2.f;
		float cy = y() + (height() - h) / 2.f;

		float mx = x_ - cx;
		float my = y_ - cy;

		mx /= min;
		my /= min;

		mx += blockDimension / 2.f;
		my += blockDimension / 2.f;

		mx /= blockDimension;
		my /= blockDimension;

		MainEvent::s_object->scene->setTarget({mx,my});
	}

	void MiniMap::SetMap(std::shared_ptr<Matrix<SceneBlock<Object::Brick>>> &map_)
	{
		map = map_;
		bitmap = al_create_bitmap(((Type::Size)*map).width * blockDimension, ((Type::Size)*map).height * blockDimension);
		updateBlocks();
	}
	void MiniMap::updateBlocks()
	{
		KIR5::BitmapTarget bt(bitmap);
		al_clear_to_color(KIR5::Color(0, 0, 0));
		map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
		{
			DrawObject((*map)[coord].object->id, coord.x * blockDimension, coord.y * blockDimension, blockDimension, blockDimension);
			if (block.selected)
			{
				al_draw_filled_rectangle(coord.x * blockDimension, coord.y * blockDimension, coord.x * blockDimension + blockDimension, coord.y * blockDimension + blockDimension, KIR5::Color(255, 0, 0, 127).getAlphaColored());
			}
		});
	}
	void MiniMap::updatePosition(Type::Move center, Type::Move size)
	{
		camera = center;
		cameraSize = size;

		camera.x *= blockDimension;
		camera.y *= blockDimension;
		camera.x += blockDimension / 2.f;
		camera.y += blockDimension / 2.f;
		cameraSize.x *= blockDimension;
		cameraSize.y *= blockDimension;
	}
}
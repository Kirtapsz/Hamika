#ifndef MAP_DRAWNER_HPP__
#define MAP_DRAWNER_HPP__

// https://www.allegro.cc/manual/5/al_hold_bitmap_drawing

#include "SceneDrawer.h"
#include "Font.h"
#include "Tools.h"

#include <KIR\AL\KIR5_color.h>
#include <KIR\AL\KIR5_blender.h>
#include <KIR\AL\KIR5_bitmap_target.h>
#include <KIR\KIR4_console.h>

template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::setGlobalGravity(bool *globalGravity)
{
	this->globalGravity = globalGravity;
}

template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::RedrawnRow(Type::Coord::base row, Type::Coord::base begin, Type::Coord::base end)
{
	for (; begin < end; begin++)
		Redrawn({begin,row});
}
template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::RedrawnCol(Type::Coord::base col, Type::Coord::base begin, Type::Coord::base end)
{
	for (; begin < end; begin++)
		Redrawn({col,begin});
}

template <typename ACTIVE_BLOCK_T>
Type::Coord SceneDrawer<ACTIVE_BLOCK_T>::GetDrawBeginSource() const
{
	return DrawBeginSource;
}
template <typename ACTIVE_BLOCK_T>
Type::Coord SceneDrawer<ACTIVE_BLOCK_T>::GetDrawBegin() const
{
	return DrawBegin;
}
template <typename ACTIVE_BLOCK_T>
Type::Coord SceneDrawer<ACTIVE_BLOCK_T>::GetDrawEnd() const
{
	return DrawEnd;
}
template <typename ACTIVE_BLOCK_T>
Type::Size SceneDrawer<ACTIVE_BLOCK_T>::GetBitmapDrawOffset() const
{
	return {-BlocksBitmapBufferSize * DrawSize.width - BlocksBitmapDrawOffset.width, -BlocksBitmapBufferSize * DrawSize.height - BlocksBitmapDrawOffset.height};
}
template <typename ACTIVE_BLOCK_T>
Type::Size SceneDrawer<ACTIVE_BLOCK_T>::GetBitmapSize() const
{
	return {BlocksBitmapSize.width * DrawSize.width, BlocksBitmapSize.height * DrawSize.height};
}

template <typename ACTIVE_BLOCK_T>
Type::Size SceneDrawer<ACTIVE_BLOCK_T>::GetDrawSize() const
{
	return DrawSize;
}
template <typename ACTIVE_BLOCK_T>
Type::Size SceneDrawer<ACTIVE_BLOCK_T>::GetDrawOffSet() const
{
	return DrawOffset;
}

template <typename ACTIVE_BLOCK_T>
Type::Move SceneDrawer<ACTIVE_BLOCK_T>::GetCameraSize() const
{
	return CameraSize;
}

template <typename ACTIVE_BLOCK_T>
Type::Move SceneDrawer<ACTIVE_BLOCK_T>::GetCamera() const
{
	return CameraLast;
}

template <typename ACTIVE_BLOCK_T>
Type::Coord SceneDrawer<ACTIVE_BLOCK_T>::GetFromCursor(int x, int y)
{
	float x_ = (CameraHcenter ? (x) : (x + BlocksBitmapBufferSize * DrawSize.width + BlocksBitmapDrawOffset.width)) / (float)DrawSize.width;
	float y_ = (CameraVcenter ? (y) : (y + BlocksBitmapBufferSize * DrawSize.height + BlocksBitmapDrawOffset.height)) / (float)DrawSize.height;

	return {
		limiter<Type::Coord::base>(0,((Type::Size)reach(map)).width - 1,CameraBegin.x + x_),
		limiter<Type::Coord::base>(0,((Type::Size)reach(map)).height - 1,CameraBegin.y + y_)
	};
}

//kamera k�z�pen van
template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::MoveCameraTo(Type::Move camera)
{
	if (CameraLast == camera)
		return;
	if (CameraHcenter)
	{
		if (camera.x < CameraX1)
			camera.x = CameraX1;
		if (camera.x > CameraX2)
			camera.x = CameraX2;

		if (CameraLast.x != camera.x)
		{
			CameraLast.x = camera.x;

			CameraBegin.x = camera.x - CameraCounts.x + 0.5f;

			if (BlocksBitmapHcenter)
			{
				CameraEnd.x = CameraBegin.x + CameraSize.x + 1;

				if ((int)CameraBegin.x<DrawBegin.x || (int)CameraEnd.x>DrawEnd.x)
				{
					DrawBeginSource.x = (int)(BlocksBitmapBufferSize + camera.x - BlocksBitmapCounts.width + 0.5f) - BlocksBitmapBufferSize;//BlocksBitmapBufferSize az�rt kel hozz�adni hogy ne mennyek �t negatv�ba, mert akkor m�sk�pp v�gja le a tizedees jegyet

					DrawBegin.x = DrawBeginSource.x;
					DrawEnd.x = DrawBegin.x + BlocksBitmapSize.width;

					DrawOffset.width = DrawBegin.x * DrawSize.width;

					if (DrawBegin.x < 0)
						DrawBegin.x = 0;
					if (DrawEnd.x > ((Type::Size)reach(map)).width)
						DrawEnd.x = ((Type::Size)reach(map)).width;
				}
			}

			BlocksBitmapDrawOffset.width = (CameraBegin.x - (DrawBeginSource.x + BlocksBitmapBufferSize)) * DrawSize.width;
		}
	}

	if (CameraVcenter)
	{
		if (camera.y < CameraY1)
			camera.y = CameraY1;
		if (camera.y > CameraY2)
			camera.y = CameraY2;

		if (CameraLast.y != camera.y)
		{
			CameraLast.y = camera.y;

			CameraBegin.y = camera.y - CameraCounts.y + 0.5f;

			if (BlocksBitmapVcenter)
			{
				CameraEnd.y = CameraBegin.y + CameraSize.y + 1;

				if ((int)CameraBegin.y<DrawBegin.y || (int)CameraEnd.y>DrawEnd.y)
				{
					DrawBeginSource.y = (int)(BlocksBitmapBufferSize + camera.y - BlocksBitmapCounts.height + 0.5f) - BlocksBitmapBufferSize;//BlocksBitmapBufferSize az�rt kel hozz�adni hogy ne mennyek �t negatv�ba, mert akkor m�sk�pp v�gja le a tizedees jegyet

					DrawBegin.y = DrawBeginSource.y;
					DrawEnd.y = DrawBegin.y + BlocksBitmapSize.height;

					DrawOffset.height = DrawBegin.y * DrawSize.height;

					if (DrawBegin.y < 0)
						DrawBegin.y = 0;
					if (DrawEnd.y > ((Type::Size)reach(map)).height)
						DrawEnd.y = ((Type::Size)reach(map)).height;
				}
			}

			BlocksBitmapDrawOffset.height = (CameraBegin.y - (DrawBeginSource.y + BlocksBitmapBufferSize)) * DrawSize.height;
		}
	}
}
template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::InitializeDrawOptions(Type::Size cameraPhySize, Type::CameraSize cameraSizeAdjust)
{
	if (cameraSizeAdjust.width > 0.f && cameraSizeAdjust.height > 0.f)
	{
		Type::CameraSize sizeDim = {cameraPhySize.width / cameraSizeAdjust.width, cameraPhySize.height / cameraSizeAdjust.height};
		Type::CameraSize::base sizeAdjust = (std::max)(sizeDim.width, sizeDim.height);
		DrawSize = {(Type::Size::base)sizeAdjust,(Type::Size::base)sizeAdjust};
	}
	else
	{
		DrawSize = {blockSizeInPixel, blockSizeInPixel};
	}

	TotalPointDrawCount = 0;
	TotalObjectDrawCount = 0;

	BlocksBitmapDrawOffset = {0,0};
	CameraBegin = {0,0};

	CameraSize.x = cameraPhySize.width / (float)DrawSize.width;
	CameraSize.y = cameraPhySize.height / (float)DrawSize.height;

	CameraCounts.x = CameraSize.x / 2.f;
	CameraCounts.y = CameraSize.y / 2.f;

	BlocksBitmapSize.width = (cameraPhySize.width + DrawSize.width - 1) / DrawSize.width + BlocksBitmapBufferSize * 2;
	if (BlocksBitmapSize.width % 2 == 0)
		BlocksBitmapSize.width++;
	BlocksBitmapSize.height = (cameraPhySize.height + DrawSize.height - 1) / DrawSize.height + BlocksBitmapBufferSize * 2;
	if (BlocksBitmapSize.height % 2 == 0)
		BlocksBitmapSize.height++;

	CameraX1 = (cameraPhySize.width / (float)DrawSize.width) / 2.f - 0.5f;
	CameraY1 = (cameraPhySize.height / (float)DrawSize.height) / 2.f - 0.5f;
	CameraX2 = ((Type::Size)reach(map)).width - CameraX1 - 1;
	CameraY2 = ((Type::Size)reach(map)).height - CameraY1 - 1;

	DrawBegin = Type::Coord::Invalid;
	DrawEnd = Type::Coord::Invalid;
	CameraLast = Type::Move::Invalid;

	if (((Type::Size)reach(map)).width <= BlocksBitmapSize.width)
	{
		BlocksBitmapSize.width = ((Type::Size)reach(map)).width;
		DrawBegin.x = 0;
		DrawEnd.x = ((Type::Size)reach(map)).width;
		DrawOffset.width = 0;
		DrawBeginSource.x = 0;
		BlocksBitmapHcenter = false;
		if (((Type::Size)reach(map)).width <= CameraSize.x)
		{
			BlocksBitmapDrawOffset.width = -(cameraPhySize.width - ((Type::Size)reach(map)).width * DrawSize.width) / 2 - BlocksBitmapBufferSize * DrawSize.width;
			CameraHcenter = false;
		}
		else
			CameraHcenter = true;
	}
	else
	{
		BlocksBitmapHcenter = true;
		CameraHcenter = true;
	}

	if (((Type::Size)reach(map)).height <= BlocksBitmapSize.height)
	{
		BlocksBitmapSize.height = ((Type::Size)reach(map)).height;
		DrawBegin.y = 0;
		DrawEnd.y = ((Type::Size)reach(map)).height;
		DrawOffset.height = 0;
		DrawBeginSource.y = 0;
		BlocksBitmapVcenter = false;
		if (((Type::Size)reach(map)).height <= CameraSize.y)
		{
			BlocksBitmapDrawOffset.height = -(cameraPhySize.height - ((Type::Size)reach(map)).height * DrawSize.height) / 2 - BlocksBitmapBufferSize * DrawSize.height;
			CameraVcenter = false;
		}
		else
			CameraVcenter = true;
	}
	else
	{
		BlocksBitmapVcenter = true;
		CameraVcenter = true;
	}

	LastDrawOffset = DrawOffset;

	BlocksBitmapCounts.width = (BlocksBitmapSize.width - 1) / 2;
	BlocksBitmapCounts.height = (BlocksBitmapSize.height - 1) / 2;

	BlocksBitmap = al_create_bitmap(BlocksBitmapSize.width * DrawSize.width, BlocksBitmapSize.height * DrawSize.height);
	LayerBitmap = al_create_bitmap(BlocksBitmapSize.width * DrawSize.width, BlocksBitmapSize.height * DrawSize.height);
	RedrawnedBitmap = al_create_bitmap(BlocksBitmapSize.width * DrawSize.width, BlocksBitmapSize.height * DrawSize.height);
	TempBitmap = al_create_bitmap(BlocksBitmapSize.width * DrawSize.width, BlocksBitmapSize.height * DrawSize.height);

	DrawBeginLast = {0,0};
	DrawEndLast = {0,0};

	map->foreach([&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		block.object->configureDrawOptions();
		block.remain->configureDrawOptions();
		block.Redrawn = true;
	});

	//drawStatistics.clear();
}
template <typename ACTIVE_BLOCK_T>
SceneDrawer<ACTIVE_BLOCK_T>::SceneDrawer()
{
	gravitySlides.initialize(Res::tiles[Res::Tiles::_Gravitation], Res::tiles[Res::BitmapPool::Fallback]);
	gravityAnimator.Initialize();
	gravityAnimator.SetNumberOfFrames(gravitySlides.getCount());
	gravityAnimator.SetAnimationTime(1.0f);
}

template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::DrawBlocks(int x, int y)
{
	Object::Brick::SObjectDrawCounts = 0;
	//drawStatistics.start();

	int cx, cy, cw, ch;
	int x1, x2, y1, y2;
	al_get_clipping_rectangle(&cx, &cy, &cw, &ch);

	{
		al_reset_clipping_rectangle();

		KIR5::BitmapTarget target;

		int offsetw = LastDrawOffset.width - DrawOffset.width;
		int offseth = LastDrawOffset.height - DrawOffset.height;

		if (offsetw != 0 || offseth != 0)
		{
			target.lock(TempBitmap);
			al_clear_to_color(KIR5::Color::_transparent);
			LayerBitmap.draw(0, 0);
			target.lock(LayerBitmap);
			al_clear_to_color(KIR5::Color::_transparent);
			TempBitmap.draw(offsetw, offseth);

			target.lock(TempBitmap);
			al_clear_to_color(KIR5::Color::_transparent);
			RedrawnedBitmap.draw(0, 0);
			target.lock(RedrawnedBitmap);
			al_clear_to_color(KIR5::Color::_transparent);
			TempBitmap.draw(offsetw, offseth);

			target.lock(TempBitmap);
			al_clear_to_color(KIR5::Color::_transparent);
			BlocksBitmap.draw(0, 0);
			target.lock(BlocksBitmap);
			al_clear_to_color(KIR5::Color::_transparent);
			TempBitmap.draw(offsetw, offseth);

			if (DrawBegin.x < DrawBeginLast.x)
				for (DrawBeginLast.x--; DrawBeginLast.x >= DrawBegin.x; DrawBeginLast.x--)
					RedrawnCol(DrawBeginLast.x, DrawBegin.y, DrawEnd.y);
			else if (DrawEnd.x > DrawEndLast.x)
				for (; DrawEndLast.x < DrawEnd.x; DrawEndLast.x++)
					RedrawnCol(DrawEndLast.x, DrawBegin.y, DrawEnd.y);

			if (DrawBegin.y < DrawBeginLast.y)
				for (DrawBeginLast.y--; DrawBeginLast.y >= DrawBegin.y; DrawBeginLast.y--)
					RedrawnRow(DrawBeginLast.y, DrawBegin.x, DrawEnd.x);
			else if (DrawEnd.y > DrawEndLast.y)
				for (; DrawEndLast.y < DrawEnd.y; DrawEndLast.y++)
					RedrawnRow(DrawEndLast.y, DrawBegin.x, DrawEnd.x);

			LastDrawOffset = DrawOffset;
			DrawBeginLast = DrawBegin;
			DrawEndLast = DrawEnd;
		}
		else
		{
			target.lock(BlocksBitmap);
		}

		map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
		{
			if (block.object->requests.draw || (block.remain->isExists && block.remain->requests.draw))
			{
				block.object->requests.draw = false;
				block.remain->requests.draw = false;
				Redrawn(coord);
			}
		});

		map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
		{
			if (block.Redrawn)
				block.DrawType = ACTIVE_BLOCK_T::DrawType::LastDrawned;
			else if (block.DrawType & ACTIVE_BLOCK_T::DrawType::LastDrawned)
				block.DrawType -= ACTIVE_BLOCK_T::DrawType::LastDrawned;
		});

		if (blockRefreshActive)
		{
			target.lock(RedrawnedBitmap);
			al_reset_clipping_rectangle();

			{
				KIR5::Blender blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);
				al_draw_filled_rectangle(0, 0, RedrawnedBitmap.width(), RedrawnedBitmap.height(), KIR5::Color(10, 10, 10, 10));
			}
			int PurpleDraw = 0;
			map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
			{
				if (block.Redrawn)
				{
					//if (
					//	coord.x*DrawSize.width - DrawOffset.width >= bitmap.width() + MapBitmapBufferSize + DrawSize.width
					//	||
					//	coord.y*DrawSize.height - DrawOffset.height >= bitmap.height() + MapBitmapBufferSize + DrawSize.height
					//	||
					//	coord.x*DrawSize.width - DrawOffset.width + DrawSize.width <-DrawSize.width
					//	||
					//	coord.y*DrawSize.height - DrawOffset.height + DrawSize.height <-+DrawSize.height
					//	)
					//	clog <<KIR4::LRED <<"NON VISIBLE BLOCK DRAWNED! " <<coord <<KIR4::eol;

					PurpleDraw++;
					al_draw_filled_rectangle(coord.x * DrawSize.width - DrawOffset.width, coord.y * DrawSize.height - DrawOffset.height, coord.x * DrawSize.width - DrawOffset.width + DrawSize.width, coord.y * DrawSize.height - DrawOffset.height + DrawSize.height, KIR5::Color(255, 0, 255, 20));

				}
			});
		}

		target.lock(BlocksBitmap);

		map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
		{
			if (block.Redrawn)
			{
				block.DrawNumber = TotalPointDrawCount++;
				block.DrawType |= ACTIVE_BLOCK_T::DrawType::Cleared;
				al_set_clipping_rectangle(coord.x * DrawSize.width - DrawOffset.width, coord.y * DrawSize.height - DrawOffset.height, DrawSize.width, DrawSize.height);
				al_clear_to_color(KIR5::Color::_transparent);
				//al_draw_filled_rectangle(coord.x*DrawSize.width, coord.y*DrawSize.height, (coord.x + 1)*DrawSize.width, (coord.y + 1)*DrawSize.height, KIR5::Color::invisible());
			}
		});

		al_reset_clipping_rectangle();

		//al_hold_bitmap_drawing(true);
		map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
		{
			if (block.Redrawn
				&&
				((/*block.object->isExists && */block.ComeFrom == coord && !block.object->events.topDraw)
				 ||
				 (block.remain->isExists && !block.remain->events.topDraw)))
			{
				x1 = coord.x * DrawSize.width;
				y1 = coord.y * DrawSize.height;
				x2 = x1 + DrawSize.width;
				y2 = y1 + DrawSize.height;

				if (block.ComeFrom != coord && block.object->isExists)
				{
					if (block.object->IsMoveRight())
						x1 += DrawSize.width * (block.object->GetMove().x + 1);
					else if (block.object->IsMoveLeft())
						x2 += DrawSize.width * (block.object->GetMove().x - 1);

					if (block.object->IsMoveDown())
						y1 += DrawSize.height * (block.object->GetMove().y + 1);
					else if (block.object->IsMoveUp())
						y2 += DrawSize.height * (block.object->GetMove().y - 1);
				}

				if (block.GoTo != coord && reach(map)[block.GoTo].object->isExists)
				{
					if (reach(map)[block.GoTo].object->IsMoveRight())
						x2 += DrawSize.width * reach(map)[block.GoTo].object->GetMove().x;
					else if (reach(map)[block.GoTo].object->IsMoveLeft())
						x1 += DrawSize.width * reach(map)[block.GoTo].object->GetMove().x;

					if (reach(map)[block.GoTo].object->IsMoveDown())
						y2 += DrawSize.height * reach(map)[block.GoTo].object->GetMove().y;
					else if (reach(map)[block.GoTo].object->IsMoveUp())
						y1 += DrawSize.height * reach(map)[block.GoTo].object->GetMove().y;
				}

				if (x2 - x1 > 0 && y2 - y1 > 0)
				{
					al_set_clipping_rectangle(x1 - DrawOffset.width, y1 - DrawOffset.height, x2 - x1, y2 - y1);

					if (block.ComeFrom == coord && !block.object->events.topDraw)
					{
						block.object->SDraw();
						block.object->DrawNumber = TotalObjectDrawCount++;
						block.DrawType |= ACTIVE_BLOCK_T::DrawType::IsNotMovingObjectDrawned;
					}
					if (block.remain->isExists && !block.remain->events.topDraw)
					{
						block.remain->SDraw();
						block.remain->DrawNumber = TotalObjectDrawCount++;
						block.DrawType |= ACTIVE_BLOCK_T::DrawType::IsNotMovingRemainDrawned;
					}
				}
			}
		});
		//al_hold_bitmap_drawing(false);

		al_reset_clipping_rectangle();

		{
			//KIR5::Blender blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
			map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
			{
				if (block.Redrawn && (block.ComeFrom != coord || block.object->events.topDraw))
				{
					//if (block.object->isExists && block.object->IsMoving())
					{
						block.object->SDraw();
						block.object->DrawNumber = TotalObjectDrawCount++;
						block.DrawType |= ACTIVE_BLOCK_T::DrawType::IsMovingObjectDrawned;
					}
				}
			});
			map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
			{
				if (block.Redrawn && block.remain->isExists && block.remain->events.topDraw)
				{
					block.remain->SDraw();
					block.remain->DrawNumber = TotalObjectDrawCount++;
					block.DrawType |= ACTIVE_BLOCK_T::DrawType::IsMovingObjectDrawned;
				}
			});
		}

		map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
		{
			if (block.Redrawn)
			{
				block.Redrawn = false;
			}
		});

		map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
		{
			{
				block.object->DrawnedCount = 0;
				if (block.remain->isExists)
					block.remain->DrawnedCount = 0;
			}
		});


		//al_reset_clipping_rectangle();

		if (layerActive)
		{
			gravityAnimator.UpdateTimer();
			if (gravityAnimator.UpdateDrawNumber())
			{
				target.lock(LayerBitmap);

				al_clear_to_color(KIR5::Color::_transparent);
				al_hold_bitmap_drawing(true);
				if (*globalGravity)
				{
					map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
					{
						gravitySlides[gravityAnimator.GetDrawNumber()].drawScaled(coord.x * DrawSize.width - DrawOffset.width, coord.y * DrawSize.height - DrawOffset.height, DrawSize.width, DrawSize.height);
					});
				}
				else
				{
					map->forrange(DrawBegin, DrawEnd, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
					{
						if (block.grid & GridFlags::Gravity)
							gravitySlides[gravityAnimator.GetDrawNumber()].drawScaled(coord.x * DrawSize.width - DrawOffset.width, coord.y * DrawSize.height - DrawOffset.height, DrawSize.width, DrawSize.height);
					});
				}
				al_hold_bitmap_drawing(false);
			}
		}
		target.unlock();
	}

	al_set_clipping_rectangle(cx, cy, cw, ch);
	//al_reset_clipping_rectangle();
	BlocksBitmap.draw(x - BlocksBitmapBufferSize * DrawSize.width - BlocksBitmapDrawOffset.width, y - BlocksBitmapBufferSize * DrawSize.height - BlocksBitmapDrawOffset.height);
	if (layerActive)
	{
		LayerBitmap.draw(x - BlocksBitmapBufferSize * DrawSize.width - BlocksBitmapDrawOffset.width, y - BlocksBitmapBufferSize * DrawSize.height - BlocksBitmapDrawOffset.height);
	}
	if (blockRefreshActive)
	{
		RedrawnedBitmap.draw(x - BlocksBitmapBufferSize * DrawSize.width - BlocksBitmapDrawOffset.width, y - BlocksBitmapBufferSize * DrawSize.height - BlocksBitmapDrawOffset.height);
	}

	al_draw_rectangle(
		x - BlocksBitmapBufferSize * DrawSize.width - BlocksBitmapDrawOffset.width,
		y - BlocksBitmapBufferSize * DrawSize.height - BlocksBitmapDrawOffset.height,
		x - BlocksBitmapBufferSize * DrawSize.width - BlocksBitmapDrawOffset.width + BlocksBitmap.width() + 1,
		y - BlocksBitmapBufferSize * DrawSize.height - BlocksBitmapDrawOffset.height + BlocksBitmap.height() + 1,
		KIR5::Color(255, 0, 255), 1);

	//al_set_clipping_rectangle(cx, cy, cw, ch);
}

template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::SetMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &map_)
{
	map = map_;
}

template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::Redrawn(Type::Coord coord)
{
	if (!reach(map)[coord].Redrawn)
	{
		reach(map)[coord].Redrawn = true;
		if (reach(map)[coord].ComeFrom != coord)
			Redrawn(reach(map)[coord].ComeFrom);
		else if (/*reach(map)[coord].object->isExists &&*/ reach(map)[coord].object->isActionMove())
		{
			if (reach(map)[coord].object->IsMoveLeft())
				Redrawn({coord.x + 1,coord.y});
			else if (reach(map)[coord].object->IsMoveRight())
				Redrawn({coord.x - 1,coord.y});
			else if (reach(map)[coord].object->IsMoveUp())
				Redrawn({coord.x,coord.y + 1});
			else if (reach(map)[coord].object->IsMoveDown())
				Redrawn({coord.x,coord.y - 1});
		}
		if (reach(map)[coord].GoTo != coord)
			Redrawn(reach(map)[coord].GoTo);
	}
}


template <typename ACTIVE_BLOCK_T>
Object::Animator::Specific SceneDrawer<ACTIVE_BLOCK_T>::gravityAnimator;

template <typename ACTIVE_BLOCK_T>
Res::Slides SceneDrawer<ACTIVE_BLOCK_T>::gravitySlides;

#endif
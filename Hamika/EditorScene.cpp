#include "EditorScene.h"
#include "Tools.h"
#include "EditorMainEvent.h"
#include "EditorObjects.h"

namespace UI::Editor::Scene
{
	Edit::Edit()
	{
		drawer.blockRefreshActive = false;
		drawer.layerActive = false;

		fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (button_ == 2)
			{
				if (this->onPanel(x_, y_))
				{
					MainEvent::s_object->controlPanel->pickID(reach(map)[drawer.GetFromCursor(x_ - x(), y_ - y())].object->id);

					return true;
				}
			}
			return false;
		});

		fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS) -> FNC_KEY_DOWN_RET
		{
			if (key_ == ALLEGRO_KEY_LEFT)
			{
				reach(map)[targetCoord].Redrawn = true;

				if (targetCoord.x == 0)
				{
					targetCoord.x = ((Type::Size)reach(map)).width - 1;
				}
				else
				{
					targetCoord.x -= 1;
				}
				reach(map)[targetCoord].Redrawn = true;

				drawer.MoveCameraTo({(float)targetCoord.x,(float)targetCoord.y});
				return true;
			}
			if (key_ == ALLEGRO_KEY_RIGHT)
			{
				reach(map)[targetCoord].Redrawn = true;

				if (targetCoord.x == ((Type::Size)reach(map)).width - 1)
				{
					targetCoord.x = 0;
				}
				else
				{
					targetCoord.x += 1;
				}
				reach(map)[targetCoord].Redrawn = true;

				drawer.MoveCameraTo({(float)targetCoord.x,(float)targetCoord.y});
				return true;
			}
			if (key_ == ALLEGRO_KEY_UP)
			{
				reach(map)[targetCoord].Redrawn = true;

				if (targetCoord.y == 0)
				{
					targetCoord.y = ((Type::Size)reach(map)).height - 1;
				}
				else
				{
					targetCoord.y -= 1;
				}
				reach(map)[targetCoord].Redrawn = true;

				drawer.MoveCameraTo({(float)targetCoord.x,(float)targetCoord.y});
				return true;
			}
			if (key_ == ALLEGRO_KEY_DOWN)
			{
				reach(map)[targetCoord].Redrawn = true;

				if (targetCoord.y == ((Type::Size)reach(map)).height - 1)
				{
					targetCoord.y = 0;
				}
				else
				{
					targetCoord.y += 1;
				}
				reach(map)[targetCoord].Redrawn = true;

				drawer.MoveCameraTo({(float)targetCoord.x,(float)targetCoord.y});
				return true;
			}

			if (key_ == ALLEGRO_KEY_RSHIFT)
			{
				isRShift = true;
				MainEvent::s_object->controlPanel->setOperationMode();
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.Redrawn = true;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.Redrawn = true;
					}
				});
			}
			else if (key_ == ALLEGRO_KEY_RCTRL)
			{
				isRCtrl = true;
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.Redrawn = true;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.Redrawn = true;
					}
				});
			}
			return false;
		});

		fncKeyUp.push_back([&](FNC_KEY_UP_PARAMS) -> FNC_KEY_UP_RET
		{
			if (key_ == ALLEGRO_KEY_RSHIFT)
			{
				isRShift = false;
				MainEvent::s_object->controlPanel->setOperationMode();
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.Redrawn = true;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.Redrawn = true;
					}
				});
			}
			else if (key_ == ALLEGRO_KEY_RCTRL)
			{
				isRCtrl = false;
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.Redrawn = true;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.Redrawn = true;
					}
				});
			}
			return false;
		});

		fncLock.push_back([&](FNC_LOCK_PARAMS)->FNC_LOCK_RET
		{
			if (map->Exists())
			{
				if (isRShift || isRCtrl)
				{
					mouseSelectHold = true;
					mouseMoveHold = false;
					holdCoordBegin = drawer.GetFromCursor(x_ - x(), y_ - y());
				}
				else
				{
					mouseMoveHold = true;
					mouseSelectHold = false;
					holdCamera = drawer.GetCamera();
					mouseHoldx = x_;
					mouseHoldy = y_;
				}
			}
		});

		fncMouseAxes.push_back([&](FNC_MOUSE_AXES_PARAMS)->FNC_MOUSE_AXES_RET
		{
			if (map->Exists())
			{
				if (mouseSelectHold)
				{
					holdCoordEnd = drawer.GetFromCursor(x_ - x(), y_ - y());
					Type::Coord lu = {(std::min)(holdCoordBegin.x,holdCoordEnd.x),(std::min)(holdCoordBegin.y,holdCoordEnd.y)};
					Type::Coord rd = {(std::max)(holdCoordBegin.x,holdCoordEnd.x) + 1,(std::max)(holdCoordBegin.y,holdCoordEnd.y) + 1};

					map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
					{
						block.selectTmp2 = block.selectTmp1 || block.deselectTmp1;
						block.selectTmp1 = false;
						block.deselectTmp1 = false;
					});

					map->forrange(lu, rd, [&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
					{
						if (isRShift)
						{
							block.selectTmp1 = true;
						}
						if (isRCtrl)
						{
							block.deselectTmp1 = true;
						}
					});

					map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
					{
						if (isRShift)
						{
							if (block.selectTmp1 != block.selectTmp2)
							{
								block.Redrawn = true;
							}
						}
						else if (isRCtrl)
						{
							if (block.deselectTmp1 != block.selectTmp2)
							{
								block.Redrawn = true;
							}
						}
					});



				}
				else if (mouseMoveHold)
				{
					mouseAxe = true;
					Type::Move camera = {
						holdCamera.x + ((mouseHoldx - x_) / (Type::Move::Type)drawer.GetDrawSize().width),
						holdCamera.y + ((mouseHoldy - y_) / (Type::Move::Type)drawer.GetDrawSize().height)
					};
					setTarget(camera);
				}
			}
		});

		fncUnlock.push_back([&](FNC_UNLOCK_PARAMS)->FNC_UNLOCK_RET
		{
			if (mouseSelectHold)
			{
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
				{
					if (block.selectTmp1)
					{
						block.selected = true;
						block.selectTmp1 = false;
						block.Redrawn = true;
					}
					if (block.deselectTmp1)
					{
						block.selected = false;
						block.deselectTmp1 = false;
						block.Redrawn = true;
					}
				});
				MainEvent::s_object->miniMap->updateBlocks();
			}
			if (mouseMoveHold)
			{
				mouseMoveHold = false;
				if (mouseAxe)
				{
					mouseAxe = false;
				}
				else
				{
					reach(map)[targetCoord].Redrawn = true;
					targetCoord = drawer.GetFromCursor(x_ - x(), y_ - y());
					reach(map)[targetCoord].Redrawn = true;
				}
			}
		});

		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			if (map->Exists())
			{
				drawer.DrawBlocks(x_, y_);
			}
		}));

		fncMoved.push_back([&](FNC_MOVED_PARAMS) ->FNC_MOVED_RET
		{
			if (map->Exists())
			{
				Type::Move camera = drawer.GetCamera();
				drawer.InitializeDrawOptions({width(), height()}, Type::CameraSize::Invalid);
				setTarget(camera);
			}
		});
	}
	Edit::~Edit()
	{
	}

	void Edit::setTarget(Type::Move camera)
	{
		if (map->Exists())
		{
			drawer.MoveCameraTo(camera);
			MainEvent::s_object->miniMap->updatePosition(drawer.GetCamera(), drawer.GetCameraSize());
		}
	}

	bool Edit::isOperationModeAll() const
	{
		return isRShift;
	}
	Type::Coord Edit::getTarget() const
	{
		return targetCoord;
	}

	void Edit::blocksUpdated()
	{
		MainEvent::s_object->miniMap->updateBlocks();
		MainEvent::s_object->controlPanel->updateBlocks();
	}
	void Edit::mapLayoutUpdated()
	{
		targetCoord = {0,0};
		map->foreach([&](const Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
		{
			if (block.object == nullptr)
			{
				block.object = new EditorObjectBase;
				ObjectCreate(this, block.object, ObjectID::Space, coord);
				block.object->rotation = Type::Rotations::Up;
			}
			else
			{
				ObjectCreate(block.object, block.object->id, coord);
			}
			if (block.remain == nullptr)
			{
				block.remain = new EditorObjectBase;
				ObjectCreate(this, block.remain, -1, coord);
				block.remain->drawnerFnc = Object::Editor::Remain::Drawner;
			}
			else
			{
				ObjectCreate(block.remain, block.remain->id, coord);
				block.remain->drawnerFnc = Object::Editor::Remain::Drawner;
			}

			block.GoTo = coord;
			block.ComeFrom = coord;
			block.Redrawn = true;
		});
		drawer.SetMap(map);
		drawer.InitializeDrawOptions({width(), height()}, Type::CameraSize::Invalid);
		drawer.MoveCameraTo({0,0});
		MainEvent::s_object->miniMap->SetMap(map);
		MainEvent::s_object->miniMap->updatePosition(drawer.GetCamera(), drawer.GetCameraSize());
	}
	void Edit::SetMap(std::shared_ptr<Res::BluePrint> &bluePrint)
	{
		map.reset(new Matrix<SceneBlock<EditorObjectBase>>(bluePrint->blocks));

		map->foreach([&](const Type::Coord &coord, SceneBlock<EditorObjectBase> &block)
		{
			block.object = new EditorObjectBase;
			ObjectCreate(this, block.object, (*bluePrint)[coord].id, coord);
			block.object->rotation = (*bluePrint)[coord].rotation;

			block.remain = new EditorObjectBase;
			ObjectCreate(this, block.remain, -1, coord);
			block.remain->drawnerFnc = Object::Editor::Remain::Drawner;

			block.grid = (*bluePrint)[coord].flags;
		});
		mapLayoutUpdated();
		MainEvent::s_object->controlPanel->SetMap(bluePrint, map);
	}

	Type::Size Edit::GetDrawSize() const
	{
		return drawer.GetDrawSize();
	}
	Type::Size Edit::GetDrawOffSet() const
	{
		return drawer.GetDrawOffSet();
	}
	EditorObjectBase *Edit::GetObject(Type::Coord coord)
	{
		return (*map)[coord].object;
	}
	Type::Flags Edit::GetBlockFlags(Type::Coord coord) const
	{
		if ((*map).Test(coord))
			return (*map)[coord].grid;
		return 0;
	}
	int Edit::selectStatus(Type::Coord coord) const
	{
		int ret = 0;
		if ((*map)[coord].selected)
		{
			ret |= 1 << 0;
		}
		if ((*map)[coord].deselectTmp1)
		{
			ret |= 1 << 1;
		}
		if ((*map)[coord].selectTmp1)
		{
			ret |= 1 << 2;
		}
		return ret;
	}
	bool Edit::isTarget(Type::Coord coord) const
	{
		return targetCoord == coord;
	}
}
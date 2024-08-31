#include "EditorScene.h"
#include "Tools.h"
#include "EditorMainEvent.h"
#include "EditorObjects.h"

namespace Editor::UI::Scene
{
	Edit::Edit()
	{
		drawer.enableLayer(SceneLayer::blocks, true);
		drawer.enableLayer(SceneLayer::coord, true);

		fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS) -> FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (button_ == 2)
			{
				if (this->onPanel(x_, y_))
				{
					MainEvent::s_object->controlPanel->pickID(reach(map)[drawer.getTargetOfCursor({x_ - x(), y_ - y()})].object->id);

					return true;
				}
			}
			return false;
		});

		fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS) -> FNC_KEY_DOWN_RET
		{
			if (key_ == ALLEGRO_KEY_LEFT)
			{
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				if (targetCoord.x() == 0)
				{
					targetCoord.x() = map->size().width() - 1;
				}
				else
				{
					targetCoord.x() -= 1;
				}
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				drawer.moveCamera({(float)targetCoord.x(),(float)targetCoord.y()});
				return true;
			}
			if (key_ == ALLEGRO_KEY_RIGHT)
			{
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				if (targetCoord.x() == map->size().width() - 1)
				{
					targetCoord.x() = 0;
				}
				else
				{
					targetCoord.x() += 1;
				}
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				drawer.moveCamera({(float)targetCoord.x(),(float)targetCoord.y()});
				return true;
			}
			if (key_ == ALLEGRO_KEY_UP)
			{
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				if (targetCoord.y() == 0)
				{
					targetCoord.y() = map->size().height() - 1;
				}
				else
				{
					targetCoord.y() -= 1;
				}
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				drawer.moveCamera({(float)targetCoord.x(),(float)targetCoord.y()});
				return true;
			}
			if (key_ == ALLEGRO_KEY_DOWN)
			{
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				if (targetCoord.y() == map->size().height() - 1)
				{
					targetCoord.y() = 0;
				}
				else
				{
					targetCoord.y() += 1;
				}
				reach(map)[targetCoord].redrawn_type_ = RedrawType::All;

				drawer.moveCamera({(float)targetCoord.x(),(float)targetCoord.y()});
				return true;
			}

			if (key_ == ALLEGRO_KEY_RSHIFT)
			{
				isRShift = true;
				MainEvent::s_object->controlPanel->setOperationMode();
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
				});
			}
			else if (key_ == ALLEGRO_KEY_RCTRL)
			{
				isRCtrl = true;
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
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
				map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
				});
			}
			else if (key_ == ALLEGRO_KEY_RCTRL)
			{
				isRCtrl = false;
				mouseMoveHold = false;
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selectTmp1)
					{
						block.selectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
					if (block.deselectTmp1)
					{
						block.deselectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
				});
			}
			return false;
		});

		fncLock.push_back([&](FNC_LOCK_PARAMS) -> FNC_LOCK_RET
		{
			if (map->Exists())
			{
				if (isRShift || isRCtrl)
				{
					mouseSelectHold = true;
					mouseMoveHold = false;
					holdCoordBegin = drawer.getTargetOfCursor({x_ - x(), y_ - y()});
				}
				else
				{
					mouseMoveHold = true;
					mouseSelectHold = false;
					holdCamera = drawer.getCameraCenter();
					mouseHoldx = x_;
					mouseHoldy = y_;
				}
			}
		});

		fncMouseAxes.push_back([&](FNC_MOUSE_AXES_PARAMS) -> FNC_MOUSE_AXES_RET
		{
			if (map->Exists())
			{
				if (mouseSelectHold)
				{
					holdCoordEnd = drawer.getTargetOfCursor({x_ - x(), y_ - y()});
					Type::Coord lu = {(std::min)(holdCoordBegin.x(),holdCoordEnd.x()),(std::min)(holdCoordBegin.y(),holdCoordEnd.y())};
					Type::Coord rd = {(std::max)(holdCoordBegin.x(),holdCoordEnd.x()) + 1,(std::max)(holdCoordBegin.y(),holdCoordEnd.y()) + 1};

					map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						block.selectTmp2 = block.selectTmp1 || block.deselectTmp1;
						block.selectTmp1 = false;
						block.deselectTmp1 = false;
					});

					map->forrange(lu, rd, [&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
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

					map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (isRShift)
						{
							if (block.selectTmp1 != block.selectTmp2)
							{
								block.redrawn_type_ = RedrawType::All;
							}
						}
						else if (isRCtrl)
						{
							if (block.deselectTmp1 != block.selectTmp2)
							{
								block.redrawn_type_ = RedrawType::All;
							}
						}
					});



				}
				else if (mouseMoveHold)
				{
					mouseAxe = true;
					Type::Move camera = {
						holdCamera.x() + ((mouseHoldx - x_) / (Type::Move::base)drawer.getBlockSize().width()),
						holdCamera.y() + ((mouseHoldy - y_) / (Type::Move::base)drawer.getBlockSize().height())
					};
					setTarget(camera);
				}
			}
		});

		fncUnlock.push_back([&](FNC_UNLOCK_PARAMS) -> FNC_UNLOCK_RET
		{
			if (mouseSelectHold)
			{
				mouseSelectHold = false;
				map->foreach([&](Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selectTmp1)
					{
						block.selected = true;
						block.selectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
					}
					if (block.deselectTmp1)
					{
						block.selected = false;
						block.deselectTmp1 = false;
						block.redrawn_type_ = RedrawType::All;
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
					reach(map)[targetCoord].redrawn_type_ = RedrawType::All;
					targetCoord = drawer.getTargetOfCursor({x_ - x(), y_ - y()});
					reach(map)[targetCoord].redrawn_type_ = RedrawType::All;
				}
			}
		});

		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			if (map->Exists())
			{
				drawer.updateBitmap();
				drawer.drawBitmaps({x_, y_});
			}
		}));

		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			if (map->Exists())
			{
				Type::Move camera = drawer.getCameraCenter();
				drawer.updateConfiguration({width(), height()}, Type::CameraSize::Invalid);
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
			drawer.moveCamera(camera);
			MainEvent::s_object->miniMap->updatePosition(drawer.getCameraCenter(), drawer.getCameraSizeInBlocks());
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
		map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
		{
			if (block.object == nullptr)
			{
				block.object = new Object::Brick;
				ObjectCreate(this, block.object, ObjectID::Space, coord);
				block.object->rotation = Type::Rotations::Up;
			}
			else
			{
				ObjectCreate(block.object, block.object->id, coord);
			}
			if (block.remain == nullptr)
			{
				block.remain = new Object::Brick;
				ObjectCreate(this, block.remain, -1, coord);
				block.remain->drawnerFnc = Object::Front::Drawner;
			}
			else
			{
				ObjectCreate(block.remain, block.remain->id, coord);
				block.remain->drawnerFnc = Object::Front::Drawner;
			}

			block.GoTo = coord;
			block.ComeFrom = coord;
			block.redrawn_type_ = RedrawType::All;
		});
		drawer.setMap(map);
		drawer.updateConfiguration({width(), height()}, Type::CameraSize::Invalid);
		drawer.moveCamera({0,0});
		MainEvent::s_object->miniMap->SetMap(map);
		MainEvent::s_object->miniMap->updatePosition(drawer.getCameraCenter(), drawer.getCameraSizeInBlocks());
	}
	void Edit::SetMap(std::shared_ptr<Res::BluePrint> &bluePrint)
	{
		map.reset(new Matrix<SceneBlock<Object::Brick>>(bluePrint->blocks));

		map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
		{
			block.object = new Object::Brick;
			ObjectCreate(this, block.object, (*bluePrint)[coord].id, coord);
			block.object->rotation = (*bluePrint)[coord].rotation;

			block.remain = new Object::Brick;
			ObjectCreate(this, block.remain, -1, coord);
			block.remain->drawnerFnc = Object::Front::Drawner;

			block.grid = (*bluePrint)[coord].flags;
		});
		mapLayoutUpdated();
		MainEvent::s_object->controlPanel->SetMap(bluePrint, map);
	}

	Type::Pixels Edit::getBlockSize() const
	{
		return drawer.getBlockSize();
	}
	Object::Brick *Edit::GetObject(Type::Coord coord)
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
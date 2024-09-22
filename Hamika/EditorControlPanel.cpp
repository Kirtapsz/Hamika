#include "EditorControlPanel.h"
#include "Font.h"
#include "IDreg.h"
#include "Tools.h"
#include "EditorMainEvent.h"
#include <KIR/AL/KIR5_panel_control.h>

namespace Editor::UI
{
	const std::string ControlPanel::GLOBAL_GRAVITY_ON = "Global gravity is ON";
	const std::string ControlPanel::GLOBAL_GRAVITY_OFF = "Global gravity is OFF";


	ControlPanel::BlockIDPanel::BlockIDPanel()
	{
		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			font = Res::TimesNewRoman[height() / 3];
		});

		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			DrawObject(id, x_, y_, w_, h_);
			if (counter >= 0)
			{
				font.draw(x_ + 1, y_ + 1, std::to_string(counter), KIR5::Color(0, 0, 0));
				font.draw(x_, y_, std::to_string(counter), KIR5::Color(255, 255, 255));
			}
		}));
	}
	ControlPanel::ControlPanel()
	{
		*this << titleOfBluePrint_Label;
		titleOfBluePrint_Label->setText("Title: ");
		titleOfBluePrint_Label->alignWidthToText();

		*this << titleOfBluePrint_TextBox;
		titleOfBluePrint_TextBox->setText("This is the title of the map");
		titleOfBluePrint_TextBox->alignWidthToText();

		*this << sizeOfCamera_Label;
		sizeOfCamera_Label->setText("Camera: ");
		sizeOfCamera_Label->alignWidthToText();

		*this << sizeOfCamera_TextBox;
		sizeOfCamera_TextBox->setText("10.000*10.000");
		sizeOfCamera_TextBox->alignWidthToText();

		*this << murphySpeed_Label;
		murphySpeed_Label->setText("Murphy: ");
		murphySpeed_Label->alignWidthToText();

		*this << murphySpeed_TextBox;
		murphySpeed_TextBox->setText("10.0000");
		murphySpeed_TextBox->alignWidthToText();

		*this << saveBluePrint_Button;
		saveBluePrint_Button->setText("SAVE");
		saveBluePrint_Button->width(saveBluePrint_Button->getTextWidth() + 10);
		saveBluePrint_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
		{
			bluePrint->blocks.resize(reach(map));
			bluePrint->blocks.foreach([&](Type::Coord coord, Res::BluePrint::Block &block)
			{
				block.id = reach(map)[coord].object->id;
				block.flags = reach(map)[coord].grid;
				block.rotation = reach(map)[coord].object->rotation;
			});
			bluePrint->scoreToUnlock = std::atol(infotron_ToCollect_TextBox->getText().c_str());
			bluePrint->title = titleOfBluePrint_TextBox->getText();
			bluePrint->globalGravity = globalGravityOnOffButton->getText() == GLOBAL_GRAVITY_ON;

			float w = -1, h = -1;
			if (sscanf_s(sizeOfCamera_TextBox->getText().c_str(), "%f*%f", &w, &h) == 2)
			{
				bluePrint->cameraSize = {w,h};
			}

			float s = 0;
			if (sscanf_s(murphySpeed_TextBox->getText().c_str(), "%f", &s) == 1)
			{
				bluePrint->speed = s;
			}

			eventEngine->sendEvent((void *)REFRESH_ITEM, (void *)(bluePrint.get()), (void *)0, (void *)0);
		});


		{
			*rotation_Panel << rotation_Label;
			rotation_Label->setText("Rotation");
			rotation_Label->width(rotation_Label->getTextWidth() + 10);

			for (size_t i = 0; i < rotation_Navigation_Buttons.size(); ++i)
			{
				*rotation_Panel << rotation_Navigation_Buttons[i];
				rotation_Navigation_Buttons[i]->setBitmap(Res::uielements[Res::UIElements::Stick[i]]);
				rotation_Navigation_Buttons[i]->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
				{
					for (auto &it : rotation_Navigation_Buttons)
					{
						it->setColor(KIR5::Color(30, 30, 30));
						it->isSelected = false;
					}

					dynamic_cast<ResizeButtonClass *>(obj_)->setColor(KIR5::Color(200, 60, 30));
					dynamic_cast<ResizeButtonClass *>(obj_)->isSelected = true;
				});
			}

			KIR5::Event::EventCallback(rotation_Navigation_Buttons[0]->fncPress, rotation_Navigation_Buttons[0].get(), 0, 0);

			*rotation_Panel << rotation_Apply_Button;
			rotation_Apply_Button->setBitmap(Res::uielements[Res::UIElements::Execute]);
			rotation_Apply_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				Type::Rotation r =
					rotation_Navigation_Buttons[0]->isSelected ? Type::Rotations::Up :
					rotation_Navigation_Buttons[1]->isSelected ? Type::Rotations::Right :
					rotation_Navigation_Buttons[2]->isSelected ? Type::Rotations::Down :
					Type::Rotations::Left;

				if (MainEvent::s_object->scene->isOperationModeAll())
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							block.object->rotation = r;
							block.redrawn_type_ = RedrawType::All;
						}
					});
				}
				else
				{
					reach(map)[MainEvent::s_object->scene->getTarget()].object->rotation = r;
					reach(map)[MainEvent::s_object->scene->getTarget()].redrawn_type_ = RedrawType::All;
				}
				MainEvent::s_object->scene->blocksUpdated();
			});

			*this << rotation_Panel;
		}

		{
			*bluePrintResize_Panel << bluePrintResize_Label;
			bluePrintResize_Label->setText("Resize");
			bluePrintResize_Label->width(bluePrintResize_Label->getTextWidth() + 10);

			*bluePrintResize_Panel << bluePrintResize_Size_TextBox;
			bluePrintResize_Size_TextBox->setTextAlignment(KIR5::CENTER);

			*bluePrintResize_Panel << bluePrintResize_Apply_Button;
			bluePrintResize_Apply_Button->setBitmap(Res::uielements[Res::UIElements::Execute]);
			bluePrintResize_Apply_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				int w, h;
				if (sscanf_s(bluePrintResize_Size_TextBox->getText().c_str(), "%d*%d", &w, &h) == 2 && w >= 2 && h >= 2)
				{
					Matrix<SceneBlock<Object::Brick>> tmpMap(std::move(*(map.get())));
					map->resize({w,h});

					int xShift = 0;
					int yShift = 0;

					int sxShift = 0;
					int syShift = 0;

					//X
					if (bluePrintResize_Navigation_Buttons[0][0]->isSelected || bluePrintResize_Navigation_Buttons[0][1]->isSelected || bluePrintResize_Navigation_Buttons[0][2]->isSelected)
					{
						xShift = 0;
					}
					if (bluePrintResize_Navigation_Buttons[1][0]->isSelected || bluePrintResize_Navigation_Buttons[1][1]->isSelected || bluePrintResize_Navigation_Buttons[1][2]->isSelected)
					{
						xShift = (tmpMap.width() - w) / 2;
					}
					if (bluePrintResize_Navigation_Buttons[2][0]->isSelected || bluePrintResize_Navigation_Buttons[2][1]->isSelected || bluePrintResize_Navigation_Buttons[2][2]->isSelected)
					{
						xShift = tmpMap.width() - w;
					}

					//Y
					if (bluePrintResize_Navigation_Buttons[0][0]->isSelected || bluePrintResize_Navigation_Buttons[1][0]->isSelected || bluePrintResize_Navigation_Buttons[2][0]->isSelected)
					{
						yShift = 0;
					}
					if (bluePrintResize_Navigation_Buttons[0][1]->isSelected || bluePrintResize_Navigation_Buttons[1][1]->isSelected || bluePrintResize_Navigation_Buttons[2][1]->isSelected)
					{
						yShift = (tmpMap.height() - h) / 2;
					}
					if (bluePrintResize_Navigation_Buttons[0][2]->isSelected || bluePrintResize_Navigation_Buttons[1][2]->isSelected || bluePrintResize_Navigation_Buttons[2][2]->isSelected)
					{
						yShift = tmpMap.height() - h;
					}

					if (yShift < 0)
					{
						syShift = -yShift;
						yShift = 0;
					}
					if (xShift < 0)
					{
						sxShift = -xShift;
						xShift = 0;
					}

					Type::Coord coord;
					for (coord.x() = 0; coord.x() < (std::min)(((Type::Size)(*map)).width(), ((Type::Size)(tmpMap)).width() - xShift); ++coord.x())
					{
						for (coord.y() = 0; coord.y() < (std::min)(((Type::Size)(*map)).height(), ((Type::Size)(tmpMap)).height() - yShift); ++coord.y())
						{
							(*map)[{coord.x() + sxShift, coord.y() + syShift}] = tmpMap[{coord.x() + xShift, coord.y() + yShift}];
							tmpMap[{coord.x() + xShift, coord.y() + yShift}].object = nullptr;
							tmpMap[{coord.x() + xShift, coord.y() + yShift}].remain = nullptr;
						}
					}

					MainEvent::s_object->scene->mapLayoutUpdated();
				}
				else
				{
					bluePrintResize_Size_TextBox->setText(std::to_string(((Type::Size)(*map)).width()) + "*" + std::to_string(((Type::Size)(*map)).height()));
				}
			});

			auto bluePrintResize_Navigation_Buttons_FncPress = [&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				int px = 0;
				int py = 0;

				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						bluePrintResize_Navigation_Buttons[x][y]->setBitmap(KIR5::Bitmap());
						if (bluePrintResize_Navigation_Buttons[x][y].get() == obj_)
						{
							px = x;
							py = y;
						}
					}
				}

				constexpr static auto setBmp = [](int x, int y, KIR5::Bitmap bitmap, std::array<std::array<std::shared_ptr<ResizeButtonClass>, 3>, 3> &bluePrintResize_Navigation_Buttons)->void
				{
					if (x >= 0 && x < 3 && y >= 0 && y < 3)
					{
						bluePrintResize_Navigation_Buttons[x][y]->setBitmap(bitmap);
					}
				};

				bluePrintResize_Navigation_Buttons[px][py]->isSelected = true;

				setBmp(px - 1, py - 1, Res::uielements[Res::UIElements::ArrowTiltedUpLeft], bluePrintResize_Navigation_Buttons);
				setBmp(px, py - 1, Res::uielements[Res::UIElements::ArrowUp], bluePrintResize_Navigation_Buttons);
				setBmp(px + 1, py - 1, Res::uielements[Res::UIElements::ArrowTiltedUpRight], bluePrintResize_Navigation_Buttons);

				setBmp(px - 1, py, Res::uielements[Res::UIElements::ArrowLeft], bluePrintResize_Navigation_Buttons);
				setBmp(px, py, Res::uielements[Res::UIElements::Ball], bluePrintResize_Navigation_Buttons);
				setBmp(px + 1, py, Res::uielements[Res::UIElements::ArrowRight], bluePrintResize_Navigation_Buttons);

				setBmp(px - 1, py + 1, Res::uielements[Res::UIElements::ArrowTiltedDownLeft], bluePrintResize_Navigation_Buttons);
				setBmp(px, py + 1, Res::uielements[Res::UIElements::ArrowDown], bluePrintResize_Navigation_Buttons);
				setBmp(px + 1, py + 1, Res::uielements[Res::UIElements::ArrowTiltedDownRight], bluePrintResize_Navigation_Buttons);
			};

			for (int x = 0; x < 3; ++x)
			{
				for (int y = 0; y < 3; ++y)
				{
					*bluePrintResize_Panel << bluePrintResize_Navigation_Buttons[x][y];
					bluePrintResize_Navigation_Buttons[x][y]->fncPress.push_back(bluePrintResize_Navigation_Buttons_FncPress);
				}
			}

			KIR5::Event::EventCallback(bluePrintResize_Navigation_Buttons[1][1]->fncPress, bluePrintResize_Navigation_Buttons[1][1].get(), 0, 0);

			*this << bluePrintResize_Panel;
		}

		{
			*s_objectPanel << s_objectLabel;
			s_objectLabel->setText("Object");
			s_objectLabel->width(s_objectLabel->getTextWidth() + 10);

			*s_objectPanel << s_objectID_Panel;
			s_objectID_Panel->fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS) -> FNC_MOUSE_BUTTON_DOWN_RET
			{
				if (s_objectID_Panel->onPanel(x_, y_))
				{
					if (button_ == 1)
					{
						if (s_objectID_Panel->id == GetObjectNumber() - 1)
						{
							s_objectID_Panel->id = 0;
						}
						else
						{
							s_objectID_Panel->id++;
						}
						s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
						updateBlocks();
						return true;
					}
					else if (button_ == 2)
					{
						if (s_objectID_Panel->id == 0)
						{
							s_objectID_Panel->id = GetObjectNumber() - 1;
						}
						else
						{
							s_objectID_Panel->id--;
						}
						s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
						updateBlocks();
						return true;
					}
				}
				return false;
			});

			*s_objectPanel << s_objectID_TextBox;
			s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
			s_objectID_TextBox->setTextAlignment(KIR5::CENTER);
			s_objectID_TextBox->fncGetFocus.push_back([&](FNC_GET_FOCUS_PARAMS) -> FNC_GET_FOCUS_RET
			{
				s_objectID_TextBox->setText("");
			});

			s_objectID_TextBox->fncLossFocus.push_back([&](FNC_LOSS_FOCUS_PARAMS) -> FNC_LOSS_FOCUS_RET
			{
				s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
			});

			s_objectID_TextBox->fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS) -> FNC_KEY_DOWN_RET
			{
				if (s_objectID_TextBox->isActiveBox())
				{
					if (key_ == ALLEGRO_KEY_ENTER)
					{
						s_objectID_Panel->id = limiter<int>(0, GetObjectNumber() - 1, std::atoi(s_objectID_TextBox->getText().c_str()));
						eventEngine->eventDestroyed(s_objectID_TextBox.get());
						return true;
					}
				}
				return false;
			});

			*s_objectPanel << s_objectApply_Button;
			s_objectApply_Button->setBitmap(Res::uielements[Res::UIElements::Execute]);
			s_objectApply_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (MainEvent::s_object->scene->isOperationModeAll())
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							ObjectCreate(block.object, s_objectID_Panel->id, coord);
							block.redrawn_type_ = RedrawType::All;
						}
					});
				}
				else
				{
					ObjectCreate(reach(map)[MainEvent::s_object->scene->getTarget()].object, s_objectID_Panel->id, MainEvent::s_object->scene->getTarget());
					reach(map)[MainEvent::s_object->scene->getTarget()].redrawn_type_ = RedrawType::All;
				}
				MainEvent::s_object->scene->blocksUpdated();

				int shiftFrom = prevBlockPickers.size() - 1;
				for (size_t i = 0; i < prevBlockPickers.size(); ++i)
				{
					if (prevBlockPickers[i]->id == s_objectID_Panel->id)
					{
						shiftFrom = i;
						break;
					}
				}
				for (int i = shiftFrom; i > 0; --i)
				{
					prevBlockPickers[i]->id = prevBlockPickers[i - 1]->id;
				}
				prevBlockPickers[0]->id = s_objectID_Panel->id;
			});

			*s_objectPanel << s_objectRandomFill_Button;
			s_objectRandomFill_Button->setText("Random fill");
			s_objectRandomFill_Button->width(s_objectRandomFill_Button->getTextWidth() + 10);
			s_objectRandomFill_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				float chancef = std::atof(s_objectRate_TextBox->getText().c_str());
				int chance = (int)(chancef * 10);
				map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selected && rand() % 1000 < chance)
					{
						ObjectCreate(block.object, s_objectID_Panel->id, coord);
						block.redrawn_type_ = RedrawType::All;
					}
				});
				MainEvent::s_object->scene->blocksUpdated();
			});

			*s_objectPanel << s_objectRate_TextBox;
			s_objectRate_TextBox->setText("100.0");
			s_objectRate_TextBox->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER);

			*s_objectPanel << s_objectRandomFill_Label;
			s_objectRandomFill_Label->setText("%");
			s_objectRandomFill_Label->width(bluePrintResize_Label->getTextWidth() + 5);

			*s_objectPanel << s_objectFillFrame_Button;
			s_objectFillFrame_Button->setText("Fill frame");
			s_objectFillFrame_Button->width(120);
			s_objectFillFrame_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (coord.x() == 0 || coord.y() == 0 || coord.x() == ((Type::Size)reach(map)).width() - 1 || coord.y() == ((Type::Size)reach(map)).height() - 1)
					{
						ObjectCreate(block.object, s_objectID_Panel->id, coord);
						block.redrawn_type_ = RedrawType::All;
					}
				});
				MainEvent::s_object->scene->blocksUpdated();
			});

			*s_objectPanel << s_objectFillContent_Button;
			s_objectFillContent_Button->setText("Fill content");
			s_objectFillContent_Button->width(120);
			s_objectFillContent_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (coord.x() != 0 && coord.y() != 0 && coord.x() != ((Type::Size)reach(map)).width() - 1 && coord.y() != ((Type::Size)reach(map)).height() - 1)
					{
						ObjectCreate(block.object, s_objectID_Panel->id, coord);
						block.redrawn_type_ = RedrawType::All;
					}
				});
				MainEvent::s_object->scene->blocksUpdated();
			});

			*s_objectPanel << s_objectselectAllOfThisType_Button;
			s_objectselectAllOfThisType_Button->setText("Select all");
			s_objectselectAllOfThisType_Button->width(120);
			s_objectselectAllOfThisType_Button->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.object->id == s_objectID_Panel->id)
					{
						if (!block.selected)
						{
							block.selected = true;
							block.redrawn_type_ = RedrawType::All;
						}
					}
					else
					{
						if (block.selected)
						{
							block.selected = false;
							block.redrawn_type_ = RedrawType::All;
						}
					}
				});
				MainEvent::s_object->scene->blocksUpdated();
			});

			*this << s_objectPanel;
		}

		{
			struct INF_
			{
				Type::ID id;
				int multiplier;
			};
			std::array<INF_, 2> infotrons{{
				{ObjectID::Infotron,1},
				{ObjectID::Electrons,9}
				}};

			*infotron_Panel << infotron_Label;
			infotron_Label->setText("Infotrons");
			infotron_Label->width(infotron_Label->getTextWidth() + 10);

			*infotron_Panel << infotron_ToCollect_TextBox;
			infotron_ToCollect_TextBox->setTextFont(Res::TimesNewRoman[24]);
			infotron_ToCollect_TextBox->setTextColor(KIR5::Color(152, 152, 152));
			infotron_ToCollect_TextBox->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER);

			*infotron_Panel << infotron_Capacity_Label;
			infotron_Capacity_Label->setText("/ 00000");
			infotron_Capacity_Label->width(infotron_Capacity_Label->getTextWidth() + 5);
			infotron_Capacity_Label->setText("/ 0");
			infotron_Capacity_Label->setTextColor(KIR5::Color(182, 122, 202));

			{
				infotron_Pickers_Buttons.resize(infotrons.size());
				for (size_t i = 0; i < infotrons.size(); ++i)
				{
					*infotron_Panel << infotron_Pickers_Buttons[i];
					infotron_Pickers_Buttons[i]->setAdjustParams(3 + i * 32 - (i / 5) * 5 * 32, 54 + (i / 5) * 32, 32, 32);
					infotron_Pickers_Buttons[i]->id = infotrons[i].id;
					infotron_Pickers_Buttons[i]->multiplier = infotrons[i].multiplier;
					infotron_Pickers_Buttons[i]->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
					{
						s_objectID_Panel->id = dynamic_cast<BlockIDPanel *>(obj_)->id;
						s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
					});
				}
			}

			*this << infotron_Panel;
		}

		{
			*special_Panel << selectAllButton;
			selectAllButton->setText("Select all");
			selectAllButton->width(selectAllButton->getTextWidth());
			selectAllButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				bool isNonSelected = false;
				map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (!block.selected)
					{
						isNonSelected = true;
					}
				});

				if (isNonSelected)
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (!block.selected)
						{
							block.selected = true;
							block.redrawn_type_ = RedrawType::All;
						}
					});
				}
				else
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							block.selected = false;
							block.redrawn_type_ = RedrawType::All;
						}
					});
				}
			});

			*special_Panel << gravityTurnOnOffButton;
			gravityTurnOnOffButton->setText("Set gravity");
			gravityTurnOnOffButton->width(gravityTurnOnOffButton->getTextWidth());
			gravityTurnOnOffButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (!MainEvent::s_object->scene->isOperationModeAll())
				{
					if (reach(map)[MainEvent::s_object->scene->getTarget()].grid & GridFlags::Gravity)
					{
						reach(map)[MainEvent::s_object->scene->getTarget()].grid &= ~GridFlags::Gravity;
					}
					else
					{
						reach(map)[MainEvent::s_object->scene->getTarget()].grid |= GridFlags::Gravity;
					}
					reach(map)[MainEvent::s_object->scene->getTarget()].redrawn_type_ = RedrawType::All;
				}
				else
				{
					bool isNew = false;
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							if (!(block.grid & GridFlags::Gravity))
							{
								isNew = true;
							}
						}
					});

					if (isNew)
					{
						map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
						{
							if (block.selected)
							{
								if (!(block.grid & GridFlags::Gravity))
								{
									block.grid |= GridFlags::Gravity;
									block.redrawn_type_ = RedrawType::All;
								}
							}
						});
					}
					else
					{
						map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
						{
							if (block.selected)
							{
								if (block.grid & GridFlags::Gravity)
								{
									block.grid &= ~GridFlags::Gravity;
									block.redrawn_type_ = RedrawType::All;
								}
							}
						});
					}
				}
				MainEvent::s_object->scene->blocksUpdated();
			});

			*special_Panel << initExplodeButton;
			initExplodeButton->setText("Set init explode");
			initExplodeButton->width(initExplodeButton->getTextWidth());
			initExplodeButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (!MainEvent::s_object->scene->isOperationModeAll())
				{
					if (reach(map)[MainEvent::s_object->scene->getTarget()].grid & GridFlags::Detonate)
					{
						reach(map)[MainEvent::s_object->scene->getTarget()].grid &= ~GridFlags::Detonate;
					}
					else
					{
						reach(map)[MainEvent::s_object->scene->getTarget()].grid |= GridFlags::Detonate;
					}
					reach(map)[MainEvent::s_object->scene->getTarget()].redrawn_type_ = RedrawType::All;
				}
				else
				{
					bool isNew = false;
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							if (!(block.grid & GridFlags::Detonate))
							{
								isNew = true;
							}
						}
					});

					if (isNew)
					{
						map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
						{
							if (block.selected)
							{
								if (!(block.grid & GridFlags::Detonate))
								{
									block.grid |= GridFlags::Detonate;
									block.redrawn_type_ = RedrawType::All;
								}
							}
						});
					}
					else
					{
						map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
						{
							if (block.selected)
							{
								if (block.grid & GridFlags::Detonate)
								{
									block.grid &= ~GridFlags::Detonate;
									block.redrawn_type_ = RedrawType::All;
								}
							}
						});
					}
				}
				MainEvent::s_object->scene->blocksUpdated();
			});

			*special_Panel << selectFriendlyButton;
			selectFriendlyButton->setText("Select neighbors");
			selectFriendlyButton->width(selectFriendlyButton->getTextWidth());
			selectFriendlyButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
				{
					if (block.selected)
					{
						block.selected = false;
						block.redrawn_type_ = RedrawType::All;
					}
				});

				reach(map)[MainEvent::s_object->scene->getTarget()].selected = true;
				Type::ID id = reach(map)[MainEvent::s_object->scene->getTarget()].object->id;

				bool isNewSelected = false;
				do
				{
					isNewSelected = false;
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (!block.selected && block.object->id == id &&
							(
								(map->Test({coord.x() - 1, coord.y() - 1}) && reach(map)[{coord.x() - 1, coord.y() - 1}].selected) ||
								(map->Test({coord.x() - 0, coord.y() - 1}) && reach(map)[{coord.x() - 0, coord.y() - 1}].selected) ||
								(map->Test({coord.x() + 1, coord.y() - 1}) && reach(map)[{coord.x() + 1, coord.y() - 1}].selected) ||

								(map->Test({coord.x() - 1, coord.y() - 0}) && reach(map)[{coord.x() - 1, coord.y() - 0}].selected) ||
								(map->Test({coord.x() + 1, coord.y() - 0}) && reach(map)[{coord.x() + 1, coord.y() - 0}].selected) ||

								(map->Test({coord.x() - 1, coord.y() + 1}) && reach(map)[{coord.x() - 1, coord.y() + 1}].selected) ||
								(map->Test({coord.x() - 0, coord.y() + 1}) && reach(map)[{coord.x() - 0, coord.y() + 1}].selected) ||
								(map->Test({coord.x() + 1, coord.y() + 1}) && reach(map)[{coord.x() + 1, coord.y() + 1}].selected)
								)
							)
						{
							block.selected = true;
							block.redrawn_type_ = RedrawType::All;
							isNewSelected = true;
						}
					});
				}
				while (isNewSelected);

				MainEvent::s_object->scene->blocksUpdated();
			});

			static constexpr Type::ID ramIDs[] = {
				ObjectID::RAMChipsMini,
				ObjectID::RAMChipsLeft ,
				ObjectID::RAMChipsRight,
				ObjectID::RAMChipsTop ,
				ObjectID::RAMChipsBottom ,
			};
			static constexpr int NUMBER_OF_RAMS = (sizeof(ramIDs) / sizeof(Type::ID));

			static std::function<bool(Type::ID id)> isRam = [](Type::ID id)->bool
			{
				for (int i = 0; i < NUMBER_OF_RAMS; ++i)
				{
					if (id == ramIDs[i])
					{
						return true;
					}
				}
				return false;
			};

			*special_Panel << ramRepairButton;
			ramRepairButton->setText("Repair RAMs");
			ramRepairButton->width(ramRepairButton->getTextWidth());
			ramRepairButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (MainEvent::s_object->scene->isOperationModeAll())
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							if (block.object->id == ObjectID::RAMChipsRight)
							{
								if (map->Test({coord.x() - 1,coord.y()}) && reach(map)[{coord.x() - 1, coord.y()}].object->id != ObjectID::RAMChipsLeft)
								{
									while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
									{
										ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
										block.redrawn_type_ = RedrawType::All;
									}
								}
							}
							if (block.object->id == ObjectID::RAMChipsBottom)
							{
								if (map->Test({coord.x(),coord.y() - 1}) && reach(map)[{coord.x(), coord.y() - 1}].object->id != ObjectID::RAMChipsTop)
								{
									while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
									{
										ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
										block.redrawn_type_ = RedrawType::All;
									}
								}
							}

							if (block.object->id == ObjectID::RAMChipsLeft)
							{
								if (map->Test({coord.x() + 1,coord.y()}) && reach(map)[{coord.x() + 1, coord.y()}].selected &&isRam(reach(map)[{coord.x() + 1, coord.y()}].object->id))
								{
									ObjectCreate(reach(map)[{coord.x() + 1, coord.y()}].object, ObjectID::RAMChipsRight, {coord.x() + 1,coord.y()});
									reach(map)[{coord.x() + 1, coord.y()}].redrawn_type_ = RedrawType::All;
								}
								else
								{
									ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
									block.redrawn_type_ = RedrawType::All;
								}
							}
							if (block.object->id == ObjectID::RAMChipsTop)
							{
								if (map->Test({coord.x(),coord.y() + 1}) && reach(map)[{coord.x(), coord.y() + 1}].selected &&isRam(reach(map)[{coord.x(), coord.y() + 1}].object->id))
								{
									ObjectCreate(reach(map)[{coord.x(), coord.y() + 1}].object, ObjectID::RAMChipsBottom, {coord.x(),coord.y() + 1});
									reach(map)[{coord.x(), coord.y() + 1}].redrawn_type_ = RedrawType::All;
								}
								else
								{
									ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
									block.redrawn_type_ = RedrawType::All;
								}
							}
						}
					});
				}
				else
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.object->id == ObjectID::RAMChipsRight)
						{
							if (map->Test({coord.x() - 1,coord.y()}) && reach(map)[{coord.x() - 1, coord.y()}].object->id != ObjectID::RAMChipsLeft)
							{
								while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
								{
									ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
									block.redrawn_type_ = RedrawType::All;
								}
							}
						}
						if (block.object->id == ObjectID::RAMChipsBottom)
						{
							if (map->Test({coord.x(),coord.y() - 1}) && reach(map)[{coord.x(), coord.y() - 1}].object->id != ObjectID::RAMChipsTop)
							{
								while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
								{
									ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
									block.redrawn_type_ = RedrawType::All;
								}
							}
						}

						if (block.object->id == ObjectID::RAMChipsLeft)
						{
							if (map->Test({coord.x() + 1,coord.y()}) && isRam(reach(map)[{coord.x() + 1, coord.y()}].object->id))
							{
								ObjectCreate(reach(map)[{coord.x() + 1, coord.y()}].object, ObjectID::RAMChipsRight, {coord.x() + 1,coord.y()});
								reach(map)[{coord.x() + 1, coord.y()}].redrawn_type_ = RedrawType::All;
							}
							else
							{
								ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
								block.redrawn_type_ = RedrawType::All;
							}
						}
						if (block.object->id == ObjectID::RAMChipsTop)
						{
							if (map->Test({coord.x(),coord.y() + 1}) && isRam(reach(map)[{coord.x(), coord.y() + 1}].object->id))
							{
								ObjectCreate(reach(map)[{coord.x(), coord.y() + 1}].object, ObjectID::RAMChipsBottom, {coord.x(),coord.y() + 1});
								reach(map)[{coord.x(), coord.y() + 1}].redrawn_type_ = RedrawType::All;
							}
							else
							{
								ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
								block.redrawn_type_ = RedrawType::All;
							}
						}
					});
				}
				MainEvent::s_object->scene->blocksUpdated();
			});

			*special_Panel << randomizeRamsButton;
			randomizeRamsButton->setText("Randomize RAMs");
			randomizeRamsButton->width(randomizeRamsButton->getTextWidth());
			randomizeRamsButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (MainEvent::s_object->scene->isOperationModeAll())
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected && isRam(block.object->id))
						{
							ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
							block.redrawn_type_ = RedrawType::All;
						}
					});
				}
				else
				{
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (isRam(block.object->id))
						{
							ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
							block.redrawn_type_ = RedrawType::All;
						}
					});
				}
				MainEvent::s_object->scene->blocksUpdated();
			});

			*special_Panel << spawnPointButton;
			spawnPointButton->setText("Set SPAWN");
			spawnPointButton->width(spawnPointButton->getTextWidth());
			spawnPointButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (!MainEvent::s_object->scene->isOperationModeAll())
				{
					if (reach(map)[MainEvent::s_object->scene->getTarget()].grid & GridFlags::SpawnPoint)
					{
						reach(map)[MainEvent::s_object->scene->getTarget()].grid &= ~GridFlags::SpawnPoint;
					}
					else
					{
						reach(map)[MainEvent::s_object->scene->getTarget()].grid |= GridFlags::SpawnPoint;
					}
					reach(map)[MainEvent::s_object->scene->getTarget()].redrawn_type_ = RedrawType::All;
				}
				else
				{
					bool isNew = false;
					map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
					{
						if (block.selected)
						{
							if (!(block.grid & GridFlags::SpawnPoint))
							{
								isNew = true;
							}
						}
					});

					if (isNew)
					{
						map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
						{
							if (block.selected)
							{
								if (!(block.grid & GridFlags::SpawnPoint))
								{
									block.grid |= GridFlags::SpawnPoint;
									block.redrawn_type_ = RedrawType::All;
								}
							}
						});
					}
					else
					{
						map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
						{
							if (block.selected)
							{
								if (block.grid & GridFlags::SpawnPoint)
								{
									block.grid &= ~GridFlags::SpawnPoint;
									block.redrawn_type_ = RedrawType::All;
								}
							}
						});
					}
				}
				MainEvent::s_object->scene->blocksUpdated();
			});

			*special_Panel << globalGravityOnOffButton;
			globalGravityOnOffButton->setText(GLOBAL_GRAVITY_OFF);
			globalGravityOnOffButton->width(globalGravityOnOffButton->getTextWidth());
			globalGravityOnOffButton->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				if (globalGravityOnOffButton->getText() == GLOBAL_GRAVITY_OFF)
				{
					globalGravityOnOffButton->setText(GLOBAL_GRAVITY_ON);
				}
				else
				{
					globalGravityOnOffButton->setText(GLOBAL_GRAVITY_OFF);
				}
			});

			*this << special_Panel;
		}
		{
			for (size_t i = 0; i < prevBlockPickers.size(); ++i)
			{
				prevBlockPickers[i]->id = (Type::ID)i;
				prevBlockPickers[i]->fncPress.push_back([&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
				{
					s_objectID_Panel->id = dynamic_cast<BlockIDPanel *>(obj_)->id;
					s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
				});
				*prevBlockPickers_Panel << prevBlockPickers[i];
				prevBlockPickers[i]->setAdjustParams(3 + i * 32 - (i / 12) * 12 * 32, 3 + (i / 12) * 32, 32, 32);
			}

			*this << prevBlockPickers_Panel;
		}

		this->fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS) -> FNC_KEY_DOWN_RET
		{
			if (
				eventEngine->getTargetPanel().get() == this ||
				eventEngine->getTargetPanel().get() == MainEvent::s_object->scene.get()
				)
			{
				if (key_ == ALLEGRO_KEY_ENTER)
				{
					KIR5::Event::EventCallback(s_objectApply_Button->fncPress, s_objectApply_Button.get(), 0, 0);
					return true;
				}
			}
			return false;
		});
	}
	ControlPanel::~ControlPanel()
	{

	}


	void ControlPanel::setOperationMode()
	{
		KIR5::Color bcolor = MainEvent::s_object->scene->isOperationModeAll() ? KIR5::Color(78, 30, 30) : KIR5::Color(30, 30, 78);

		rotation_Apply_Button->setColor(bcolor);
		s_objectApply_Button->setColor(bcolor);
		s_objectselectAllOfThisType_Button->setColor(bcolor);
		s_objectFillFrame_Button->setColor(bcolor);
		s_objectFillContent_Button->setColor(bcolor);
		gravityTurnOnOffButton->setColor(bcolor);
		initExplodeButton->setColor(bcolor);
		spawnPointButton->setColor(bcolor);

		randomizeRamsButton->setColor(bcolor);
		ramRepairButton->setColor(bcolor);
	}

	void ControlPanel::pickID(Type::ID id)
	{
		s_objectID_Panel->id = id;
		s_objectID_TextBox->setText(std::to_string(s_objectID_Panel->id));
		updateBlocks();
	}

	void ControlPanel::updateBlocks()
	{
		s_objectID_Panel->counter = 0;
		for (auto &it : infotron_Pickers_Buttons)
		{
			it->counter = 0;
		}
		this->map->foreach([&](const Type::Coord &coord, SceneBlock<Object::Brick> &block)
		{
			if (block.object->id == s_objectID_Panel->id)
			{
				++s_objectID_Panel->counter;
			}
			for (auto &it : infotron_Pickers_Buttons)
			{
				if (block.object->id == it->id)
				{
					++it->counter;
				}
			}
		});

		int totalInfotrons = 0;
		for (auto &it : infotron_Pickers_Buttons)
		{
			totalInfotrons += it->counter * it->multiplier;
		}
		infotron_Capacity_Label->setText("/" + std::to_string(totalInfotrons));

	}

	void ControlPanel::SetMap(std::shared_ptr<Res::BluePrint> &bluePrint_, std::shared_ptr<Matrix<SceneBlock<Object::Brick>>> &map)
	{
		bluePrint = bluePrint_;
		this->map = map;
		infotron_ToCollect_TextBox->setText(std::to_string(bluePrint->scoreToUnlock));
		titleOfBluePrint_TextBox->setText(bluePrint->title);
		bluePrintResize_Size_TextBox->setText(std::to_string(((Type::Size)(*map)).width()) + "*" + std::to_string(((Type::Size)(*map)).height()));

		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << bluePrint->cameraSize.width() << "*" << bluePrint->cameraSize.height();
			sizeOfCamera_TextBox->setText(stream.str());
		}

		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(4) << bluePrint->speed;
			murphySpeed_TextBox->setText(stream.str());
		}

		globalGravityOnOffButton->setText(bluePrint->globalGravity ? GLOBAL_GRAVITY_ON : GLOBAL_GRAVITY_OFF);
		updateBlocks();
	}
}
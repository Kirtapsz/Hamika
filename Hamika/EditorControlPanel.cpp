#include "EditorControlPanel.h"
#include "Font.h"
#include "IDreg.h"
#include "Tools.h"
#include "EditorMainEvent.h"
#include <KIR/AL/KIR5_panel_control.h>

namespace UI::Editor
{
	const std::string ControlPanel::GLOBAL_GRAVITY_ON = "Global gravity is ON";
	const std::string ControlPanel::GLOBAL_GRAVITY_OFF = "Global gravity is OFF";


	ControlPanel::BlockIDPanel::BlockIDPanel()
	{
		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			DrawObject(id, x_, y_, w_, h_);
			if (counter >= 0)
			{
				KIR5::Font font = Res::TimesNewRoman[12];
				font.draw(x_ + 1, y_ + 1, std::to_string(counter), KIR5::Color(0, 0, 0));
				font.draw(x_, y_, std::to_string(counter), KIR5::Color(255, 255, 255));
			}
		}));
	}
	ControlPanel::ControlPanel()
	{
		KIR5::Shared<KIR5::Column<>> container;
		container->show();
		container->setGap(5);
		// UI ============= ROW 1
		{
			titleOfBluePrint_Label->setText("Title: ");
			titleOfBluePrint_Label->alignWidthToText();

			titleOfBluePrint_TextBox->setText("This is the title of the map");
			titleOfBluePrint_TextBox->alignWidthToText();

			sizeOfCamera_Label->setText("Camera: ");
			sizeOfCamera_Label->alignWidthToText();

			sizeOfCamera_TextBox->setText("10.000*10.000");
			sizeOfCamera_TextBox->alignWidthToText();

			murphySpeed_Label->setText("Murphy: ");
			murphySpeed_Label->alignWidthToText();

			murphySpeed_TextBox->setText("10.0000");
			murphySpeed_TextBox->alignWidthToText();

			saveBluePrint_Button->setText("save");
			saveBluePrint_Button->width(saveBluePrint_Button->getTextWidth() + 10);
			saveBluePrint_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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

			KIR5::Shared<KIR5::Row<>> row;
			row->show();
			row->setGap(20);
			row->pushBack(titleOfBluePrint_Label);
			row->pushBack(titleOfBluePrint_TextBox);
			row->pushBack(sizeOfCamera_Label);
			row->pushBack(sizeOfCamera_TextBox);
			row->pushBack(murphySpeed_Label);
			row->pushBack(murphySpeed_TextBox);
			row->pushBack(saveBluePrint_Button);
			container->pushBack(row);
		}
		// UI ============= ROW 2
		{
			KIR5::Shared<KIR5::Row<>> containerLine;
			containerLine->show();
			containerLine->setGap(20);
			// ---------------- COL 1
			{
				rotation_Label->setText("Rotation");
				rotation_Label->width(rotation_Label->getTextWidth() + 10);

				for (size_t i = 0; i < rotation_Navigation_Buttons.size(); ++i)
				{
					rotation_Navigation_Buttons[i]->setBitmap(Res::uielements[Res::UIElements::Stick[i]]);
					rotation_Navigation_Buttons[i]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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

				rotation_Apply_Button->setBitmap(Res::uielements[Res::UIElements::Execute]);
				rotation_Apply_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					Type::Rotation r =
						rotation_Navigation_Buttons[0]->isSelected ? Type::Rotations::Up :
						rotation_Navigation_Buttons[1]->isSelected ? Type::Rotations::Right :
						rotation_Navigation_Buttons[2]->isSelected ? Type::Rotations::Down :
						Type::Rotations::Left;

					if (mainEvent->activeMap->isOperationModeAll())
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.selected)
							{
								block.object->rotation = r;
								block.Redrawn = true;
							}
						});
					}
					else
					{
						reach(map)[mainEvent->activeMap->getTarget()].object->rotation = r;
						reach(map)[mainEvent->activeMap->getTarget()].Redrawn = true;
					}
					mainEvent->activeMap->blocksUpdated();
				});

				KIR5::Shared<KIR5::Column<UIwindow<UI_M>>> localContent;
				localContent->show();
				localContent->setGap(UI_M::gap);
				localContent->pushBack(rotation_Label);

				KIR5::Shared<KIR5::Row<>> rows[3];
				for (auto &row : rows)
				{
					row->show();
					row->setGap(UI_M::gap);
					localContent->pushBack(row);
				}

				rows[0]->pushBack(KIR5::Shared<UIpanel<UI_M>>());
				rows[0]->pushBack(rotation_Navigation_Buttons[0]);
				rows[0]->pushBack(KIR5::Shared<UIpanel<UI_M>>());

				rows[1]->pushBack(rotation_Navigation_Buttons[3]);
				rows[1]->pushBack(rotation_Apply_Button);
				rows[1]->pushBack(rotation_Navigation_Buttons[1]);

				rows[2]->pushBack(KIR5::Shared<UIpanel<UI_M>>());
				rows[2]->pushBack(rotation_Navigation_Buttons[2]);
				rows[2]->pushBack(KIR5::Shared<UIpanel<UI_M>>());

				containerLine->pushBack(localContent);
			}
			// ---------------- COL 2
			{
				bluePrintResize_Label->setText("Resize");
				bluePrintResize_Label->width(bluePrintResize_Label->getTextWidth() + 10);

				bluePrintResize_Size_TextBox->width(UI_M::dimension * 2 + UI_M::gap);
				bluePrintResize_Size_TextBox->setTextAlignment(KIR5::CENTER);

				bluePrintResize_Apply_Button->setBitmap(Res::uielements[Res::UIElements::Execute]);
				bluePrintResize_Apply_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					int w, h;
					if (sscanf_s(bluePrintResize_Size_TextBox->getText().c_str(), "%d*%d", &w, &h) == 2 && w >= 2 && h >= 2)
					{
						Matrix<ActiveBlock<EditorObjectBase>> tmpMap(std::move(*(map.get())));
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
							xShift = (((Type::Size)(tmpMap)).width - w) / 2;
						}
						if (bluePrintResize_Navigation_Buttons[2][0]->isSelected || bluePrintResize_Navigation_Buttons[2][1]->isSelected || bluePrintResize_Navigation_Buttons[2][2]->isSelected)
						{
							xShift = ((Type::Size)(tmpMap)).width - w;
						}

						//Y
						if (bluePrintResize_Navigation_Buttons[0][0]->isSelected || bluePrintResize_Navigation_Buttons[1][0]->isSelected || bluePrintResize_Navigation_Buttons[2][0]->isSelected)
						{
							yShift = 0;
						}
						if (bluePrintResize_Navigation_Buttons[0][1]->isSelected || bluePrintResize_Navigation_Buttons[1][1]->isSelected || bluePrintResize_Navigation_Buttons[2][1]->isSelected)
						{
							yShift = (((Type::Size)(tmpMap)).height - h) / 2;
						}
						if (bluePrintResize_Navigation_Buttons[0][2]->isSelected || bluePrintResize_Navigation_Buttons[1][2]->isSelected || bluePrintResize_Navigation_Buttons[2][2]->isSelected)
						{
							yShift = ((Type::Size)(tmpMap)).height - h;
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

						Type::Coord coord = {0};
						for (coord.x = 0; coord.x < (std::min)(((Type::Size)(*map)).width, ((Type::Size)(tmpMap)).width - xShift); ++coord.x)
						{
							for (coord.y = 0; coord.y < (std::min)(((Type::Size)(*map)).height, ((Type::Size)(tmpMap)).height - yShift); ++coord.y)
							{
								(*map)[{coord.x + sxShift, coord.y + syShift}] = tmpMap[{coord.x + xShift, coord.y + yShift}];
								tmpMap[{coord.x + xShift, coord.y + yShift}].object = nullptr;
								tmpMap[{coord.x + xShift, coord.y + yShift}].remain = nullptr;
							}
						}

						mainEvent->activeMap->mapLayoutUpdated();
					}
					else
					{
						bluePrintResize_Size_TextBox->setText(std::to_string(((Type::Size)(*map)).width) + "*" + std::to_string(((Type::Size)(*map)).height));
					}
				});

				bluePrintResize_Navigation_Buttons[0][0]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[0][0]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(Res::uielements[Res::UIElements::ArrowRight]);
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownRight]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(KIR5::Bitmap());
				});

				bluePrintResize_Navigation_Buttons[1][0]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[1][0]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(Res::uielements[Res::UIElements::ArrowLeft]);
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(Res::uielements[Res::UIElements::Ball]);;
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(Res::uielements[Res::UIElements::ArrowRight]);
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownLeft]);
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownRight]);
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(KIR5::Bitmap());
				});

				bluePrintResize_Navigation_Buttons[2][0]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[2][0]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(Res::uielements[Res::UIElements::ArrowLeft]);
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownLeft]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(KIR5::Bitmap());
				});

				bluePrintResize_Navigation_Buttons[0][1]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[0][1]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpRight]);
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowRight]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownRight]);
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(KIR5::Bitmap());
				});

				bluePrintResize_Navigation_Buttons[1][1]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[1][1]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpLeft]);
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpRight]);
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(Res::uielements[Res::UIElements::ArrowLeft]);
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(Res::uielements[Res::UIElements::ArrowRight]);
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownLeft]);
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownRight]);
				});

				bluePrintResize_Navigation_Buttons[2][1]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[2][1]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpLeft]);
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowLeft]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedDownLeft]);
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
				});

				bluePrintResize_Navigation_Buttons[0][2]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[0][2]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpRight]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(Res::uielements[Res::UIElements::ArrowRight]);
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(KIR5::Bitmap());
				});

				bluePrintResize_Navigation_Buttons[1][2]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[1][2]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpLeft]);
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpRight]);
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(Res::uielements[Res::UIElements::ArrowLeft]);
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(Res::uielements[Res::UIElements::Ball]);
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(Res::uielements[Res::UIElements::ArrowRight]);
				});

				bluePrintResize_Navigation_Buttons[2][2]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int x = 0; x < 3; ++x)
					{
						for (int y = 0; y < 3; ++y)
						{
							bluePrintResize_Navigation_Buttons[x][y]->isSelected = false;
						}
					}

					bluePrintResize_Navigation_Buttons[2][2]->isSelected = true;

					bluePrintResize_Navigation_Buttons[0][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[2][0]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[0][1]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][1]->setBitmap(Res::uielements[Res::UIElements::ArrowTiltedUpLeft]);
					bluePrintResize_Navigation_Buttons[2][1]->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
					bluePrintResize_Navigation_Buttons[0][2]->setBitmap(KIR5::Bitmap());
					bluePrintResize_Navigation_Buttons[1][2]->setBitmap(Res::uielements[Res::UIElements::ArrowLeft]);
					bluePrintResize_Navigation_Buttons[2][2]->setBitmap(Res::uielements[Res::UIElements::Ball]);
				});

				KIR5::Event::EventCallback(bluePrintResize_Navigation_Buttons[1][1]->fncPress, bluePrintResize_Navigation_Buttons[1][1].get(), 0, 0);


				KIR5::Shared<KIR5::Column<UIwindow<UI_M>>> localContent;
				localContent->show();
				localContent->setGap(UI_M::gap);

				localContent->pushBack(bluePrintResize_Label);

				KIR5::Shared<KIR5::Row<>> row;
				row->show();
				row->setGap(UI_M::gap);
				row->pushBack(bluePrintResize_Size_TextBox);
				row->pushBack(bluePrintResize_Apply_Button);

				localContent->pushBack(row);

				for (size_t y = 0; y < bluePrintResize_Navigation_Buttons.size(); ++y)
				{
					KIR5::Shared<KIR5::Row<>> row;
					row->show();
					row->setGap(UI_M::gap);
					for (size_t x = 0; x < bluePrintResize_Navigation_Buttons[y].size(); ++x)
					{
						row->pushBack(bluePrintResize_Navigation_Buttons[x][y]);
					}
					localContent->pushBack(row);
				}

				containerLine->pushBack(localContent);
			}
			// ---------------- COL 3
			{
				object_Label->setText("Object");
				object_Label->width(object_Label->getTextWidth() + 10);

				object_Panel->fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
				{
					if (object_Panel->onPanel(x_, y_))
					{
						if (button_ == 1)
						{
							if (object_Panel->id == GetObjectNumber() - 1)
							{
								object_Panel->id = 0;
							}
							else
							{
								object_Panel->id++;
							}
							object_ID_TextBox->setText(std::to_string(object_Panel->id));
							updateBlocks();
							return true;
						}
						else if (button_ == 2)
						{
							if (object_Panel->id == 0)
							{
								object_Panel->id = GetObjectNumber() - 1;
							}
							else
							{
								object_Panel->id--;
							}
							object_ID_TextBox->setText(std::to_string(object_Panel->id));
							updateBlocks();
							return true;
						}
					}
					return false;
				});

				object_ID_TextBox->width(UI_M::dimension * 2 + UI_M::gap);
				object_ID_TextBox->setText(std::to_string(object_Panel->id));
				object_ID_TextBox->setTextAlignment(KIR5::CENTER);
				object_ID_TextBox->fncGetFocus.push_back([&](FNC_GET_FOCUS_PARAMS)->FNC_GET_FOCUS_RET
				{
					object_ID_TextBox->setText("");
				});

				object_ID_TextBox->fncLossFocus.push_back([&](FNC_LOSS_FOCUS_PARAMS)->FNC_LOSS_FOCUS_RET
				{
					object_ID_TextBox->setText(std::to_string(object_Panel->id));
				});

				object_ID_TextBox->fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
				{
					if (object_ID_TextBox->isActiveBox())
					{
						if (key_ == ALLEGRO_KEY_ENTER)
						{
							object_Panel->id = limiter<int>(0, GetObjectNumber() - 1, std::atoi(object_ID_TextBox->getText().c_str()));
							eventEngine->eventDestroyed(object_ID_TextBox.get());
							return true;
						}
					}
					return false;
				});

				object_Apply_Button->setBitmap(Res::uielements[Res::UIElements::Execute]);
				object_Apply_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (mainEvent->activeMap->isOperationModeAll())
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.selected)
							{
								ObjectCreate(block.object, object_Panel->id, coord);
								block.Redrawn = true;
							}
						});
					}
					else
					{
						ObjectCreate(reach(map)[mainEvent->activeMap->getTarget()].object, object_Panel->id, mainEvent->activeMap->getTarget());
						reach(map)[mainEvent->activeMap->getTarget()].Redrawn = true;
					}
					mainEvent->activeMap->blocksUpdated();

					int shiftFrom = prevBlockPickers.size() - 1;
					for (size_t i = 0; i < prevBlockPickers.size(); ++i)
					{
						if (prevBlockPickers[i]->id == object_Panel->id)
						{
							shiftFrom = i;
							break;
						}
					}
					for (int i = shiftFrom; i > 0; --i)
					{
						prevBlockPickers[i]->id = prevBlockPickers[i - 1]->id;
					}
					prevBlockPickers[0]->id = object_Panel->id;
				});

				object_RandomFill_Button->setText("Random fill");
				object_RandomFill_Button->width(object_RandomFill_Button->getTextWidth() + 10);
				object_RandomFill_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					float chancef = std::atof(object_Rate_TextBox->getText().c_str());
					int chance = (int)(chancef * 10);
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (block.selected && rand() % 1000 < chance)
						{
							ObjectCreate(block.object, object_Panel->id, coord);
							block.Redrawn = true;
						}
					});
					mainEvent->activeMap->blocksUpdated();
				});

				object_Rate_TextBox->width(UI_M::dimension * 2 + UI_M::gap);
				object_Rate_TextBox->setText("100.0");
				object_Rate_TextBox->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER);

				object_RandomFill_Label->setText("%");
				object_RandomFill_Label->width(bluePrintResize_Label->getTextWidth() + 5);

				object_FillFrame_Button->setText("Fill frame");
				object_FillFrame_Button->width(120);
				object_FillFrame_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (coord.x == 0 || coord.y == 0 || coord.x == ((Type::Size)reach(map)).width - 1 || coord.y == ((Type::Size)reach(map)).height - 1)
						{
							ObjectCreate(block.object, object_Panel->id, coord);
							block.Redrawn = true;
						}
					});
					mainEvent->activeMap->blocksUpdated();
				});

				object_FillContent_Button->setText("Fill content");
				object_FillContent_Button->width(120);
				object_FillContent_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (coord.x != 0 && coord.y != 0 && coord.x != ((Type::Size)reach(map)).width - 1 && coord.y != ((Type::Size)reach(map)).height - 1)
						{
							ObjectCreate(block.object, object_Panel->id, coord);
							block.Redrawn = true;
						}
					});
					mainEvent->activeMap->blocksUpdated();
				});

				object_selectAllOfThisType_Button->setText("Select all");
				object_selectAllOfThisType_Button->width(120);
				object_selectAllOfThisType_Button->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (block.object->id == object_Panel->id)
						{
							if (!block.selected)
							{
								block.selected = true;
								block.Redrawn = true;
							}
						}
						else
						{
							if (block.selected)
							{
								block.selected = false;
								block.Redrawn = true;
							}
						}
					});
					mainEvent->activeMap->blocksUpdated();
				});


				KIR5::Shared<KIR5::Column<UIwindow<UI_M>>> localContent;
				localContent->show();
				localContent->setGap(UI_M::gap);

				localContent->pushBack(object_Label);

				{
					KIR5::Shared<KIR5::Row<>> row;
					row->show();
					row->setGap(UI_M::gap);
					row->pushBack(object_Panel);
					row->pushBack(object_ID_TextBox);
					row->pushBack(object_Apply_Button);
					localContent->pushBack(row);
				}

				{
					KIR5::Shared<KIR5::Row<>> row;
					row->show();
					row->setGap(UI_M::gap);
					row->pushBack(object_RandomFill_Button);
					row->pushBack(object_Rate_TextBox);
					row->pushBack(object_RandomFill_Label);
					localContent->pushBack(row);
				}
				localContent->pushBack(object_FillFrame_Button);
				localContent->pushBack(object_FillContent_Button);
				localContent->pushBack(object_selectAllOfThisType_Button);

				containerLine->pushBack(localContent);
			}
			// ---------------- COL 4
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

				infotron_Label->setText("Infotrons");
				infotron_Label->width(infotron_Label->getTextWidth() + 10);

				infotron_ToCollect_TextBox->width(UI_M::dimension * 2 + UI_M::gap);
				infotron_ToCollect_TextBox->setTextFont(Res::TimesNewRoman[24]);
				infotron_ToCollect_TextBox->setTextColor(KIR5::Color(152, 152, 152));
				infotron_ToCollect_TextBox->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER);

				infotron_Capacity_Label->setText("/ 00000");
				infotron_Capacity_Label->width(infotron_Capacity_Label->getTextWidth() + 5);
				infotron_Capacity_Label->setText("/ 0");
				infotron_Capacity_Label->setTextColor(KIR5::Color(182, 122, 202));

				{
					infotron_Pickers_Buttons.resize(infotrons.size());
					for (size_t i = 0; i < infotrons.size(); ++i)
					{
						infotron_Pickers_Buttons[i]->id = infotrons[i].id;
						infotron_Pickers_Buttons[i]->multiplier = infotrons[i].multiplier;
						infotron_Pickers_Buttons[i]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
						{
							object_Panel->id = dynamic_cast<BlockIDPanel *>(obj_)->id;
							object_ID_TextBox->setText(std::to_string(object_Panel->id));
						});
					}
				}


				KIR5::Shared<KIR5::Column<UIwindow<UI_M>>> localContent;
				localContent->show();
				localContent->setGap(UI_M::gap);

				localContent->pushBack(infotron_Label);

				{
					KIR5::Shared<KIR5::Row<>> row;
					row->show();
					row->setGap(UI_M::gap);
					row->pushBack(infotron_ToCollect_TextBox);
					row->pushBack(infotron_Capacity_Label);
					localContent->pushBack(row);
				}

				{
					KIR5::Shared<KIR5::Row<>> row;
					row->show();
					row->setGap(UI_M::gap);
					for (auto &it : infotron_Pickers_Buttons)
					{
						row->pushBack(it);
					}
					localContent->pushBack(row);
				}

				containerLine->pushBack(localContent);
			}
			// ---------------- COL 5
			{
				selectAllButton->setText("Select all");
				selectAllButton->width(selectAllButton->getTextWidth());
				selectAllButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					bool isNonSelected = false;
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (!block.selected)
						{
							isNonSelected = true;
						}
					});

					if (isNonSelected)
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (!block.selected)
							{
								block.selected = true;
								block.Redrawn = true;
							}
						});
					}
					else
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.selected)
							{
								block.selected = false;
								block.Redrawn = true;
							}
						});
					}
				});

				gravityTurnOnOffButton->setText("Set gravity");
				gravityTurnOnOffButton->width(gravityTurnOnOffButton->getTextWidth());
				gravityTurnOnOffButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (!mainEvent->activeMap->isOperationModeAll())
					{
						if (reach(map)[mainEvent->activeMap->getTarget()].grid & GridFlags::Gravity)
						{
							reach(map)[mainEvent->activeMap->getTarget()].grid &= ~GridFlags::Gravity;
						}
						else
						{
							reach(map)[mainEvent->activeMap->getTarget()].grid |= GridFlags::Gravity;
						}
						reach(map)[mainEvent->activeMap->getTarget()].Redrawn = true;
					}
					else
					{
						bool isNew = false;
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
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
							map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
							{
								if (block.selected)
								{
									if (!(block.grid & GridFlags::Gravity))
									{
										block.grid |= GridFlags::Gravity;
										block.Redrawn = true;
									}
								}
							});
						}
						else
						{
							map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
							{
								if (block.selected)
								{
									if (block.grid & GridFlags::Gravity)
									{
										block.grid &= ~GridFlags::Gravity;
										block.Redrawn = true;
									}
								}
							});
						}
					}
					mainEvent->activeMap->blocksUpdated();
				});

				initExplodeButton->setText("Set init explode");
				initExplodeButton->width(initExplodeButton->getTextWidth());
				initExplodeButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (!mainEvent->activeMap->isOperationModeAll())
					{
						if (reach(map)[mainEvent->activeMap->getTarget()].grid & GridFlags::Detonate)
						{
							reach(map)[mainEvent->activeMap->getTarget()].grid &= ~GridFlags::Detonate;
						}
						else
						{
							reach(map)[mainEvent->activeMap->getTarget()].grid |= GridFlags::Detonate;
						}
						reach(map)[mainEvent->activeMap->getTarget()].Redrawn = true;
					}
					else
					{
						bool isNew = false;
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
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
							map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
							{
								if (block.selected)
								{
									if (!(block.grid & GridFlags::Detonate))
									{
										block.grid |= GridFlags::Detonate;
										block.Redrawn = true;
									}
								}
							});
						}
						else
						{
							map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
							{
								if (block.selected)
								{
									if (block.grid & GridFlags::Detonate)
									{
										block.grid &= ~GridFlags::Detonate;
										block.Redrawn = true;
									}
								}
							});
						}
					}
					mainEvent->activeMap->blocksUpdated();
				});

				selectFriendlyButton->setText("Select neighbors");
				selectFriendlyButton->width(selectFriendlyButton->getTextWidth());
				selectFriendlyButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (block.selected)
						{
							block.selected = false;
							block.Redrawn = true;
						}
					});

					reach(map)[mainEvent->activeMap->getTarget()].selected = true;
					Type::ID id = reach(map)[mainEvent->activeMap->getTarget()].object->id;

					bool isNewSelected = false;
					do
					{
						isNewSelected = false;
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (!block.selected && block.object->id == id &&
								(
									(map->Test({coord.x - 1, coord.y - 1}) && reach(map)[{coord.x - 1, coord.y - 1}].selected) ||
									(map->Test({coord.x - 0, coord.y - 1}) && reach(map)[{coord.x - 0, coord.y - 1}].selected) ||
									(map->Test({coord.x + 1, coord.y - 1}) && reach(map)[{coord.x + 1, coord.y - 1}].selected) ||

									(map->Test({coord.x - 1, coord.y - 0}) && reach(map)[{coord.x - 1, coord.y - 0}].selected) ||
									(map->Test({coord.x + 1, coord.y - 0}) && reach(map)[{coord.x + 1, coord.y - 0}].selected) ||

									(map->Test({coord.x - 1, coord.y + 1}) && reach(map)[{coord.x - 1, coord.y + 1}].selected) ||
									(map->Test({coord.x - 0, coord.y + 1}) && reach(map)[{coord.x - 0, coord.y + 1}].selected) ||
									(map->Test({coord.x + 1, coord.y + 1}) && reach(map)[{coord.x + 1, coord.y + 1}].selected)
									)
								)
							{
								block.selected = true;
								block.Redrawn = true;
								isNewSelected = true;
							}
						});
					}
					while (isNewSelected);

					mainEvent->activeMap->blocksUpdated();
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

				ramRepairButton->setText("Repair RAMs");
				ramRepairButton->width(ramRepairButton->getTextWidth());
				ramRepairButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (mainEvent->activeMap->isOperationModeAll())
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.selected)
							{
								if (block.object->id == ObjectID::RAMChipsRight)
								{
									if (map->Test({coord.x - 1,coord.y}) && reach(map)[{coord.x - 1, coord.y}].object->id != ObjectID::RAMChipsLeft)
									{
										while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
										{
											ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
											block.Redrawn = true;
										}
									}
								}
								if (block.object->id == ObjectID::RAMChipsBottom)
								{
									if (map->Test({coord.x,coord.y - 1}) && reach(map)[{coord.x, coord.y - 1}].object->id != ObjectID::RAMChipsTop)
									{
										while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
										{
											ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
											block.Redrawn = true;
										}
									}
								}

								if (block.object->id == ObjectID::RAMChipsLeft)
								{
									if (map->Test({coord.x + 1,coord.y}) && reach(map)[{coord.x + 1, coord.y}].selected &&isRam(reach(map)[{coord.x + 1, coord.y}].object->id))
									{
										ObjectCreate(reach(map)[{coord.x + 1, coord.y}].object, ObjectID::RAMChipsRight, {coord.x + 1,coord.y});
										reach(map)[{coord.x + 1, coord.y}].Redrawn = true;
									}
									else
									{
										ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
										block.Redrawn = true;
									}
								}
								if (block.object->id == ObjectID::RAMChipsTop)
								{
									if (map->Test({coord.x,coord.y + 1}) && reach(map)[{coord.x, coord.y + 1}].selected &&isRam(reach(map)[{coord.x, coord.y + 1}].object->id))
									{
										ObjectCreate(reach(map)[{coord.x, coord.y + 1}].object, ObjectID::RAMChipsBottom, {coord.x,coord.y + 1});
										reach(map)[{coord.x, coord.y + 1}].Redrawn = true;
									}
									else
									{
										ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
										block.Redrawn = true;
									}
								}
							}
						});
					}
					else
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.object->id == ObjectID::RAMChipsRight)
							{
								if (map->Test({coord.x - 1,coord.y}) && reach(map)[{coord.x - 1, coord.y}].object->id != ObjectID::RAMChipsLeft)
								{
									while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
									{
										ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
										block.Redrawn = true;
									}
								}
							}
							if (block.object->id == ObjectID::RAMChipsBottom)
							{
								if (map->Test({coord.x,coord.y - 1}) && reach(map)[{coord.x, coord.y - 1}].object->id != ObjectID::RAMChipsTop)
								{
									while (block.object->id == ObjectID::RAMChipsBottom || block.object->id == ObjectID::RAMChipsRight)
									{
										ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
										block.Redrawn = true;
									}
								}
							}

							if (block.object->id == ObjectID::RAMChipsLeft)
							{
								if (map->Test({coord.x + 1,coord.y}) && isRam(reach(map)[{coord.x + 1, coord.y}].object->id))
								{
									ObjectCreate(reach(map)[{coord.x + 1, coord.y}].object, ObjectID::RAMChipsRight, {coord.x + 1,coord.y});
									reach(map)[{coord.x + 1, coord.y}].Redrawn = true;
								}
								else
								{
									ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
									block.Redrawn = true;
								}
							}
							if (block.object->id == ObjectID::RAMChipsTop)
							{
								if (map->Test({coord.x,coord.y + 1}) && isRam(reach(map)[{coord.x, coord.y + 1}].object->id))
								{
									ObjectCreate(reach(map)[{coord.x, coord.y + 1}].object, ObjectID::RAMChipsBottom, {coord.x,coord.y + 1});
									reach(map)[{coord.x, coord.y + 1}].Redrawn = true;
								}
								else
								{
									ObjectCreate(block.object, ObjectID::RAMChipsMini, coord);
									block.Redrawn = true;
								}
							}
						});
					}
					mainEvent->activeMap->blocksUpdated();
				});

				randomizeRamsButton->setText("Randomize RAMs");
				randomizeRamsButton->width(randomizeRamsButton->getTextWidth());
				randomizeRamsButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (mainEvent->activeMap->isOperationModeAll())
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.selected && isRam(block.object->id))
							{
								ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
								block.Redrawn = true;
							}
						});
					}
					else
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (isRam(block.object->id))
							{
								ObjectCreate(block.object, ramIDs[rand() % NUMBER_OF_RAMS], coord);
								block.Redrawn = true;
							}
						});
					}
					mainEvent->activeMap->blocksUpdated();
				});

				spawnPointButton->setText("Set SPAWN");
				spawnPointButton->width(spawnPointButton->getTextWidth());
				spawnPointButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (!mainEvent->activeMap->isOperationModeAll())
					{
						if (reach(map)[mainEvent->activeMap->getTarget()].grid & GridFlags::SpawnPoint)
						{
							reach(map)[mainEvent->activeMap->getTarget()].grid &= ~GridFlags::SpawnPoint;
						}
						else
						{
							reach(map)[mainEvent->activeMap->getTarget()].grid |= GridFlags::SpawnPoint;
						}
						reach(map)[mainEvent->activeMap->getTarget()].Redrawn = true;
					}
					else
					{
						bool isNew = false;
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
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
							map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
							{
								if (block.selected)
								{
									if (!(block.grid & GridFlags::SpawnPoint))
									{
										block.grid |= GridFlags::SpawnPoint;
										block.Redrawn = true;
									}
								}
							});
						}
						else
						{
							map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
							{
								if (block.selected)
								{
									if (block.grid & GridFlags::SpawnPoint)
									{
										block.grid &= ~GridFlags::SpawnPoint;
										block.Redrawn = true;
									}
								}
							});
						}
					}
					mainEvent->activeMap->blocksUpdated();
				});

				globalGravityOnOffButton->setText(GLOBAL_GRAVITY_OFF);
				globalGravityOnOffButton->width(globalGravityOnOffButton->getTextWidth());
				globalGravityOnOffButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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

				KIR5::Shared<KIR5::Column<UIwindow<UI_M>>> localContent;
				localContent->show();
				localContent->setGap(UI_S::gap);
				localContent->addFlag(KIR5::Column<UIwindow<UI_M>>::ConsistentWidth);

				localContent->pushBack(selectAllButton);
				localContent->pushBack(gravityTurnOnOffButton);
				localContent->pushBack(initExplodeButton);
				localContent->pushBack(selectFriendlyButton);
				localContent->pushBack(ramRepairButton);
				localContent->pushBack(randomizeRamsButton);
				localContent->pushBack(spawnPointButton);
				localContent->pushBack(globalGravityOnOffButton);


				containerLine->pushBack(localContent);
			}
			container->pushBack(containerLine);
		}
		// UI ============= ROW 3
		{
			for (size_t i = 0; i < prevBlockPickers.size(); ++i)
			{
				prevBlockPickers[i]->id = (Type::ID)i;
				prevBlockPickers[i]->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					object_Panel->id = dynamic_cast<BlockIDPanel *>(obj_)->id;
					object_ID_TextBox->setText(std::to_string(object_Panel->id));
				});
			}


			KIR5::Shared<KIR5::Row<UIwindow<UI_M>>> localContent;
			localContent->show();
			localContent->setGap(UI_S::gap);

			for (size_t i = 0; i < prevBlockPickers.size(); ++i)
			{
				localContent->pushBack(prevBlockPickers[i]);
			}

			container->pushBack(localContent);

		}
		// UI =============


		this->fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
		{
			if (
				eventEngine->getTergetPanel().get() == this ||
				eventEngine->getTergetPanel().get() == mainEvent->activeMap.get()
				)
			{
				if (key_ == ALLEGRO_KEY_ENTER)
				{
					KIR5::Event::EventCallback(object_Apply_Button->fncPress, object_Apply_Button.get(), 0, 0);
					return true;
				}
			}
			return false;
		});

		*this << container;
	}
	ControlPanel::~ControlPanel()
	{

	}


	void ControlPanel::setOperationMode()
	{
		KIR5::Color bcolor = mainEvent->activeMap->isOperationModeAll() ? KIR5::Color(78, 30, 30) : KIR5::Color(30, 30, 78);

		rotation_Apply_Button->setColor(bcolor);
		object_Apply_Button->setColor(bcolor);
		object_selectAllOfThisType_Button->setColor(bcolor);
		object_FillFrame_Button->setColor(bcolor);
		object_FillContent_Button->setColor(bcolor);
		gravityTurnOnOffButton->setColor(bcolor);
		initExplodeButton->setColor(bcolor);
		spawnPointButton->setColor(bcolor);

		randomizeRamsButton->setColor(bcolor);
		ramRepairButton->setColor(bcolor);
	}

	void ControlPanel::pickID(Type::ID id)
	{
		object_Panel->id = id;
		object_ID_TextBox->setText(std::to_string(object_Panel->id));
		updateBlocks();
	}

	void ControlPanel::updateBlocks()
	{
		object_Panel->counter = 0;
		for (auto &it : infotron_Pickers_Buttons)
		{
			it->counter = 0;
		}
		this->map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
		{
			if (block.object->id == object_Panel->id)
			{
				++object_Panel->counter;
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

	void ControlPanel::SetMap(std::shared_ptr<Res::BluePrint> &bluePrint_, std::shared_ptr<Matrix<ActiveBlock<EditorObjectBase>>> &map)
	{
		bluePrint = bluePrint_;
		this->map = map;
		infotron_ToCollect_TextBox->setText(std::to_string(bluePrint->scoreToUnlock));
		titleOfBluePrint_TextBox->setText(bluePrint->title);
		bluePrintResize_Size_TextBox->setText(std::to_string(((Type::Size)(*map)).width) + "*" + std::to_string(((Type::Size)(*map)).height));

		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << bluePrint->cameraSize.width << "*" << bluePrint->cameraSize.height;
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
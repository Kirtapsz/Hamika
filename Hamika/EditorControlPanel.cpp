#include "EditorControlPanel.h"
#include "Font.h"
#include "IDreg.h"
#include "Global.h"
#include "EditorMainEvent.h"
#include <KIR/AL/KIR5_panel_control.h>

namespace Editor
{
	ControlPanel::BlockIDPanel::BlockIDPanel()
	{
		this->fncDraw = [&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
		{
			DrawObject(id, x_, y_, w_, h_);
			if (counter >= 0)
			{
				Font::TimesNewRoman[12].draw(x_ + 1, y_ + 1, std::to_string(counter), KIR5::Color(0, 0, 0));
				Font::TimesNewRoman[12].draw(x_, y_, std::to_string(counter), KIR5::Color(255, 255, 255));
			}
		};
	}
	ControlPanel::ControlPanel()
	{
		constexpr int gap = 3;
		constexpr int buttonSize = 32;
		constexpr int textBoxSize = 32;
		constexpr int labelSize = 16;


		this->fncKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
		{
			if (
				eventEngine->getTergetPanel().get() == this ||
				eventEngine->getTergetPanel().get() == mainEvent->activeMap.get()
				)
			{
				if (key_ == ALLEGRO_KEY_ENTER)
				{
					fillButton->fncPress(fillButton.get(), 0, 0);
					return true;
				}
			}
			return false;
		};

		KIR5::EVENT<KIR5::Column<KIR5::Panel>> containerRows;
		KIR5::EVENT<KIR5::Row<KIR5::Panel>> toolColumns;
		toolColumns->setGap(gap);
		KIR5::EVENT<KIR5::Row<KIR5::Panel>> prevBlockColumns;
		prevBlockColumns->setGap(gap);

		arrowTilted.resize(4);
		arrowTilted[0].load("Hamika\\texture\\editor\\ArrowTilted.png");
		arrowTilted[1] = arrowTilted[0];
		arrowTilted[1].flipHorizontal();
		arrowTilted[2] = arrowTilted[1];
		arrowTilted[2].flipVertical();
		arrowTilted[3] = arrowTilted[0];
		arrowTilted[3].flipVertical();

		arrowNarrow.resize(4);
		arrowNarrow[0].load("Hamika\\texture\\editor\\ArrowUp.png");
		arrowNarrow[1] = arrowNarrow[0];
		arrowNarrow[1].rotate(ALLEGRO_PI / 2.f);
		arrowNarrow[2] = arrowNarrow[0];
		arrowNarrow[2].flipVertical();
		arrowNarrow[3] = arrowNarrow[1];
		arrowNarrow[3].flipHorizontal();

		stick.resize(4);
		stick[0].load("Hamika\\texture\\editor\\StickUp.png");
		stick[1] = stick[0];
		stick[1].rotate(ALLEGRO_PI / 2.f);
		stick[2] = stick[0];
		stick[2].flipVertical();
		stick[3] = stick[1];
		stick[3].flipHorizontal();

		ball.load("Hamika\\texture\\editor\\Ball.png");

		execute.load("Hamika\\texture\\editor\\Execute.png");

		{
			rotationContainer->show();
			rotationContainer->setGap(gap);

			{
				rotationLabel->setTextColor(KIR5::Color(50, 50, 50));
				rotationLabel->setTextFont(Font::TimesNewRoman[12]);
				rotationLabel->setText("Rotation");
				rotationLabel->move(0, 0, rotationLabel->width(), labelSize);
				rotationLabel->show();
				rotationLabel->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				rotationContainer->pushBack(rotationLabel);
			}

			rotationButtons.resize(4);
			for (int i = 0; i < 4; ++i)
			{
				rotationButtons[i]->resize(buttonSize, buttonSize);
				rotationButtons[i]->show();
				rotationButtons[i]->setColor(KIR5::Color(30, 30, 30));
				rotationButtons[i]->setBitmap(stick[i]);
				rotationButtons[i]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					for (int i = 0; i < 4; ++i)
					{
						rotationButtons[i]->setColor(KIR5::Color(30, 30, 30));
						rotationButtons[i]->isSelected = false;
					}

					dynamic_cast<ResizeButtonClass *>(obj_)->setColor(KIR5::Color(200, 60, 30));
					dynamic_cast<ResizeButtonClass *>(obj_)->isSelected = true;
				};
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				KIR5::EVENT<KIR5::Panel> tmp1;
				tmp1->resize(buttonSize, buttonSize);
				KIR5::EVENT<KIR5::Panel> tmp2;
				tmp2->resize(buttonSize, buttonSize);

				row->pushBack(tmp1);
				row->pushBack(rotationButtons[0]);
				row->pushBack(tmp2);

				rotationContainer->pushBack(row);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				row->pushBack(rotationButtons[3]);

				rotationExecute->move(1 * (buttonSize + gap) + gap, mapResizeLabel->virtualy2() + gap, buttonSize, buttonSize);
				rotationExecute->setBitmap(execute);
				rotationExecute->show();
				rotationExecute->setColor(KIR5::Color(30, 30, 30));
				row->pushBack(rotationExecute);
				rotationExecute->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					Type::Rotation r =
						rotationButtons[0]->isSelected ? Type::Rotations::Up :
						rotationButtons[1]->isSelected ? Type::Rotations::Right :
						rotationButtons[2]->isSelected ? Type::Rotations::Down :
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
				};

				row->pushBack(rotationButtons[1]);

				rotationContainer->pushBack(row);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				KIR5::EVENT<KIR5::Panel> tmp1;
				tmp1->resize(buttonSize, buttonSize);
				KIR5::EVENT<KIR5::Panel> tmp2;
				tmp2->resize(buttonSize, buttonSize);

				row->pushBack(tmp1);
				row->pushBack(rotationButtons[2]);
				row->pushBack(tmp2);

				rotationContainer->pushBack(row);
			}

			rotationButtons[0]->fncPress(rotationButtons[0].get(), 0, 0);

			rotationBackground->resize(rotationContainer->width(), rotationContainer->height());
			rotationBackground->setBackgroundColor(KIR5::Color(8, 8, 8));
			rotationBackground->show();
			*rotationBackground << rotationContainer;
			rotationContainer->position(0, 0);

			rotationFrame->resize(rotationContainer->width() + 4, rotationContainer->height() + 4);
			rotationFrame->setBackgroundColor(KIR5::Color(60, 60, 60));
			rotationFrame->show();
			*rotationFrame << rotationBackground;
			rotationBackground->position(2, 2);

			rotationFrame->position(0, 0);
			toolColumns->pushBack(rotationFrame);
		}

		{
			mapResizeContainer->show();
			mapResizeContainer->setGap(gap);

			{
				mapResizeLabel->setTextColor(KIR5::Color(50, 50, 50));
				mapResizeLabel->setTextFont(Font::TimesNewRoman[12]);
				mapResizeLabel->setText("Resize");
				mapResizeLabel->resize(mapResizeLabel->getTextWidth(), labelSize);
				mapResizeLabel->show();
				mapResizeLabel->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				mapResizeContainer->pushBack(mapResizeLabel);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				mapResizeTextBox->resize(buttonSize * 2 + gap, buttonSize);
				mapResizeTextBox->setTextFont(Font::TimesNewRoman[16]);
				mapResizeTextBox->setTextColor(KIR5::Color(152, 152, 152));
				mapResizeTextBox->show();
				mapResizeTextBox->setColor(KIR5::Color(30, 30, 30));
				row->pushBack(mapResizeTextBox);

				mapResizeExecute->resize(buttonSize, buttonSize);
				mapResizeExecute->show();
				mapResizeExecute->setBitmap(execute);
				mapResizeExecute->setColor(KIR5::Color(30, 30, 30));
				row->pushBack(mapResizeExecute);
				mapResizeExecute->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					int w, h;
					if (sscanf_s(mapResizeTextBox->getText().c_str(), "%d*%d", &w, &h) == 2 && w >= 2 && h >= 2)
					{
						Array2D<ActiveBlock<EditorObjectBase>> tmpMap(std::move(*(map.get())));
						map->resize({w,h});

						int xShift = 0;
						int yShift = 0;

						int sxShift = 0;
						int syShift = 0;

						//X
						if (resizeButtons[0][0]->isSelected || resizeButtons[0][1]->isSelected || resizeButtons[0][2]->isSelected)
						{
							xShift = 0;
						}
						if (resizeButtons[1][0]->isSelected || resizeButtons[1][1]->isSelected || resizeButtons[1][2]->isSelected)
						{
							xShift = (((Type::Size)(tmpMap)).width - w) / 2;
						}
						if (resizeButtons[2][0]->isSelected || resizeButtons[2][1]->isSelected || resizeButtons[2][2]->isSelected)
						{
							xShift = ((Type::Size)(tmpMap)).width - w;
						}

						//Y
						if (resizeButtons[0][0]->isSelected || resizeButtons[1][0]->isSelected || resizeButtons[2][0]->isSelected)
						{
							yShift = 0;
						}
						if (resizeButtons[0][1]->isSelected || resizeButtons[1][1]->isSelected || resizeButtons[2][1]->isSelected)
						{
							yShift = (((Type::Size)(tmpMap)).height - h) / 2;
						}
						if (resizeButtons[0][2]->isSelected || resizeButtons[1][2]->isSelected || resizeButtons[2][2]->isSelected)
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

						Type::Coord coord;
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
						mapResizeTextBox->setText(std::to_string(((Type::Size)(*map)).width) + "*" + std::to_string(((Type::Size)(*map)).height));
					}
				};

				mapResizeContainer->pushBack(row);
			}


			resizeButtons.resize(3);
			for (auto &it : resizeButtons)
			{
				it.resize(3);
			}

			for (int y = 0; y < 3; ++y)
			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);
				for (int x = 0; x < 3; ++x)
				{
					resizeButtons[x][y]->resize(buttonSize, buttonSize);
					resizeButtons[x][y]->show();
					resizeButtons[x][y]->setColor(KIR5::Color(30, 30, 30));
					row->pushBack(resizeButtons[x][y]);
				}
				mapResizeContainer->pushBack(row);
			}

			resizeButtons[0][0]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[0][0]->isSelected = true;

				resizeButtons[0][0]->setBitmap(ball);
				resizeButtons[1][0]->setBitmap(arrowNarrow[1]);
				resizeButtons[2][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][1]->setBitmap(arrowNarrow[2]);
				resizeButtons[1][1]->setBitmap(arrowTilted[2]);
				resizeButtons[2][1]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[2][2]->setBitmap(KIR5::Bitmap());
			};

			resizeButtons[1][0]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[1][0]->isSelected = true;

				resizeButtons[0][0]->setBitmap(arrowNarrow[3]);
				resizeButtons[1][0]->setBitmap(ball);
				resizeButtons[2][0]->setBitmap(arrowNarrow[1]);
				resizeButtons[0][1]->setBitmap(arrowTilted[3]);
				resizeButtons[1][1]->setBitmap(arrowNarrow[2]);
				resizeButtons[2][1]->setBitmap(arrowTilted[2]);
				resizeButtons[0][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[2][2]->setBitmap(KIR5::Bitmap());
			};

			resizeButtons[2][0]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[2][0]->isSelected = true;

				resizeButtons[0][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][0]->setBitmap(arrowNarrow[3]);
				resizeButtons[2][0]->setBitmap(ball);
				resizeButtons[0][1]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][1]->setBitmap(arrowTilted[3]);
				resizeButtons[2][1]->setBitmap(arrowNarrow[2]);
				resizeButtons[0][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[2][2]->setBitmap(KIR5::Bitmap());
			};

			resizeButtons[0][1]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[0][1]->isSelected = true;

				resizeButtons[0][0]->setBitmap(arrowNarrow[0]);
				resizeButtons[1][0]->setBitmap(arrowTilted[1]);
				resizeButtons[2][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][1]->setBitmap(ball);
				resizeButtons[1][1]->setBitmap(arrowNarrow[1]);
				resizeButtons[2][1]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][2]->setBitmap(arrowNarrow[2]);
				resizeButtons[1][2]->setBitmap(arrowTilted[2]);
				resizeButtons[2][2]->setBitmap(KIR5::Bitmap());
			};

			resizeButtons[1][1]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[1][1]->isSelected = true;

				resizeButtons[0][0]->setBitmap(arrowTilted[0]);
				resizeButtons[1][0]->setBitmap(arrowNarrow[0]);
				resizeButtons[2][0]->setBitmap(arrowTilted[1]);
				resizeButtons[0][1]->setBitmap(arrowNarrow[3]);
				resizeButtons[1][1]->setBitmap(ball);
				resizeButtons[2][1]->setBitmap(arrowNarrow[1]);
				resizeButtons[0][2]->setBitmap(arrowTilted[3]);
				resizeButtons[1][2]->setBitmap(arrowNarrow[2]);
				resizeButtons[2][2]->setBitmap(arrowTilted[2]);
			};

			resizeButtons[2][1]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[2][1]->isSelected = true;

				resizeButtons[0][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][0]->setBitmap(arrowTilted[0]);
				resizeButtons[2][0]->setBitmap(arrowNarrow[0]);
				resizeButtons[0][1]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][1]->setBitmap(arrowNarrow[3]);
				resizeButtons[2][1]->setBitmap(ball);
				resizeButtons[0][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][2]->setBitmap(arrowTilted[3]);
				resizeButtons[2][2]->setBitmap(arrowNarrow[2]);
			};

			resizeButtons[0][2]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[0][2]->isSelected = true;

				resizeButtons[0][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[2][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][1]->setBitmap(arrowNarrow[0]);
				resizeButtons[1][1]->setBitmap(arrowTilted[1]);
				resizeButtons[2][1]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][2]->setBitmap(ball);
				resizeButtons[1][2]->setBitmap(arrowNarrow[1]);
				resizeButtons[2][2]->setBitmap(KIR5::Bitmap());
			};

			resizeButtons[1][2]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[1][2]->isSelected = true;

				resizeButtons[0][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[2][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][1]->setBitmap(arrowTilted[0]);
				resizeButtons[1][1]->setBitmap(arrowNarrow[0]);
				resizeButtons[2][1]->setBitmap(arrowTilted[1]);
				resizeButtons[0][2]->setBitmap(arrowNarrow[3]);
				resizeButtons[1][2]->setBitmap(ball);
				resizeButtons[2][2]->setBitmap(arrowNarrow[1]);
			};

			resizeButtons[2][2]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						resizeButtons[x][y]->isSelected = false;
					}
				}

				resizeButtons[2][2]->isSelected = true;

				resizeButtons[0][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[2][0]->setBitmap(KIR5::Bitmap());
				resizeButtons[0][1]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][1]->setBitmap(arrowTilted[0]);
				resizeButtons[2][1]->setBitmap(arrowNarrow[0]);
				resizeButtons[0][2]->setBitmap(KIR5::Bitmap());
				resizeButtons[1][2]->setBitmap(arrowNarrow[3]);
				resizeButtons[2][2]->setBitmap(ball);
			};

			resizeButtons[1][1]->fncPress(resizeButtons[1][1].get(), 0, 0);

			mapResizeBackground->resize(mapResizeContainer->width(), mapResizeContainer->height());
			mapResizeBackground->setBackgroundColor(KIR5::Color(8, 8, 8));
			mapResizeBackground->show();
			*mapResizeBackground << mapResizeContainer;
			mapResizeContainer->position(0, 0);

			mapResizeFrame->resize(mapResizeContainer->width() + 4, mapResizeContainer->height() + 4);
			mapResizeFrame->setBackgroundColor(KIR5::Color(60, 60, 60));
			mapResizeFrame->show();
			*mapResizeFrame << mapResizeBackground;
			mapResizeBackground->position(2, 2);

			mapResizeFrame->position(200, 0);
			toolColumns->pushBack(mapResizeFrame);
		}

		{
			blockPickerContainer->show();
			blockPickerContainer->setGap(3);

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				objectPanel->show();
				objectPanel->resize(32, 32);

				objectPanel->fncMouseButtonDown = [&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
				{
					if (objectPanel->onPanel(x_, y_))
					{
						if (button_ == 1)
						{
							if (objectPanel->id == GetObjectNumber() - 1)
							{
								objectPanel->id = 0;
							}
							else
							{
								objectPanel->id++;
							}
							objectIDTextBox->setText(std::to_string(objectPanel->id));
							updateBlocks();
							return true;
						}
						else if (button_ == 2)
						{
							if (objectPanel->id == 0)
							{
								objectPanel->id = GetObjectNumber() - 1;
							}
							else
							{
								objectPanel->id--;
							}
							objectIDTextBox->setText(std::to_string(objectPanel->id));
							updateBlocks();
							return true;
						}
					}
					return false;
				};

				row->pushBack(objectPanel);

				objectIDTextBox->resize(64, 32);
				objectIDTextBox->setTextFont(Font::TimesNewRoman[24]);
				objectIDTextBox->setText(std::to_string(objectPanel->id));
				objectIDTextBox->setTextColor(KIR5::Color(152, 152, 152));
				objectIDTextBox->show();
				objectIDTextBox->setTextAlignment(KIR5::CENTER);
				objectIDTextBox->setColor(KIR5::Color(30, 30, 30));

				objectIDTextBoxGetFocus.lock(objectIDTextBox->fncGetFocus);
				objectIDTextBoxGetFocus.set([&](FNC_GET_FOCUS_PARAMS)->FNC_GET_FOCUS_RET
				{
					objectIDTextBox->setText("");
				});

				objectIDTextBoxLossFocus.lock(objectIDTextBox->fncLossFocus);
				objectIDTextBoxLossFocus.set([&](FNC_LOSS_FOCUS_PARAMS)->FNC_LOSS_FOCUS_RET
				{
					objectIDTextBox->setText(std::to_string(objectPanel->id));
				});

				objectIDTextBox->fncKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
				{
					if (objectIDTextBox->isActiveBox())
					{
						if (key_ == ALLEGRO_KEY_ENTER)
						{
							objectPanel->id = limiter<int>(0, GetObjectNumber() - 1, std::atoi(objectIDTextBox->getText().c_str()));
							eventEngine->eventDestroyed(objectIDTextBox.get());
							return true;
						}
					}
					return false;
				};

				row->pushBack(objectIDTextBox);

				fillButton->resize(buttonSize, buttonSize);
				fillButton->show();
				fillButton->setBitmap(execute);
				fillButton->setColor(KIR5::Color(30, 30, 30));
				row->pushBack(fillButton);
				fillButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					if (mainEvent->activeMap->isOperationModeAll())
					{
						map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
						{
							if (block.selected)
							{
								ObjectCreate(block.object, objectPanel->id, coord);
								block.Redrawn = true;
							}
						});
					}
					else
					{
						ObjectCreate(reach(map)[mainEvent->activeMap->getTarget()].object, objectPanel->id, mainEvent->activeMap->getTarget());
						reach(map)[mainEvent->activeMap->getTarget()].Redrawn = true;
					}
					mainEvent->activeMap->blocksUpdated();

					int shiftFrom = prevBlockPickers.size() - 1;
					for (size_t i = 0; i < prevBlockPickers.size(); ++i)
					{
						if (prevBlockPickers[i]->id == objectPanel->id)
						{
							shiftFrom = i;
							break;
						}
					}
					for (int i = shiftFrom; i > 0; --i)
					{
						prevBlockPickers[i]->id = prevBlockPickers[i - 1]->id;
					}
					prevBlockPickers[0]->id = objectPanel->id;
				};

				blockPickerContainer->pushBack(row);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				randomFillButton->resize(120, 32);
				randomFillButton->setTextFont(Font::TimesNewRoman[24]);
				randomFillButton->setText("Random fill");
				randomFillButton->setTextColor(KIR5::Color(152, 152, 152));
				randomFillButton->show();
				randomFillButton->setTextAlignment(KIR5::CENTER);
				randomFillButton->setColor(KIR5::Color(30, 30, 30));
				row->pushBack(randomFillButton);
				randomFillButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					float chancef = std::atof(randomFillTextBox->getText().c_str());
					int chance = (int)(chancef * 10);
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (block.selected && rand() % 1000 < chance)
						{
							ObjectCreate(block.object, objectPanel->id, coord);
							block.Redrawn = true;
						}
					});
					mainEvent->activeMap->blocksUpdated();
				};

				randomFillTextBox->resize(64, 32);
				randomFillTextBox->setTextFont(Font::TimesNewRoman[24]);
				randomFillTextBox->setText("100.0");
				randomFillTextBox->setTextColor(KIR5::Color(152, 152, 152));
				randomFillTextBox->show();
				randomFillTextBox->setTextAlignment(KIR5::RIGHT);
				randomFillTextBox->setColor(KIR5::Color(30, 30, 30));
				row->pushBack(randomFillTextBox);

				randomFillLabel->setTextColor(KIR5::Color(50, 50, 50));
				randomFillLabel->setTextFont(Font::TimesNewRoman[24]);
				randomFillLabel->setText("%");
				randomFillLabel->resize(mapResizeLabel->getTextWidth(), labelSize);
				randomFillLabel->show();
				randomFillLabel->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				row->pushBack(randomFillLabel);

				blockPickerContainer->pushBack(row);
			}

			{
				fillFrameButton->resize(120, 32);
				fillFrameButton->setTextFont(Font::TimesNewRoman[24]);
				fillFrameButton->setText("Fill frame");
				fillFrameButton->setTextColor(KIR5::Color(152, 152, 152));
				fillFrameButton->show();
				fillFrameButton->setTextAlignment(KIR5::CENTER);
				fillFrameButton->setColor(KIR5::Color(30, 30, 30));
				blockPickerContainer->pushBack(fillFrameButton);
				fillFrameButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (coord.x == 0 || coord.y == 0 || coord.x == ((Type::Size)reach(map)).width - 1 || coord.y == ((Type::Size)reach(map)).height - 1)
						{
							ObjectCreate(block.object, objectPanel->id, coord);
							block.Redrawn = true;
						}
					});
					mainEvent->activeMap->blocksUpdated();
				};
			}
			{
				fillContentButton->resize(120, 32);
				fillContentButton->setTextFont(Font::TimesNewRoman[24]);
				fillContentButton->setText("Fill content");
				fillContentButton->setTextColor(KIR5::Color(152, 152, 152));
				fillContentButton->show();
				fillContentButton->setTextAlignment(KIR5::CENTER);
				fillContentButton->setColor(KIR5::Color(30, 30, 30));
				blockPickerContainer->pushBack(fillContentButton);
				fillContentButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (coord.x != 0 && coord.y != 0 && coord.x != ((Type::Size)reach(map)).width - 1 && coord.y != ((Type::Size)reach(map)).height - 1)
						{
							ObjectCreate(block.object, objectPanel->id, coord);
							block.Redrawn = true;
						}
					});
					mainEvent->activeMap->blocksUpdated();
				};
			}
			{
				fillSelectAllButton->resize(120, 32);
				fillSelectAllButton->setTextFont(Font::TimesNewRoman[24]);
				fillSelectAllButton->setText("Select all");
				fillSelectAllButton->setTextColor(KIR5::Color(152, 152, 152));
				fillSelectAllButton->show();
				fillSelectAllButton->setTextAlignment(KIR5::CENTER);
				fillSelectAllButton->setColor(KIR5::Color(30, 30, 30));
				blockPickerContainer->pushBack(fillSelectAllButton);
				fillSelectAllButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
					{
						if (block.object->id == objectPanel->id)
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
				};
			}


			blockPickerBackground->resize(blockPickerContainer->width(), blockPickerContainer->height());
			blockPickerBackground->setBackgroundColor(KIR5::Color(8, 8, 8));
			blockPickerBackground->show();
			*blockPickerBackground << blockPickerContainer;
			blockPickerContainer->position(0, 0);

			blockPickerFrame->resize(blockPickerContainer->width() + 4, blockPickerContainer->height() + 4);
			blockPickerFrame->setBackgroundColor(KIR5::Color(60, 60, 60));
			blockPickerFrame->show();
			*blockPickerFrame << blockPickerBackground;
			blockPickerBackground->position(2, 2);

			blockPickerFrame->position(400, 0);
			toolColumns->pushBack(blockPickerFrame);
		}

		{
			int infotronMultipliers[] = {
				1,
				9,
			};

			Type::ID infotronIds[] = {
			   ObjectID::Infotron,
			   ObjectID::Electrons,
			};
			constexpr int numberOfinfotronIDs = (sizeof(infotronIds) / sizeof(Type::ID));

			infotronContainer->show();
			infotronContainer->setGap(3);

			{
				infotronLabel->setTextColor(KIR5::Color(50, 50, 50));
				infotronLabel->setTextFont(Font::TimesNewRoman[12]);
				infotronLabel->setText("Infotrons");
				infotronLabel->resize(infotronLabel->getTextWidth(), labelSize);
				infotronLabel->show();
				infotronLabel->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				infotronContainer->pushBack(infotronLabel);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				infotronToCollectTextBox->resize(numberOfinfotronIDs * buttonSize + (numberOfinfotronIDs - 1) * gap, textBoxSize);
				infotronToCollectTextBox->setTextFont(Font::TimesNewRoman[24]);
				infotronToCollectTextBox->setTextColor(KIR5::Color(152, 152, 152));
				infotronToCollectTextBox->show();
				infotronToCollectTextBox->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER);
				infotronToCollectTextBox->setColor(KIR5::Color(5, 5, 5));
				row->pushBack(infotronToCollectTextBox);

				infotronToCollectLabel->resize(infotronToCollectTextBox->width(), infotronToCollectTextBox->height());
				infotronToCollectLabel->setTextFont(Font::TimesNewRoman[24]);
				infotronToCollectLabel->setTextColor(KIR5::Color(152, 152, 202));
				infotronToCollectLabel->show();
				infotronToCollectLabel->setText("/0");
				infotronToCollectLabel->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				row->pushBack(infotronToCollectLabel);

				infotronContainer->pushBack(row);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->show();
				row->setGap(gap);

				infotronPickers.resize(numberOfinfotronIDs);
				for (int i = 0; i < numberOfinfotronIDs; ++i)
				{
					infotronPickers[i]->show();
					infotronPickers[i]->resize(buttonSize, buttonSize);
					infotronPickers[i]->id = infotronIds[i];
					infotronPickers[i]->multiplier = infotronMultipliers[i];
					infotronPickers[i]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
					{
						objectPanel->id = dynamic_cast<BlockIDPanel *>(obj_)->id;
						objectIDTextBox->setText(std::to_string(objectPanel->id));
					};

					row->pushBack(infotronPickers[i]);
				}

				infotronContainer->pushBack(row);
			}

			infotronBackground->resize(infotronContainer->width(), infotronContainer->height());
			infotronBackground->setBackgroundColor(KIR5::Color(8, 8, 8));
			infotronBackground->show();
			*infotronBackground << infotronContainer;
			infotronContainer->position(0, 0);

			infotronFrame->resize(infotronContainer->width() + 4, infotronContainer->height() + 4);
			infotronFrame->setBackgroundColor(KIR5::Color(60, 60, 60));
			infotronFrame->show();
			*infotronFrame << infotronBackground;
			infotronBackground->position(2, 2);

			toolColumns->pushBack(infotronFrame);
		}

		{
			KIR5::EVENT<KIR5::Column<KIR5::Panel>> column;
			column->show();
			column->setGap(gap);

			selectAllButton->resize(220, 18);
			selectAllButton->setTextFont(Font::TimesNewRoman[14]);
			selectAllButton->setText("Select all");
			selectAllButton->setTextColor(KIR5::Color(152, 152, 152));
			selectAllButton->show();
			selectAllButton->setTextAlignment(KIR5::CENTER);
			selectAllButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(selectAllButton);
			selectAllButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
			};

			gravityTurnOnOffButton->resize(220, 18);
			gravityTurnOnOffButton->setTextFont(Font::TimesNewRoman[14]);
			gravityTurnOnOffButton->setText("Set gravity");
			gravityTurnOnOffButton->setTextColor(KIR5::Color(152, 152, 152));
			gravityTurnOnOffButton->show();
			gravityTurnOnOffButton->setTextAlignment(KIR5::CENTER);
			gravityTurnOnOffButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(gravityTurnOnOffButton);
			gravityTurnOnOffButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
			};

			initExplodeButton->resize(220, 18);
			initExplodeButton->setTextFont(Font::TimesNewRoman[14]);
			initExplodeButton->setText("Set init explode");
			initExplodeButton->setTextColor(KIR5::Color(152, 152, 152));
			initExplodeButton->show();
			initExplodeButton->setTextAlignment(KIR5::CENTER);
			initExplodeButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(initExplodeButton);
			initExplodeButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
			};

			selectFriendlyButton->resize(220, 18);
			selectFriendlyButton->setTextFont(Font::TimesNewRoman[14]);
			selectFriendlyButton->setText("Select neighbors");
			selectFriendlyButton->setTextColor(KIR5::Color(152, 152, 152));
			selectFriendlyButton->show();
			selectFriendlyButton->setTextAlignment(KIR5::CENTER);
			selectFriendlyButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(selectFriendlyButton);
			selectFriendlyButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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

				bool isNewSelected;
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
			};

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

			ramRepairButton->resize(220, 18);
			ramRepairButton->setTextFont(Font::TimesNewRoman[14]);
			ramRepairButton->setText("Repair RAMs");
			ramRepairButton->setTextColor(KIR5::Color(152, 152, 152));
			ramRepairButton->show();
			ramRepairButton->setTextAlignment(KIR5::CENTER);
			ramRepairButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(ramRepairButton);
			ramRepairButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
			};

			randomizeRamsButton->resize(220, 18);
			randomizeRamsButton->setTextFont(Font::TimesNewRoman[14]);
			randomizeRamsButton->setText("Randomize RAMs");
			randomizeRamsButton->setTextColor(KIR5::Color(152, 152, 152));
			randomizeRamsButton->show();
			randomizeRamsButton->setTextAlignment(KIR5::CENTER);
			randomizeRamsButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(randomizeRamsButton);
			randomizeRamsButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
			};

			spawnPointButton->resize(220, 18);
			spawnPointButton->setTextFont(Font::TimesNewRoman[14]);
			spawnPointButton->setText("Set SPAWN");
			spawnPointButton->setTextColor(KIR5::Color(152, 152, 152));
			spawnPointButton->show();
			spawnPointButton->setTextAlignment(KIR5::CENTER);
			spawnPointButton->setColor(KIR5::Color(30, 30, 30));
			column->pushBack(spawnPointButton);
			spawnPointButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
			};

			toolColumns->pushBack(column);
		}

		{
			prevBlockPickers.resize(24);
			for (size_t i = 0; i < prevBlockPickers.size(); ++i)
			{
				prevBlockPickers[i]->resize(32, 32);
				prevBlockPickers[i]->show();
				prevBlockPickers[i]->id = i;
				prevBlockPickers[i]->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					objectPanel->id = dynamic_cast<BlockIDPanel *>(obj_)->id;
					objectIDTextBox->setText(std::to_string(objectPanel->id));
				};

				prevBlockColumns->pushBack(prevBlockPickers[i]);
			}

		}

		toolColumns->show();
		prevBlockColumns->show();


		nameOfTheMapTextBox->resize(300, textBoxSize);
		nameOfTheMapTextBox->setTextFont(Font::TimesNewRoman[24]);
		nameOfTheMapTextBox->setTextColor(KIR5::Color(152, 152, 152));
		nameOfTheMapTextBox->show();
		nameOfTheMapTextBox->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
		nameOfTheMapTextBox->setColor(KIR5::Color(5, 5, 5));

		saveButton->resize(70, 32);
		saveButton->setTextFont(Font::TimesNewRoman[24]);
		saveButton->setText("Save");
		saveButton->setTextColor(KIR5::Color(152, 152, 152));
		saveButton->show();
		saveButton->setTextAlignment(KIR5::CENTER);
		saveButton->setColor(KIR5::Color(30, 30, 30));
		saveButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			bluePrint->resize(reach(map));
			bluePrint->foreach([&](Type::Coord coord, BluePrintBlock &block)
			{
				block.id = reach(map)[coord].object->id;
				block.flags = reach(map)[coord].grid;
				block.rotation = reach(map)[coord].object->rotation;
			});
			bluePrint->SetAim(std::atol(infotronToCollectTextBox->getText().c_str()));
			bluePrint->SetName(nameOfTheMapTextBox->getText());

			eventEngine->sendEvent((void *)REFRESH_ITEM, (void *)(bluePrint.get()), (void *)0, (void *)0);
		};

		containerRows->pushBack(nameOfTheMapTextBox);
		containerRows->pushBack(toolColumns);
		containerRows->pushBack(prevBlockColumns);
		containerRows->pushBack(saveButton);

		containerRows->show();
		*this << containerRows;
	}
	ControlPanel::~ControlPanel()
	{

	}


	void ControlPanel::setOperationMode()
	{
		KIR5::Color bcolor = mainEvent->activeMap->isOperationModeAll() ? KIR5::Color(78, 30, 30) : KIR5::Color(30, 30, 78);

		rotationExecute->setColor(bcolor);
		fillButton->setColor(bcolor);
		fillSelectAllButton->setColor(bcolor);
		fillFrameButton->setColor(bcolor);
		fillContentButton->setColor(bcolor);
		gravityTurnOnOffButton->setColor(bcolor);
		initExplodeButton->setColor(bcolor);
		spawnPointButton->setColor(bcolor);

		randomizeRamsButton->setColor(bcolor);
		ramRepairButton->setColor(bcolor);
	}

	void ControlPanel::pickID(Type::ID id)
	{
		objectPanel->id = id;
		objectIDTextBox->setText(std::to_string(objectPanel->id));
		updateBlocks();
	}

	void ControlPanel::updateBlocks()
	{
		objectPanel->counter = 0;
		for (auto &it : infotronPickers)
		{
			it->counter = 0;
		}
		this->map->foreach([&](const Type::Coord &coord, ActiveBlock<EditorObjectBase> &block)
		{
			if (block.object->id == objectPanel->id)
			{
				++objectPanel->counter;
			}
			for (auto &it : infotronPickers)
			{
				if (block.object->id == it->id)
				{
					++it->counter;
				}
			}
		});

		int totalInfotrons = 0;
		for (auto &it : infotronPickers)
		{
			totalInfotrons += it->counter * it->multiplier;
		}
		infotronToCollectLabel->setText("/" + std::to_string(totalInfotrons));

	}

	void ControlPanel::SetMap(std::shared_ptr<BluePrint> &bluePrint_, std::shared_ptr<Array2D<ActiveBlock<EditorObjectBase>>> &map)
	{
		bluePrint = bluePrint_;
		this->map = map;
		infotronToCollectTextBox->setText(std::to_string(bluePrint->GetAim()));
		nameOfTheMapTextBox->setText(bluePrint->GetName());
		mapResizeTextBox->setText(std::to_string(((Type::Size)(*map)).width) + "*" + std::to_string(((Type::Size)(*map)).height));
		updateBlocks();
	}
}
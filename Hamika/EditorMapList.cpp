#include "EditorMapList.h"
#include "Global.h"
#include "EditorMainEvent.h"

#include <KIR/sys/KIR5_files.h>
#include <KIR/sys/KIR5_system.h>
#include <KIR/AL/KIR5_panel_control.h>

namespace Editor
{
	MapList::Item::Item(MapList &mapList, int ID_, std::shared_ptr<BluePrint> &bluePrint_):
		bluePrint(bluePrint_)
	{
		*this << ID;
		ID->setTextFont(Font::Consolas[18]);
		ID->show();
		ID->setTextAlignment(KIR5::CENTER);
		ID->setTextColor(KIR5::Color(230, 120, 40));
		ID->fncKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
		{
			if (ID->isActiveBox())
			{
				if (key_ == ALLEGRO_KEY_ENTER)
				{
					eventEngine->sendEvent((void *)MOVE_ITEM_POSITION, (void *)(this), (void *)(std::atoi(ID->getText().c_str())), (void *)0);
					return true;
				}
			}
			return false;
		};

		*this << name;
		name->show();
		name->setTextAlignment(KIR5::CENTER);
		name->setTextFont(ID->getTextFont());
		name->setTextColor(KIR5::Color(180, 180, 180));
		name->setText(bluePrint->GetName().c_str());

		*this << moveUp;
		moveUp->show();
		moveUp->setBitmap(mapList.arrowUpImg);
		moveUp->fncPress = [&](FNC_PRESS_PARAMS)
		{
			if (this->ID_ > 0)
			{
				eventEngine->sendEvent((void *)MOVE_ITEM_UP, (void *)(this), (void *)0, (void *)0);
			}
		};

		*this << moveDown;
		moveDown->show();
		moveDown->setBitmap(mapList.arrowDownImg);
		moveDown->fncPress = [&](FNC_PRESS_PARAMS)
		{
			eventEngine->sendEvent((void *)MOVE_ITEM_DOWN, (void *)(this), (void *)0, (void *)0);
		};

		*this << deleteMap;
		deleteMap->show();
		deleteMap->setBitmap(mapList.deleteImg);
		deleteMap->fncPress = [&](FNC_PRESS_PARAMS)
		{
			eventEngine->sendEvent((void *)DELETE_ITEM, (void *)(this), (void *)0, (void *)0);
		};

		*this << editMap;
		editMap->show();
		editMap->setBitmap(mapList.editImg);
		editMap->fncPress = [&](FNC_PRESS_PARAMS)
		{
			mainEvent->activeMap->SetMap(this->bluePrint);
		};


		*this << blueprintPanel;
		blueprintPanel->show();
		blueprintPanel->setBluePrint(bluePrint);

		FncMoved.lock(this->fncMoved);
		FncMoved.set([&](FNC_MOVED_PARAMS)
		{
			ID->move(0, 0, 40, 30);
			name->move(ID->virtualx2(), 0, width() - ID->virtualx2(), 30);

			moveUp->move(name->virtualx2() - 30, name->virtualy2(), 30, 15);
			moveDown->move(name->virtualx2() - 30, moveUp->virtualy2(), 30, 15);

			deleteMap->move(name->virtualx2() - 30, moveDown->virtualy2(), 30, 30);
			editMap->move(name->virtualx2() - 30, deleteMap->virtualy2(), 30, 30);

			blueprintPanel->move(0, ID->virtualy2(), width() - moveUp->width(), height() - ID->height());
		});


		FncDraw.lock(fncDraw);
		FncDraw.set([&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(al_map_rgb(15, 15, 16));
		});

		FncDrawBlueprint.lock(blueprintPanel->fncDraw);
		FncDrawBlueprint.set([&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(al_map_rgb(35, 35, 36));
		});

		fncMoved(this);
		setID(ID_);
	}
	void MapList::Item::setID(int ID_)
	{
		this->ID_ = ID_;
		char buffer[32];
		sprintf(buffer, "%03d", ID_);
		ID->setText(buffer);
	}


	MapList::MapList()
	{
		{
			savePanel->show();
			savePanel->resize(400, 300);
			savePanel->fncDraw = [&](FNC_DRAW_PARAMS)
			{
				al_clear_to_color(KIR5::Color(20, 20, 20));
			};

			KIR5::EVENT<KIR5::Column<KIR5::Panel>> col;
			col->setGap(3);
			col->show();

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->setGap(3);
				row->show();

				saveDirectoryLabel->setTextFont(Font::TimesNewRoman[14]);
				saveDirectoryLabel->setTextColor(KIR5::Color(100, 100, 100));
				saveDirectoryLabel->setTextAlignment(KIR5::CENTER);
				saveDirectoryLabel->show();
				saveDirectoryLabel->setText("Directory: ");
				saveDirectoryLabel->resize((std::max)(saveWordNameDirectoryLabel->getTextWidth(), saveDirectoryLabel->getTextWidth()), 26);
				row->pushBack(saveDirectoryLabel);

				saveDirectory->setTextFont(Font::Consolas[12]);
				saveDirectory->show();
				saveDirectory->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				saveDirectory->setTextColor(KIR5::Color(230, 120, 40));
				saveDirectory->resize(300, 26);
				row->pushBack(saveDirectory);

				col->pushBack(row);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->setGap(3);
				row->show();

				saveWordNameDirectoryLabel->setTextFont(Font::TimesNewRoman[14]);
				saveWordNameDirectoryLabel->setTextColor(KIR5::Color(100, 100, 100));
				saveWordNameDirectoryLabel->setTextAlignment(KIR5::CENTER);
				saveWordNameDirectoryLabel->show();
				saveWordNameDirectoryLabel->setText("Word name: ");
				saveWordNameDirectoryLabel->resize((std::max)(saveWordNameDirectoryLabel->getTextWidth(), saveDirectoryLabel->getTextWidth()), 26);
				row->pushBack(saveWordNameDirectoryLabel);

				saveWordNameDirectory->setTextFont(Font::Consolas[12]);
				saveWordNameDirectory->show();
				saveWordNameDirectory->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
				saveWordNameDirectory->setTextColor(KIR5::Color(230, 120, 40));
				saveWordNameDirectory->resize(300, 26);
				row->pushBack(saveWordNameDirectory);

				col->pushBack(row);
			}

			{
				KIR5::EVENT<KIR5::Row<KIR5::Panel>> row;
				row->setGap(3);
				row->show();

				saveOriginal->setTextFont(Font::TimesNewRoman[14]);
				saveOriginal->setTextColor(KIR5::Color(100, 100, 100));
				saveOriginal->setTextAlignment(KIR5::CENTER);
				saveOriginal->show();
				saveOriginal->setColor(KIR5::Color(50, 50, 50));
				saveOriginal->setText("Save original");
				saveOriginal->resize(saveOriginal->getTextWidth(), 26);
				row->pushBack(saveOriginal);
				saveOriginal->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					OriginalLevelWorldIO io;
					for (auto &it : list->items())
					{
						io.levels.push_back(it->bluePrint);
					}
					io.Write(KIR5::pathCombine<>(saveDirectory->getText(), saveWordNameDirectory->getText()));
					savePanelBackground->hide();
				};

				col->pushBack(row);
			}

			saveCancel->setTextFont(Font::TimesNewRoman[14]);
			saveCancel->setTextColor(KIR5::Color(100, 100, 100));
			saveCancel->setTextAlignment(KIR5::CENTER);
			saveCancel->show();
			saveCancel->setColor(KIR5::Color(50, 50, 50));
			saveCancel->setText("Cancel");
			saveCancel->move(savePanel->width() - saveCancel->getTextWidth() - 3, savePanel->height() - 26 - 3, saveCancel->getTextWidth(), 26);
			saveCancel->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				savePanelBackground->hide();
			};

			*savePanel << col << saveCancel;
			col->position(3, 3);

			*savePanelBackground << savePanel;
			savePanelBackground->fncDraw = [&](FNC_DRAW_PARAMS)
			{
				al_draw_filled_rectangle(x_, y_, w_, h_, KIR5::Color(0, 0, 0, 127).getAlphaColored());
			};

			savePanelBackground->fncMoved = [&](FNC_MOVED_PARAMS)
			{
				savePanel->align(KIR5::CENTER);
			};
		}






		arrowUpImg.load("Hamika\\texture\\editor\\StickUp.png");
		arrowDownImg = arrowUpImg;
		arrowDownImg.flipVertical();
		editImg.load("Hamika\\texture\\editor\\Edit.png");
		deleteImg.load("Hamika\\texture\\editor\\Delete.png");

		loadButton->setTextFont(Font::TimesNewRoman[12]);
		loadButton->setTextColor(KIR5::Color(100, 100, 100));
		loadButton->setTextAlignment(KIR5::CENTER);
		loadButton->show();
		loadButton->setColor(KIR5::Color(50, 50, 50));
		loadButton->setText("Load");
		*this << loadButton;
		loadButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			std::string filename;
			std::cout << KIR5::getCurrentDirectory<>() << std::endl;
			{
				KIR5::CurrentDirectoryGuard<> dirGuard;
				filename = KIR5::browseForFile<>();
			}
			std::cout << KIR5::getCurrentDirectory<>() << std::endl;
			if (filename.length() > 0)
			{
				OriginalLevelWorldIO io;
				io.Read(filename.c_str());
				setWorld(io.levels);
			}
		};

		saveButton->setTextFont(Font::TimesNewRoman[12]);
		saveButton->setTextColor(KIR5::Color(100, 100, 100));
		saveButton->setTextAlignment(KIR5::CENTER);
		saveButton->show();
		saveButton->setColor(KIR5::Color(50, 50, 50));
		saveButton->setText("Save");
		*this << saveButton;
		saveButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			std::string directory;
			std::cout << KIR5::getCurrentDirectory<>() << std::endl;
			{
				KIR5::CurrentDirectoryGuard<> dirGuard;
				directory = KIR5::browseForFolder<>();
			}
			saveDirectory->setText(directory);
			saveWordNameDirectory->setText("word.dat");
			savePanelBackground->show();
			savePanelBackground->bringTop();
		};

		newMapButton->setTextFont(Font::TimesNewRoman[12]);
		newMapButton->setTextColor(KIR5::Color(100, 100, 100));
		newMapButton->setTextAlignment(KIR5::CENTER);
		newMapButton->show();
		newMapButton->setColor(KIR5::Color(50, 50, 50));
		newMapButton->setText("New map");
		*this << newMapButton;
		newMapButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			KIR5::EVENT<BluePrint> bluePrint;
			bluePrint->resize({10,10});
			bluePrint->foreach([](Type::Coord coord, BluePrintBlock &block)
			{
				block.id = 0;
				block.flags = 0;
				block.rotation = 0;
			});

			list->pushBack(KIR5::EVENT<Item>(new Item(*this, list->items().size() + 1, bluePrint)));

			scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
			scrollBar->cursor(0xFFFFFFFF);
		};

		newWorldButton->setTextFont(Font::TimesNewRoman[12]);
		newWorldButton->setTextColor(KIR5::Color(100, 100, 100));
		newWorldButton->setTextAlignment(KIR5::CENTER);
		newWorldButton->show();
		newWorldButton->setColor(KIR5::Color(50, 50, 50));
		newWorldButton->setText("New world");
		*this << newWorldButton;
		newWorldButton->fncPress = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			list->clear();

			KIR5::EVENT<BluePrint> bluePrint;
			bluePrint->resize({10,10});
			bluePrint->foreach([](Type::Coord coord, BluePrintBlock &block)
			{
				block.id = 0;
				block.flags = 0;
				block.rotation = 0;
			});

			list->pushBack(KIR5::EVENT<Item>(new Item(*this, list->items().size() + 1, bluePrint)));

			scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
			scrollBar->cursor(0xFFFFFFFF);
		};

		*this << list;
		list->show();
		list->itemHeight(120);

		*this << scrollBar;
		scrollBar->show();
		scrollBar->setThumb(scrollBarThumb);
		scrollBar->fncDraw = [&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(KIR5::Color(62, 62, 62));
		};

		scrollBarThumb->setColor(KIR5::Color(104, 104, 104));

		FncMoved.lock(this->fncMoved);
		FncMoved.set([&](FNC_MOVED_PARAMS)
		{
			constexpr int buttonHeight = 30;
			list->move(0, 0, width() - 15, height() - buttonHeight);
			scrollBar->move(width() - 15, 0, 15, height() - buttonHeight);
			scrollBar->sizeOfPage(height() - buttonHeight);

			loadButton->move(0, height() - buttonHeight, loadButton->getTextWidth() + 20, buttonHeight);
			saveButton->move(loadButton->virtualx2(), height() - buttonHeight, saveButton->getTextWidth() + 20, buttonHeight);
			newMapButton->move(saveButton->virtualx2(), height() - buttonHeight, newMapButton->getTextWidth() + 20, buttonHeight);
			newWorldButton->move(newMapButton->virtualx2(), height() - buttonHeight, newWorldButton->getTextWidth() + 20, buttonHeight);
		});

		fncUpdate = [&](FNC_UPDATE_PARAMS)
		{
			if (ALLEGRO_EVENT_TYPE_IS_USER(ptr_->type))
			{
				switch (ptr_->user.data1)
				{
					case DELETE_ITEM:
					{
						void *ptr = (void *)ptr_->user.data2;
						for (size_t i = 0; i < list->items().size(); ++i)
						{
							if (list->items()[i].get() == ptr)
							{
								list->erase(i);
								scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
								break;
							}
						}

						break;
					}
					case MOVE_ITEM_UP:
					{
						void *ptr = (void *)ptr_->user.data2;
						for (size_t i = 0; i < list->items().size(); ++i)
						{
							if (list->items()[i].get() == ptr)
							{
								auto item = list->erase(i);
								list->insert(item, (std::max)((int)i - 1, 0));
								scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
								break;
							}
						}

						break;
					}
					case MOVE_ITEM_DOWN:
					{
						void *ptr = (void *)ptr_->user.data2;
						for (size_t i = 0; i < list->items().size(); ++i)
						{
							if (list->items()[i].get() == ptr)
							{
								auto item = list->erase(i);
								list->insert(item, (std::min)(i + 1, list->items().size()));
								scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
								break;
							}
						}

						break;
					}
					case MOVE_ITEM_POSITION:
					{
						void *ptr = (void *)ptr_->user.data2;
						size_t newID = (size_t)ptr_->user.data3;

						for (size_t i = 0; i < list->items().size(); ++i)
						{
							if (list->items()[i].get() == ptr)
							{
								auto item = list->erase(i);
								list->insert(item, (std::max)(size_t(0), (std::min)(newID - 1, list->items().size())));
								scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
								break;
							}
						}

						for (size_t i = 0; i < list->items().size(); ++i)
						{
							list->items()[i]->setID(i + 1);
						}

						break;
					}
					case REFRESH_ITEM:
					{
						void *ptr = (void *)ptr_->user.data2;
						for (size_t i = 0; i < list->items().size(); ++i)
						{
							if (list->items()[i]->bluePrint.get() == ptr)
							{
								list->items()[i]->blueprintPanel->redrawn = true;
								break;
							}
						}

						break;
					}
					default:
					{
						break;
					}
				}
			}
		};

		fncMoved(this);
	}
	MapList::~MapList()
	{

	}
	int MapList::staticWidth()
	{
		return 190;
	}
	void MapList::setWorld(std::vector<std::shared_ptr<BluePrint>> &world)
	{
		while (list->items().size() > 0)
		{
			list->erase(0);
		}

		int ID_ = 1;
		for (auto &it : world)
		{
			list->pushBack(KIR5::EVENT<Item>(new Item(*this, ID_++, it)));
		}

		scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
		scrollBar->cursor(0);
		list->cursor(0);
	}
}
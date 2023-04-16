#include "EditorWorld.h"
#include "Tools.h"
#include "EditorMainEvent.h"
#include "Resource.h"

#include <KIR/sys/KIR5_files.h>
#include <KIR/sys/KIR5_system.h>
#include <KIR/AL/KIR5_panel_control.h>

namespace UI::Editor
{
	Worldi::Item::Item(Worldi &mapList, int ID_, const std::shared_ptr<Res::BluePrint> &bluePrint_):
		bluePrint(bluePrint_)
	{
		*this << ID;
		ID->fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
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
		});

		*this << name;
		name->setText(bluePrint->title);

		*this << moveUp;
		moveUp->setBitmap(Res::uielements[Res::UIElements::ArrowUp]);
		moveUp->fncPress.push_back([&](FNC_PRESS_PARAMS)
		{
			if (this->ID_ > 0)
			{
				eventEngine->sendEvent((void *)MOVE_ITEM_UP, (void *)(this), (void *)0, (void *)0);
			}
		});

		*this << moveDown;
		moveDown->setBitmap(Res::uielements[Res::UIElements::ArrowDown]);
		moveDown->fncPress.push_back([&](FNC_PRESS_PARAMS)
		{
			eventEngine->sendEvent((void *)MOVE_ITEM_DOWN, (void *)(this), (void *)0, (void *)0);
		});

		*this << deleteMap;
		deleteMap->setBitmap(Res::uielements[Res::UIElements::Delete]);
		deleteMap->fncPress.push_back([&](FNC_PRESS_PARAMS)
		{
			eventEngine->sendEvent((void *)DELETE_ITEM, (void *)(this), (void *)0, (void *)0);
		});

		*this << editMap;
		editMap->setBitmap(Res::uielements[Res::UIElements::Edit]);
		editMap->fncPress.push_back([&](FNC_PRESS_PARAMS)
		{
			MainEvent::s_object->activeMap->SetMap(this->bluePrint);
		});


		*this << blueprintPanel;
		blueprintPanel->bluePrint(bluePrint);

		this->fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			ID->move(0, 0, 40, 30);
			name->move(ID->virtualx2(), 0, width() - ID->virtualx2(), 30);

			moveUp->move(name->virtualx2() - 30, name->virtualy2(), 30, 15);
			moveDown->move(name->virtualx2() - 30, moveUp->virtualy2(), 30, 15);

			deleteMap->move(name->virtualx2() - 30, moveDown->virtualy2(), 30, 30);
			editMap->move(name->virtualx2() - 30, deleteMap->virtualy2(), 30, 30);

			blueprintPanel->move(0, ID->virtualy2(), width() - moveUp->width(), height() - ID->height());
		});


		fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
		{
			al_clear_to_color(al_map_rgb(15, 15, 16));
		}));

		blueprintPanel->fncDraw.push_front(FNC_DRAW([&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
		{
			al_clear_to_color(al_map_rgb(35, 35, 36));
		}));

		callbackMove();
		setID(ID_);
	}
	void Worldi::Item::setID(int ID_)
	{
		this->ID_ = ID_;
		char buffer[32];
		sprintf(buffer, "%03d", ID_);
		ID->setText(buffer);
	}
	const std::shared_ptr<Res::BluePrint> &Worldi::Item::getBluePrint() const
	{
		return bluePrint;
	}


	Worldi::Worldi()
	{
		*this << content;
		content->show();
		content->setGap(3);
		(*content) += KIR5::Column<Panel>::WrapHeight;
		(*content) += KIR5::Column<Panel>::ConsistentWidth;

		{
			KIR5::Shared<KIR5::Row<>> row;
			row->show();
			row->setGap(3);
			(*row) += KIR5::Row<>::WrapWidth;
			(*row) += KIR5::Row<>::ConsistentHeight;

			worltTitle_Label->setText("World title:");
			worltTitle_Label->height(30);
			worltTitle_Label->width(worltTitle_Label->getTextWidth() + 10);
			row->pushBack(worltTitle_Label);

			row->pushBack(worltTitle_TextBox);

			row->fncMoved.push_back([&](FNC_MOVED_PARAMS)->FNC_MOVED_RET
			{
				dynamic_cast<KIR5::Row<> *>(obj_)->wrapItem(worltTitle_TextBox.get());
			});

			content->pushBack(row);
		}

		{
			content->pushBack(KIR5::Shared<Line<2, 2>>());
		}

		{
			listPanel->show();

			list->show();
			list->itemHeight(120);
			*listPanel << list;

			scrollBar->show();
			scrollBar->setThumb(scrollBarThumb);
			*listPanel << scrollBar;
			scrollBar->fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
			{
				al_clear_to_color(KIR5::Color(62, 62, 62));
			}));

			scrollBarThumb->setColor(KIR5::Color(104, 104, 104));

			listPanel->fncMoved.push_back([&](FNC_MOVED_PARAMS)
			{
				list->move(0, 0, listPanel->width() - 15, listPanel->height());
				scrollBar->move(width() - 15, 0, 15, listPanel->height());
				scrollBar->sizeOfPage(listPanel->height());
			});

			content->pushBack(listPanel);
		}

		{
			content->pushBack(KIR5::Shared<Line<2, 2>>());
		}

		{
			KIR5::Shared<KIR5::Row<>> row;
			row->setGap(3);
			row->show();
			(*row) += KIR5::Row<>::ConsistentHeight;

			loadButton->setText("Load");
			loadButton->height(30);
			loadButton->width(loadButton->getTextWidth() + 20);
			row->pushBack(loadButton);
			loadButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
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
					std::shared_ptr<Res::World> world(new Res::World(filename));
					Res::LoadResource(*world);
					setWorld(world);
				}
			});

			saveButton->setText("Save");
			saveButton->height(30);
			saveButton->width(saveButton->getTextWidth() + 20);
			row->pushBack(saveButton);
			saveButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				MainEvent::s_object->saveWorldDialog->show();
			});

			newMapButton->setText("New map");
			newMapButton->height(30);
			newMapButton->width(newMapButton->getTextWidth() + 20);
			row->pushBack(newMapButton);
			newMapButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				KIR5::Shared<Res::BluePrint> bluePrint;
				bluePrint->blocks.resize({10,10});
				bluePrint->blocks.foreach([](Type::Coord coord, Res::BluePrint::Block &block)
				{
					block.id = 0;
					block.flags = 0;
					block.rotation = 0;
				});

				list->pushBack(KIR5::Shared<Item>(new Item(*this, list->items().size() + 1, bluePrint)));

				scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
				scrollBar->cursor(0xFFFFFFFF);
			});

			newWorldButton->setText("New world");
			newWorldButton->height(30);
			newWorldButton->width(newWorldButton->getTextWidth() + 20);
			row->pushBack(newWorldButton);
			newWorldButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				list->clear();

				KIR5::Shared<Res::BluePrint> bluePrint;
				bluePrint->blocks.resize({10,10});
				bluePrint->blocks.foreach([](Type::Coord coord, Res::BluePrint::Block &block)
				{
					block.id = 0;
					block.flags = 0;
					block.rotation = 0;
				});

				list->pushBack(KIR5::Shared<Item>(new Item(*this, list->items().size() + 1, bluePrint)));

				scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
				scrollBar->cursor(0xFFFFFFFF);
			});

			content->pushBack(row);
		}

		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			content->move(0, 0, width(), height());
			content->wrapItem(listPanel.get());
		});

		fncUpdate.push_back([&](FNC_UPDATE_PARAMS)
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
								list->items()[i]->blueprintPanel->redraw();
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
		});
	}
	Worldi::~Worldi()
	{

	}
	int Worldi::staticWidth()
	{
		return 190;
	}
	void Worldi::setWorld(std::shared_ptr<Res::World> &world)
	{
		while (list->items().size() > 0)
		{
			list->erase(0);
		}

		int ID_ = 1;
		for (auto &it : world->bluePrints)
		{
			list->pushBack(KIR5::Shared<Item>(new Item(*this, ID_++, it)));
		}

		scrollBar->sizeOfBook(list->itemHeight() * list->items().size());
		scrollBar->cursor(0);
		list->cursor(0);

		MainEvent::s_object->worldi->worltTitle_TextBox->setText(world->title);
	}
}
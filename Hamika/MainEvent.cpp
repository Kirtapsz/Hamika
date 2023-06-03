#include "MainEvent.h"
#include "Font.h"

#include "OriginalObjects.h"
#include "Worlds.h"

#include <KIR\AL\KIR5_bitmap_target.h>
#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\sys\KIR5_files.h>

namespace UI
{
	MainEvent::Menu::Worlds::Worlds()
	{
		(*this)->addFlag(KIR5::Row<Panel, WorldButton>::FixHeight);
		(*this)->setGap(3);

		for (auto &it : Res::worlds)
		{
			(*this)->pushBack(KIR5::Shared<WorldButton>(it));
		}
#ifdef _DEBUG 
		for (auto &it : Res::testWorlds)
		{
			(*this)->pushBack(KIR5::Shared<WorldButton>(it));
		}
#endif

		get()->fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			for (auto &it : (*this)->items())
			{
				it->resize((*this)->height(), (*this)->height());
			}
		});
	}
	MainEvent::Menu::OpenSettings::OpenSettings()
	{
		get()->setBitmap(Res::uielements[Res::UIElements::Settings]);

		get()->fncPress.push_back([](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
		{
			s_object->hideDynamicPanel();
			s_object->menu.settings->show();
		});
	}
	MainEvent::Menu::Settings::Settings()
	{
		panelLabel->setText("Settings");

		cancelButton->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			s_object->defaultDynamicPanel();
		});

		(*get()) << panelLabel << cancelButton;
	}
	MainEvent::Menu::Accounts::Accounts()
	{
		panelLabel->setText("Accounts");

		cancelButton->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			s_object->defaultDynamicPanel();
		});

		(*get()) << panelLabel << cancelButton << list << create;
		list->show();
		create->show();

		refresh();
	}
	void MainEvent::Menu::Accounts::refresh()
	{
		list.items->clear();
		for (auto &it : Res::accounts.list)
		{
			KIR5::Shared<UI::Button<Res::Consolas>> accountButton;
			accountButton->setText(it->username());
			accountButton->fncPress.push_back([](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
			{
				UI::Button<Res::Consolas> *_ptr = dynamic_cast<UI::Button<Res::Consolas>*>(obj_);
				s_object->setAccount(Res::accounts.get(_ptr->getText()));
			});
			list.items->pushBack(accountButton);
			list.scrollBar->sizeOfBook(list.items->itemHeight() * list.items->items().size());
		}
	}
	MainEvent::Menu::BlueprintInfo::BlueprintInfo()
	{
		*get()
			<< ID
			<< title
			<< hallOfFlameLabel
			<< hallOfFlameLine
			<< personalRecordLabel
			<< personalRecord;

		personalRecordLabel->setText("Personal record:");
		personalRecord->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);
		hallOfFlameLabel->setText("HALL OF FLAME");
		hallOfFlameLabel->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);

		KIR5::Panel::FNC_PRESS fncPress = [&](FNC_PRESS_PARAMS) -> FNC_PRESS_RET
		{
			HallOfFlameRow *hallOfFlameRow = dynamic_cast<HallOfFlameRow *>(obj_->getParent());
			MainEvent::s_object->replayGame(hallOfFlameRow->_replayUuid);
		};

		for (std::size_t i = 0; i < hallOfFlameRows.size(); ++i)
		{
			*get() << hallOfFlameRows[i];

			*hallOfFlameRows[i]
				<< hallOfFlameRows[i]->rank
				<< hallOfFlameRows[i]->username
				<< hallOfFlameRows[i]->time;

			hallOfFlameRows[i]->rank->setText("#" + std::to_string(i));
			hallOfFlameRows[i]->time->setTextAlignment(KIR5::RIGHT | KIR5::VCENTER | KIR5::IGNORE_DESCENT);
			hallOfFlameRows[i]->rank->fncPress.push_back(fncPress);
		}
	}
	void MainEvent::Menu::BlueprintInfo::refresh()
	{
		if (MainEvent::s_object->menu.bluePrints->getWorld())
		{
			char buffer[32];

			ID->show();
			title->show();
			hallOfFlameLabel->show();
			hallOfFlameLine->show();

			sprintf_s(buffer, "%03d", MainEvent::s_object->menu.bluePrints->getIndex());
			ID->setText(buffer);

			auto bluePrint_ = MainEvent::s_object->menu.bluePrints->getBluePrint();
			title->setText(bluePrint_->title);

			if (MainEvent::s_object->account_)
			{
				auto _completedBlueprints = MainEvent::s_object->account_->completedBlueprints();
				auto item = std::find_if(_completedBlueprints.cbegin(), _completedBlueprints.cend(), [&bluePrint_](const Res::Account::CompletedBluePrint &completedBluePrint) -> bool
				{
					return bluePrint_->hash == completedBluePrint.bluePrintHash();
				});
				if (item != _completedBlueprints.end())
				{
					personalRecord->setText(hourStopper(item->timeMS()));

					personalRecordLabel->show();
					personalRecord->show();
				}
				else
				{
					personalRecordLabel->hide();
					personalRecord->hide();
				}
			}
			else
			{
				personalRecordLabel->hide();
				personalRecord->hide();
			}

			struct Record
			{
				std::string _replayUuid;
				std::string _username;
				std::uint32_t _timeMS;
			};
			std::vector<Record> records;
			for (auto &account : Res::accounts.list)
			{
				const Res::Account::CompletedBluePrint *record = account->getRecordOn(bluePrint_);
				if (record != nullptr)
				{
					records.push_back({
						record->replayUuid(),
						account->username(),
						record->timeMS()
									  });
				}
			}

			std::sort(records.begin(), records.end(), [](const Record &left, const Record &right) -> bool
			{
				return left._timeMS < right._timeMS;
			});

			for (std::size_t i = 0; i < hallOfFlameRows.size(); ++i)
			{
				if (i < records.size())
				{
					hallOfFlameRows[i]->show();
					hallOfFlameRows[i]->username->setText(records[i]._username);
					hallOfFlameRows[i]->time->setText(hourStopper(records[i]._timeMS));
					hallOfFlameRows[i]->_replayUuid = records[i]._replayUuid;
				}
				else
				{
					hallOfFlameRows[i]->hide();
				}
			}
		}
		else
		{
			ID->hide();
			title->hide();
			hallOfFlameLabel->hide();
			hallOfFlameLine->hide();
			personalRecordLabel->hide();
			personalRecord->hide();
			for (auto &it : hallOfFlameRows)
			{
				it->hide();
			}
		}
	}
	MainEvent::Menu::CurrentAccount::CurrentAccount()
	{
		panelLabel->setText("Account");

		usernameLabel->setText("Username  : ");
		totalTimePlayedLabel->setText("Played    : ");
		numberOfCompletedMapsLabel->setText("Completed : ");

		cancelButton->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			s_object->setAccount(nullptr);
			s_object->defaultDynamicPanel();
		});

		(*get()) << panelLabel << cancelButton
			<< usernameLabel << totalTimePlayedLabel << numberOfCompletedMapsLabel
			<< username << totalTimePlayed << numberOfCompletedMaps;
	}
	void MainEvent::Menu::CurrentAccount::refresh()
	{
		if (s_object->account_)
		{
			username->setText(s_object->account_->username());
			totalTimePlayed->setText(dayStopper(s_object->account_->totalTimePlayedMS()));
			numberOfCompletedMaps->setText(std::to_string(s_object->account_->completedBlueprints().size()));
		}
	}
	MainEvent::Menu::CreateAccount::CreateAccount()
	{
		panelLabel->setText("Add account");
		usernameLabel->setText("Username:");
		passwrodLabel->setText("Password:");

		cancelButton->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			s_object->defaultDynamicPanel();
		});

		applyButton->setText("Create");
		applyButton->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			const std::string &username = s_object->menu.createAccount.usernameTextBox->getText();
			const std::string &password = s_object->menu.createAccount.passwrodTextBox->getText();
			try
			{
				Res::accounts.add(username, password);
				s_object->menu.accounts.refresh();
				s_object->setAccount(Res::accounts.get(username));
			}
			catch (const std::exception &)
			{

			}

			s_object->defaultDynamicPanel();
		});

		get()->hide();
		(*get()) << panelLabel << usernameTextBox << passwrodTextBox << usernameLabel << passwrodLabel << cancelButton << applyButton;
	}

	std::shared_ptr<MainEvent> MainEvent::s_object;

	MainEvent::WorldButton::WorldButton(const std::shared_ptr<Res::World> &_world):
		world_(_world)
	{
		show();
		setBitmap(world_->thumbnail);

		fncPress.push_back([&](FNC_PRESS_PARAMS)
		{
			s_object->menu.bluePrints->setWorld(world_);
			s_object->menu.worldTitle->setText(world_->title);
		});
	}

	void MainEvent::initialize(std::shared_ptr<KIR5::Panel> _parent)
	{
		new MainEvent(display);
	}
	void MainEvent::shutdown()
	{
		s_object = nullptr;
	}
	MainEvent::MainEvent(const std::shared_ptr<KIR5::Panel> &_parent):
		parent(_parent)
	{
		s_object = std::shared_ptr<MainEvent>(this);

		*parent << menu;
		menu->show();

		parent->fncMoved.push_back([&](FNC_MOVED_PARAMS)
		{
			static constexpr float rw = decltype(menu)::element_type::ADJUSTER_WIDTH;
			static constexpr float rh = decltype(menu)::element_type::ADJUSTER_HEIGHT;
			float rate = (std::min)(parent->width() / rw, parent->height() / rh) * 0.95f;
			menu->resize(rw * rate, rh * rate);
			menu->align(KIR5::CENTER);

			if (scene_)
			{
				scene_->move(0, 0, parent->width(), parent->height());
			}
		});
		setAccount(nullptr);
		menu.currentAccount.refresh();
		menu.blueprintInfo.refresh();
		menu.bluePrints->refresh();

		parent->fncUpdate.push_back([&](FNC_UPDATE_PARAMS)
		{
			if (ALLEGRO_EVENT_TYPE_IS_USER(ptr_->type))
			{
				switch (ptr_->user.data1)
				{
					case Scene::ACTIVE_MAP_FINISHED:
					{
						finishGame(ptr_->user.data2);

						break;
					}
					default:
					{
						break;
					}
				}
			}
		});

		defaultDynamicPanel();
	}
	MainEvent::~MainEvent()
	{
	}

	void MainEvent::hideDynamicPanel()
	{
		menu.currentAccount->hide();
		menu.createAccount->hide();
		menu.accounts->hide();
		menu.settings->hide();
	}
	void MainEvent::setAccount(const std::shared_ptr<Res::Account> &_account)
	{
		account_ = _account;
		menu.bluePrints->setAccount(account_);
		menu.currentAccount.refresh();
		menu.blueprintInfo.refresh();
		defaultDynamicPanel();
	}
	void MainEvent::defaultDynamicPanel()
	{
		hideDynamicPanel();
		if (account_)
		{
			menu.currentAccount->show();
		}
		else if (menu.accounts.list.items->items().size())
		{
			menu.accounts->show();
		}
		else
		{
			menu.createAccount->show();
		}
	}




	void MainEvent::devGame(const std::shared_ptr<Res::BluePrint> &bluePrint_)
	{
		if (bluePrint_)
		{
			scene_ = KIR5::Shared<Scene::Developer>(bluePrint_);
			*parent << scene_;
			scene_->move(0, 0, parent->width(), parent->height());
			menu->hide();
		}
	}

	void MainEvent::playGame(const std::shared_ptr<Res::BluePrint> &bluePrint_)
	{
		if (account_ && bluePrint_)
		{
			scene_ = KIR5::Shared<Scene::Play>(bluePrint_, account_);
			*parent << scene_;
			scene_->move(0, 0, parent->width(), parent->height());
			menu->hide();
		}
	}
	void MainEvent::replayGame(const std::string &replayUuid)
	{
		scene_ = KIR5::Shared<Scene::Replay>(replayUuid);
		*parent << scene_;
		scene_->move(0, 0, parent->width(), parent->height());
		menu->hide();
	}
	void MainEvent::finishGame(bool updateUI)
	{
		menu->show();
		*parent >> scene_;
		scene_ = nullptr;

		if (updateUI)
		{
			menu.currentAccount.refresh();
			menu.blueprintInfo.refresh();
			menu.bluePrints->refresh();
		}
	}

	MainEvent::Menu::Accounts::Create::Create()
	{
		(*this)->setBitmap(Res::uielements[Res::UIElements::Add]);

		(*this)->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			s_object->menu.createAccount->show();
			s_object->menu.currentAccount->hide();
			s_object->menu.createAccount.usernameTextBox->setText("");
			s_object->menu.createAccount.passwrodTextBox->setText("");
		});
	}

	MainEvent::Menu::Accounts::List::List()
	{
		(*this)->show();

		(**this) << scrollBar;
		scrollBar->show();
		scrollBar->setThumb(scrollBarThumb);
		scrollBar->cursor(0);

		(**this) << items;
		items->cursor(0);
		items->show();

		get()->fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			items->itemHeight((*this)->height() / 5);

			items->move(0, 0, (*this)->width() - (std::max)(2.f, (*this)->width() * 0.05f), (*this)->height());
			scrollBar->move(items->virtualx2(), 0, (*this)->width() - items->virtualx2(), (*this)->height());
			scrollBar->sizeOfPage(scrollBar->height());
			scrollBar->sizeOfBook(items->itemHeight() * items->items().size());
		});
	}
}
#include "MainEvent.h"
#include "Font.h"

#include "OriginalObjects.h"
#include "Resource.h"

#include <KIR\AL\KIR5_bitmap_target.h>
#include <KIR\AL\KIR5_event_engine.h>
#include <KIR\sys\KIR5_files.h>

namespace UI
{

	MapList::Panel::Panel()
	{
		font = Res::MajorSnafu[30];

		fncDraw.push_back(FNC_DRAW([&](FNC_DRAW_PARAMS)->FNC_DRAW_RET
		{
			font.draw(x_ + 10, y_ + 10, KIR4::to_string(ID), KIR5::Color(255, 255, 0, alpha).getAlphaColored(), KIR5::LEFT);
		}));
	}

	void MapList::Panel::SetMapContainer(const std::vector<std::shared_ptr<Res::BluePrint>> *bluePrints)
	{
		this->bluePrints = bluePrints;
	}
	void MapList::Panel::SetFocus(int ID)
	{
		this->ID = ID;
		if (ID < 0 || (int)bluePrints->size() <= ID)
		{
			hide();
			setBluePrint(std::shared_ptr<Res::BluePrint>());
		}
		else
		{
			setBluePrint((*bluePrints)[ID]);
		}
	}
	void MapList::Panel::show()
	{
		if (ID >= 0 && (int)bluePrints->size() > ID)
			KIR5::Panel::show();
	}
	int MapList::Panel::GetFocus() const
	{
		return ID;
	}
	void MapList::Panel::Restart()
	{
		source.w = width();
		source.x = virtualx();
		source.y = virtualy();
		source.a = alpha;
	}

	void MapList::Panel::resize(float w)
	{
		fw = w;
		KIR5::Panel::resize(fw, fw);
	}
	void MapList::Panel::move(float x, float y)
	{
		fx += x;
		fy += y;
		KIR5::Panel::position(fx, fy);
	}
	void MapList::Panel::position(float x, float y)
	{
		fx = x;
		fy = y;
		KIR5::Panel::position(fx, fy);
	}
	float MapList::Panel::widthf()
	{
		return fw;
	}
	float MapList::Panel::virtual_xf()
	{
		return fx;
	}
	float MapList::Panel::virtual_yf()
	{
		return fy;
	}
	void MapList::Panel::align(int type)
	{
		KIR5::Panel::align(type);

		fw = width();
		fx = virtualx();
		fy = virtualy();
	}

	void MapList::SetTop()
	{
		panelp[2]->bringTop();
	}
	//balr�l jobbra
	void MapList::MoveRight()
	{
		if (MoveCount < 0)
		{
			if (Focus > 0)
			{
				Panel *tmp = panelp[4];
				for (int i = 4; i > 0; i--)
					panelp[i] = panelp[i - 1];
				panelp[0] = tmp;
				panelp[1]->show();
				panelp[0]->SetFocus(panelp[1]->GetFocus() - 1);

				for (int i = 0; i < 5; i++)
					panel[i]->Restart();
				MoveCount++;
				Focus--;
				arrived = 1;

				SetTop();
			}
			else
				MoveCount = 0;
		}
	}
	//jobbr�l balra
	void MapList::MoveLeft()
	{
		if (MoveCount > 0)
		{
			if (Focus < (int)world->bluePrints.size() - 1)
			{
				Panel
					*tmp = panelp[0];
				for (int i = 0; i < 4; i++)
					panelp[i] = panelp[i + 1];
				panelp[4] = tmp;
				panelp[3]->show();
				panelp[4]->SetFocus(panelp[3]->GetFocus() + 1);

				for (int i = 0; i < 5; i++)
					panel[i]->Restart();
				MoveCount--;
				Focus++;
				arrived = 1;

				SetTop();
			}
			else
				MoveCount = 0;
		}
	}
	void MapList::SetFocus(int Focus)
	{
		this->Focus = Focus;
		for (int i = 0; i < 5; i++)
			panelp[i]->SetFocus(Focus + (i - 2));
		MouseButtonDown = false;
	}
	MapList::MapList()
	{
		world = &Res::world1;

		for (int i = 0; i < 5; i++)
		{
			panelp[i] = panel[i].get();
			panel[i]->SetMapContainer(&world->bluePrints);
		}

		fncMoved.push_back([&](FNC_MOVED_PARAMS)
		{
			//LEFT
			panelp[0]->resize(height() * 0.6f);
			panelp[1]->resize(height() * 0.9f);
			panelp[2]->resize(height());
			panelp[3]->resize(height() * 0.9f);
			panelp[4]->resize(height() * 0.6f);
			//RIGHT

			for (int i = 0; i < 5; i++)
			{
				*this << panel[i];
				panelp[i]->show();
				panelp[i]->align(KIR5::CENTER);
			}
			//LEFT
			panelp[0]->move(-height(), 0);
			panelp[1]->move(-height() * 0.5f, 0);
			//CENTER
			panelp[3]->move(+height() * 0.5f, 0);
			panelp[4]->move(+height(), 0);
			//RIGHT

			paneldata[0].a = 0;
			paneldata[1].a = 240;
			paneldata[2].a = 255;
			paneldata[3].a = 240;
			paneldata[4].a = 0;

			for (int i = 0; i < 5; i++)
			{
				paneldata[i].x = panelp[i]->virtualx();
				paneldata[i].y = panelp[i]->virtualy();
				paneldata[i].w = panelp[i]->widthf();
			}

			for (int i = 0; i < 5; i++)
			{
				panelp[i]->Restart();
				panelp[i]->alpha = paneldata[i].a;
			}

			panelp[0]->hide();
			panelp[4]->hide();

			SetTop();
		});

		fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
		{
			if (key_ == 83)
				MoveCount++;
			if (key_ == 82)
				MoveCount--;
			return false;
		});

		fncTimer.push_back([&](FNC_TIMER_PARAMS)
		{
			if (arrived != 2)
			{
				arrived = 1;
				for (int i = 0; i < 5; i++)
				{
					float
						ret;

					if ((ret = MoveTo(paneldata[i].x, panelp[i]->source.x, panelp[i]->virtual_xf(), cps_)) == MoveToRetLast)
						panelp[i]->position(paneldata[i].x, panelp[i]->virtualy());
					else if (ret != MoveToRetZero)
						panelp[i]->move(ret, 0);

					if ((ret = MoveTo(paneldata[i].y, panelp[i]->source.y, panelp[i]->virtual_yf(), cps_)) == MoveToRetLast)
						panelp[i]->position(panelp[i]->virtualx(), paneldata[i].y);
					else if (ret != MoveToRetZero)
						panelp[i]->move(0, ret);

					if ((ret = MoveTo(paneldata[i].w, panelp[i]->source.w, panelp[i]->widthf(), cps_)) == MoveToRetLast)
						panelp[i]->resize(paneldata[i].w);
					else if (ret != MoveToRetZero)
						panelp[i]->resize(panelp[i]->widthf() + ret);

					if ((ret = MoveTo(paneldata[i].a, panelp[i]->source.a, panelp[i]->alpha, cps_)) == MoveToRetLast)
					{
						panelp[i]->alpha = paneldata[i].a;
						if (panelp[i]->alpha == 0)
							panelp[i]->hide();
					}
					else if (ret != MoveToRetZero)
						panelp[i]->alpha += ret;
				}
				if (arrived == 1)
				{
					arrived = 2;
					MoveRight();
					MoveLeft();
					if (arrived == 2)
						speed = Defspeed;
				}
			}
			else
			{
				MoveRight();
				MoveLeft();
			}
		});

		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			al_draw_rectangle(x_, y_, x_ + w_, y_ + h_, al_map_rgb(255, 0, 255), 1);
		}));

		fncMouseAxes.push_back([&](FNC_MOUSE_AXES_PARAMS)
		{
			if (MouseButtonDown)
			{
				int
					MC = (x_ - mx) / (width() * 0.9f) * world->bluePrints.size();
				MoveCount += lastMC - MC;
				lastMC = MC;
				MouseAxes = true;
			}
		});

		fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (button_ == KIR5::MOUSE_BUTTON_LEFT)
			{
				MoveCount = 0;
				MouseButtonDown = true;
				MouseAxes = false;
				mx = x_;
				lastMC = 0;
			}
			return false;
		});

		fncMouseButtonUp.push_back([&](FNC_MOUSE_BUTTON_UP_PARAMS)->FNC_MOUSE_BUTTON_UP_RET
		{
			if (button_ == KIR5::MOUSE_BUTTON_LEFT)
			{
				if (!MouseAxes)
				{
					float
						percent = (x_ - x()) / (float)(width()) * 100.f;
					if (percent >= 0.f && percent <= 35.f)
						MoveCount--;
					else if (percent <= 100.f && percent >= 65.f)
						MoveCount++;
				}
				MouseButtonDown = false;
			}
			return false;
		});
	}
	MapList::~MapList()
	{

	}

	MainEvent::Menu::Worlds::Worlds()
	{
		(*this)->addFlag(KIR5::Row<KIR5::Panel, WorldButton>::FixHeight);
		(*this)->setGap(3);

		KIR5::STRING_CONDITION::stringT worldPath = KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "worlds");
		KIR5::FindFiles ff(worldPath, "\\*.dat");
		for (auto &it : ff)
		{
			KIR5::Shared<WorldButton> worldButton = new WorldButton(KIR5::pathCombine(worldPath, it.getShortName()));
			worldButton->show();
			//worldButton->setBitmap(Bitmap::world[it.getNameOnly()]);
			(*this)->pushBack(worldButton);
		}

		get()->fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			for (auto &it : (*this)->items())
			{
				it->resize((*this)->height(), (*this)->height());
			}
		});
	}
	MainEvent::Menu::Settings::Settings()
	{

	}
	MainEvent::Menu::BluePrints::BluePrints()
	{
		(*this)->SetFocus(0);
	}
	MainEvent::Menu::Accounts::Accounts()
	{
		(**this) << list << create;
		list->show();
		create->show();

		refresh();
	}
	MainEvent::Menu::Accounts::~Accounts()
	{
	}
	void MainEvent::Menu::Accounts::refresh()
	{
		list.items->clear();
		for (auto &it : Res::accounts.list)
		{
			KIR5::Shared<UI::Button<Res::Consolas>> newAccountButton;
			newAccountButton->setText(it->username());
			list.items->pushBack(newAccountButton);
			list.scrollBar->sizeOfBook(list.items->itemHeight() * list.items->items().size());
		}
	}
	MainEvent::Menu::HirtoryOfBlueprint::HirtoryOfBlueprint()
	{

	}
	MainEvent::Menu::CurrentAccount::CurrentAccount()
	{

	}
	MainEvent::Menu::CreateAccount::CreateAccount()
	{
		(*this)->hide();
		(**this) << panelLabel << usernameTextBox << passwrodTextBox << usernameLabel << passwrodLabel << cancelButton << applyButton;
	}
	MainEvent::Menu::PlayButton::PlayButton()
	{
		(*this)->setBitmap(Res::uielements[Res::UIElements::Play]);
	}
	MainEvent::Menu::SkipButton::SkipButton()
	{
		(*this)->setBitmap(Res::uielements[Res::UIElements::Skip]);
	}


	float MapList::MoveTo(float dest, float source, float current, float cps)
	{
		float
			ret = MoveToRetZero;
		if (current < dest)
		{
			ret = std::abs((std::abs(dest) - std::abs(source))) / (speed / float(std::abs(MoveCount) + 1) * cps);
			if (current + ret >= dest)
				return MoveToRetLast;
			else
				arrived = 0;
		}
		else if (current > dest)
		{
			ret = -std::abs((std::abs(dest) - std::abs(source))) / (speed / float(std::abs(MoveCount) + 1) * cps);
			if (current + ret <= dest)
				return MoveToRetLast;
			else
				arrived = 0;
		}
		return ret;
	}

	int MapList::GetFocus() const
	{
		return Focus;
	}

	const double MapList::Defspeed = 0.5;

	std::shared_ptr<MainEvent> mainEvent;
	int maxBitmapSize;

	MainEvent::WorldButton::WorldButton(const std::string &filename):
		filename(filename)
	{

	}

	MainEvent::MainEvent(std::shared_ptr<KIR5::Panel> _parent):
		parent(_parent)
	{
		Objects::RunInitializer();

		*parent << menu << activeMap;
		menu->show();
		activeMap->hide();

		parent->fncKeyChar.push_back([&](FNC_KEY_CHAR_PARAMS)->FNC_KEY_CHAR_RET
		{
			if (key_ == 's')
			{
				activeMap->startMap(*menu.bluePrints->world->bluePrints[menu.bluePrints->GetFocus()], account);
				activeMap->show();
			}
			if (key_ == 'r')
			{
				//std::shared_ptr<ActiveMapBot> replayBot(new ActiveMapBot());
				//replayBot->load(replayTextBox->getText());
				activeMap->startMap(*menu.bluePrints->world->bluePrints[menu.bluePrints->GetFocus()], account);
				activeMap->show();
			}
			return false;
		});

		parent->fncTimer.push_back([&](FNC_TIMER_PARAMS)
		{
		});

		parent->fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(KIR5::Color(20, 20, 20));

			//Object::cpuKill();
		}));

		parent->fncMoved.push_back([&](FNC_MOVED_PARAMS)
		{
			static constexpr float rw = 500.f;
			static constexpr float rh = 400.f;
			float rate = (std::min)(parent->width() / rw, parent->height() / rh);
			menu->resize(rw * rate, rh * rate);
			menu->align(KIR5::CENTER);

			activeMap->move(0, 0, parent->width(), parent->height());
		});
		account = nullptr;
		menu.accounts.refresh();
	}
	MainEvent::~MainEvent()
	{
	}

	void MainEvent::mapFinished(bool victory, unsigned long long ticks)
	{
		menu->show();

		activeMap->hide();
	}

	MainEvent::Menu::CreateAccount::UsernameLabel::UsernameLabel()
	{
		(*this)->setText("Username:");
	}

	MainEvent::Menu::Accounts::Create::Create()
	{
		(*this)->setBitmap(Res::uielements[Res::UIElements::Add]);
		(*this)->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			mainEvent->menu.createAccount->show();
			mainEvent->menu.currentAccount->hide();
			mainEvent->menu.createAccount.usernameTextBox->setText("");
			mainEvent->menu.createAccount.passwrodTextBox->setText("");
		});
	}

	MainEvent::Menu::CreateAccount::PasswrodLabel::PasswrodLabel()
	{
		(*this)->setText("Password:");
	}

	MainEvent::Menu::CreateAccount::ApplyButton::ApplyButton()
	{
		(*this)->setText("Create");
		(*this)->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			mainEvent->menu.createAccount->hide();
			mainEvent->menu.currentAccount->show();

			const std::string &username = mainEvent->menu.createAccount.usernameTextBox->getText();
			const std::string &password = mainEvent->menu.createAccount.passwrodTextBox->getText();
			Res::Accounts::ERR_C err_c = Res::accounts.add(username, password);
			if (err_c == Res::Accounts::E_OK)
			{
				mainEvent->menu.accounts.refresh();
			}
		});
	}

	MainEvent::Menu::CreateAccount::PanelLabel::PanelLabel()
	{
		(*this)->setText("Create new account");
	}

	MainEvent::Menu::CreateAccount::CancelButton::CancelButton()
	{
		(*this)->fncPress.push_back([](FNC_PRESS_PARAMS)
		{
			mainEvent->menu.createAccount->hide();
			mainEvent->menu.currentAccount->show();
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
#include "MainEvent.h"
#include "Font.h"

#include "OriginalObjects.h"

#include <KIR\AL\KIR5_bitmap_target.h>
#include <KIR\AL\KIR5_event_engine.h>

MapList::Panel::Panel()
{
	font = Font::MajorSnafu[30];

	FncDraw.lock(fncDraw);
	FncDraw.set([&](FNC_DRAW_PARAMS)
	{
		font.draw(x_ + 10, y_ + 10, KIR4::to_string(ID), KIR5::Color(255, 255, 0, alpha).getAlphaColored(), KIR5::LEFT);
	});
}

void MapList::Panel::SetMapContainer(const std::vector<std::shared_ptr<BluePrint>> *bluePrints)
{
	this->bluePrints = bluePrints;
}
void MapList::Panel::SetFocus(int ID)
{
	this->ID = ID;
	if (ID < 0 || (int)bluePrints->size() <= ID)
	{
		hide();
		setBluePrint(std::shared_ptr<BluePrint>());
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
		if (Focus < (int)world.getBluePrints().size() - 1)
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
	for (int i = 0; i < 5; i++)
	{
		panelp[i] = panel[i].get();
		panel[i]->SetMapContainer(&world.getBluePrints());
	}

	fncMoved = [&](FNC_MOVED_PARAMS)
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
	};

	fncKeyDown = [&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
	{
		if (key_ == 83)
			MoveCount++;
		if (key_ == 82)
			MoveCount--;
		return false;
	};

	fncTimer = [&](FNC_TIMER_PARAMS)
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
	};

	fncDraw = [&](FNC_DRAW_PARAMS)
	{
		al_draw_rectangle(x_, y_, x_ + w_, y_ + h_, al_map_rgb(255, 0, 255), 1);
	};

	fncMouseAxes = [&](FNC_MOUSE_AXES_PARAMS)
	{
		if (MouseButtonDown)
		{
			int
				MC = (x_ - mx) / (width() * 0.9f) * world.getBluePrints().size();
			MoveCount += lastMC - MC;
			lastMC = MC;
			MouseAxes = true;
		}
	};

	fncMouseButtonDown = [&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
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
	};

	fncMouseButtonUp = [&](FNC_MOUSE_BUTTON_UP_PARAMS)->FNC_MOUSE_BUTTON_UP_RET
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
	};
}
MapList::~MapList()
{

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

MainEvent::MainEvent():
	KIR5::Display("Hamika", 720 + 240, 480 + 240, ALLEGRO_RESIZABLE | ALLEGRO_OPENGL)
{
	maxBitmapSize = al_get_display_option(*this, ALLEGRO_MAX_BITMAP_SIZE);
	Objects::RunInitializer();

	{
		*this << originalMapList;
		originalMapList->show();
		originalMapList->world.load("Hamika\\worlds\\YEAR_1991.dat");
		originalMapList->SetFocus(0);
	}

	{
		*this << replayTextBox;
		replayTextBox->setTextFont(Font::TimesNewRoman[16]);
		replayTextBox->setText("");
		replayTextBox->setTextColor(KIR5::Color(255, 100, 100));
		replayTextBox->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
		replayTextBox->move(0, 0, 200, replayTextBox->getTextHeight() + 5);
		replayTextBox->setColor(KIR5::Color(50, 50, 50));
		replayTextBox->show();
	}

	{
		cursorBmp.initialize(KIR5::Bitmap("Hamika\\Texture\\cursor.png"), ObjectBase::bitmapPool.get("Error"));
		cursor.resize(cursorBmp.getCount());
		for (int i = 0; i < cursorBmp.getCount(); i++)
			cursor[i] =
			al_create_mouse_cursor(cursorBmp[i],
								   0, 0);
		cursorAnimator.Initialize();
		cursorAnimator.SetNumberOfFrames(cursorBmp.getCount());
		cursorAnimator.SetAnimationTime(1.f);
	}

	activeMap->hide();
	*this << activeMap;

	fncKeyChar = [&](FNC_KEY_CHAR_PARAMS)->FNC_KEY_CHAR_RET
	{
		if (key_ == 's')
		{
			originalMapList->hide();
			activeMap->startMap(*originalMapList->world.getBluePrints()[originalMapList->GetFocus()], std::shared_ptr<ActiveMapBot>());
			activeMap->show();
		}
		if (key_ == 'r')
		{
			originalMapList->hide();
			std::shared_ptr<ActiveMapBot> replayBot(new ActiveMapBot());
			replayBot->load(replayTextBox->getText());
			activeMap->startMap(*originalMapList->world.getBluePrints()[originalMapList->GetFocus()], replayBot);
			activeMap->show();
		}
		return false;
	};

	fncTimer = [&](FNC_TIMER_PARAMS)
	{
		cursorAnimator.UpdateTimer();
		if (cursorAnimator.UpdateDrawNumber())
			al_set_mouse_cursor(*this, cursor[cursorAnimator.GetDrawNumber()]);
	};

	fncDraw = [&](FNC_DRAW_PARAMS)
	{
		al_clear_to_color(KIR5::Color(20, 20, 20));

		//Object::cpuKill();
	};

	fncMoved = [&](FNC_MOVED_PARAMS)
	{
		activeMap->move(0, 0, width(), height());
		originalMapList->resize(width() * 0.833333f, height() * 0.416666f);
		originalMapList->align(KIR5::CENTER);
	};
	fncMoved(this);
}
MainEvent::~MainEvent()
{

}

void MainEvent::mapFinished(bool victory, unsigned long long ticks)
{
	activeMap->hide();
	originalMapList->show();
}
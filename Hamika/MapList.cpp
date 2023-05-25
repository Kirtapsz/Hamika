#include "MapList.h"
#include "Tools.h"
#include "MainEvent.h"

#include <iostream>

namespace UI
{
	std::int32_t MapList::getNoOfBlueprints() const
	{
		return world_ ? world_->bluePrints.size() : 0;
	}
	void MapList::refresh()
	{
		if (account_)
		{
			static constexpr std::size_t maximumNumberOfJumps = 3;
			std::size_t numberOfJumps = maximumNumberOfJumps + 1;
			for (std::int32_t i = 0; i < getNoOfBlueprints(); ++i)
			{
				KIR5::sha512digest &_hash = world_->bluePrints[i]->hash;
				auto item = std::find_if(account_->completedBlueprints().begin(), account_->completedBlueprints().end(), [&_hash](const Res::Account::CompletedBluePrint &completedBluePrint) -> bool
				{
					return _hash == completedBluePrint.bluePrintHash();
				});
				if (item != account_->completedBlueprints().end())
				{
					worldStatus_[i] = Completed;
				}
				else if (numberOfJumps > 0)
				{
					--numberOfJumps;
					worldStatus_[i] = Available;
					if (worldStatus_[targetTransition_] == Completed && targetTransition_ < i)
					{
						targetTransition_ = i;
					}
				}
				else
				{
					break;
				}
			}
			MainEvent::s_object->menu.blueprintInfo.refresh();
		}
		else
		{
			worldStatus_.assign(worldStatus_.size(), Unavailable);
		}
		status();
	}
	void MapList::setWorld(const std::shared_ptr<Res::World> &_world)
	{
		world_ = _world;
		worldStatus_.resize(getNoOfBlueprints(), Unavailable);
		reset();
	}
	const std::shared_ptr<Res::World> &MapList::getWorld() const
	{
		return world_;
	}
	void MapList::setAccount(const std::shared_ptr<Res::Account> &_account)
	{
		account_ = _account;
		reset();
	}
	std::int32_t MapList::getIndex() const
	{
		return targetTransition_;
	}
	void MapList::setIndex(std::int32_t _index)
	{
		if (ifSync(targetTransition_, range<std::int32_t>(0, getNoOfBlueprints() - 1, _index)))
		{
			MainEvent::s_object->menu.blueprintInfo.refresh();
		}
	}

	void MapList::move()
	{
		std::size_t maxWidth = width() * 0.6f;
		std::size_t maxHeight = height() * 0.6f;

		constexpr std::float_t reduce = 0.6f;

		std::size_t minWidth = maxWidth - maxWidth * reduce;
		std::size_t minHeight = maxHeight - maxHeight * reduce;

		for (auto &it : bluePrintPanel)
		{
			if (it->index_ >= 0 && it->index_ < getNoOfBlueprints())
			{
				//KIR5::Event::Suspend fncMovedSusp(it->fncMoved);
				it->show();

				std::float_t transition = it->index_ - currentTransition_;
				std::float_t fr = transition / (float)(bluePrintPanelWingsize - 1); /* -1 ... 1 */
				it->fra = std::abs(fr); /* 1 ... 0 ... 1 */

				int w = maxWidth - maxWidth * reduce * it->fra;
				int h = maxHeight - maxHeight * reduce * it->fra;

				float fx = (std::sin(fr * 1.57f) + 1) / 2.f;
				float fy = (std::cos((fr - 1) * 3.14f) + 1) / 2.f;

				float x = fx * (width() - minWidth) + minWidth / 2.f;
				float y = fy * (height() - (minHeight + maxHeight) / 2.f) + maxHeight / 2.f;

				it->move(x - w / 2.f, y - h / 2.f, w, h);

				it->alpha = range(0.f, 255.f, (std::pow(fr, 2.f) * -0.7f/*edge state*/ + 1.2f/*middle state*/) * 255.f);
			}
			else
			{
				it->hide();
				it->fra = 1;
			}
		}

		std::sort(bluePrintPanel.begin(), bluePrintPanel.end(), [](const KIR5::Shared<S_BlueprintPanel> &left, const KIR5::Shared<S_BlueprintPanel> &right) -> bool
		{
			return left->fra > right->fra;
		});

		for (auto &it : bluePrintPanel)
		{
			it->bringTop();
			it->button->disableIteractions();
		}

		bluePrintPanel.back()->button->enableIteractions();
	}
	void MapList::sort()
	{
		std::int32_t leftIndex = middleIndex_ - bluePrintPanelWingsize;
		std::int32_t rightIndex = middleIndex_ + bluePrintPanelWingsize;

		std::sort(bluePrintPanel.begin(), bluePrintPanel.end(), [](const KIR5::Shared<S_BlueprintPanel> &left, const KIR5::Shared<S_BlueprintPanel> &right) -> bool
		{
			return left->index_ < right->index_;
		});

		for (auto it = bluePrintPanel.begin(); it != bluePrintPanel.end(); ++it)
		{
			if (leftIndex <= (*it)->index_)
			{
				break;
			}
			(*it)->index_ = rightIndex--;
		}
		for (auto it = bluePrintPanel.rbegin(); it != bluePrintPanel.rend(); ++it)
		{
			if (rightIndex == (*it)->index_)
			{
				break;
			}
			(*it)->index_ = leftIndex++;
		}

		status();
	}
	void MapList::status()
	{
		for (auto &it : bluePrintPanel)
		{
			static KIR5::Color colorCompletedButton(90, 225, 0);
			static KIR5::Color colorAvailableButton(24, 30, 138);
			static KIR5::Color colorUnavailableButton(255, 40, 40);

			static KIR5::Color colorCompletedPrint(255, 255, 255);
			static KIR5::Color colorAvailablePrint(70, 159, 231);
			static KIR5::Color colorUnavailablePrint(248, 81, 81);

			it->bluePrint(getByIndex(it->index_));

			switch (getStatus(it->index_))
			{
				case Completed:
				{
					it->button->setBitmap(Res::uielements[Res::UIElements::Play]);
					it->button->setBitmapColor(colorCompletedButton);
					it->color(colorCompletedPrint);
					break;
				}
				case Available:
				{
					it->button->setBitmap(Res::uielements[Res::UIElements::Play]);
					it->button->setBitmapColor(colorAvailableButton);
					it->color(colorAvailablePrint);
					break;
				}
				case Unavailable:
				{
					it->button->setBitmap(Res::uielements[Res::UIElements::Locked]);
					it->button->setBitmapColor(colorUnavailableButton);
					it->color(colorUnavailablePrint);
					break;
				}
			}
		}
	}

	std::shared_ptr<Res::BluePrint> MapList::getBluePrint() const
	{
		return getByIndex(targetTransition_);
	}
	std::shared_ptr<Res::BluePrint> MapList::getByIndex(std::int32_t _index) const
	{
		if (_index >= 0 && _index < static_cast<std::int32_t>(world_->bluePrints.size()))
		{
			return world_->bluePrints[_index];
		}
		else
		{
			return std::shared_ptr<Res::BluePrint>();
		}
	}
	void MapList::reset()
	{
		targetTransition_ = 0;
		if (world_)
		{
			currentTransition_ = 0;
			middleIndex_ = 0;
			std::int32_t _index = middleIndex_ - bluePrintPanelWingsize;
			for (auto &it : bluePrintPanel)
			{
				it->index_ = _index++;
				it->bluePrint(getByIndex(it->index_));
			}
		}
		else
		{
			for (auto &it : bluePrintPanel)
			{
				it->index_ = -1;
			}
		}
		refresh();
		move();
	}
	MapList::Status MapList::getStatus(std::int32_t index_) const
	{
		if (index_ >= 0 && index_ < getNoOfBlueprints())
		{
			return worldStatus_[index_];
		}
		else
		{
			return Unavailable;
		}
	}
	MapList::MapList()
	{
		auto actionButton = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			S_BlueprintPanel *ptr = dynamic_cast<S_BlueprintPanel *>(obj_->getParent());
			if (getStatus(ptr->index_) != Unavailable)
			{
				MainEvent::s_object->playGame(getByIndex(ptr->index_));
			}
		};
#ifdef _DEBUG
		auto devActionPress = [&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (button_ == KIR5::MOUSE_BUTTON_RIGHT)
			{
				S_BlueprintPanel *ptr = dynamic_cast<S_BlueprintPanel *>(obj_);
				if (ptr)
				{
					if (ptr->onPanel(x_, y_))
					{
						MainEvent::s_object->devGame(getByIndex(ptr->index_));
						return true;
					}
				}
			}
			return false;
		};
#endif

		auto actionPress = [&](FNC_KEY_CHAR_PARAMS)->FNC_KEY_CHAR_RET
		{
			if (key_ == 13)
			{
				S_BlueprintPanel *ptr = dynamic_cast<S_BlueprintPanel *>(obj_->getParent());
				if (getStatus(ptr->index_) != Unavailable)
				{
					MainEvent::s_object->playGame(getByIndex(ptr->index_));
					return true;
				}
			}
			return false;
		};

		auto setColor = [&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
		{
			S_BlueprintPanel *ptr = dynamic_cast<S_BlueprintPanel *>(obj_);
			if (ptr->setColor)
			{
				ptr->setColor = false;
				ptr->color(KIR5::Color(255, 255, 255, ptr->alpha));
			}
		};

		auto jumpOnIndex = [&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
		{
			S_BlueprintPanel *ptr = dynamic_cast<S_BlueprintPanel *>(obj_);

			if (getIndex() != ptr->index_)
			{
				mouseOnHold = false;
				setIndex(ptr->index_);
			}
		};

		for (auto &it : bluePrintPanel)
		{
			*this << it;
			*it << it->button;
			it->button->show();
			it->button->fncPress.push_back(actionButton);
#ifdef _DEBUG
			it->fncMouseButtonDown.push_back(devActionPress);
#endif
			it->button->fncKeyChar.push_back(actionPress);
			it->fncPress.push_back(jumpOnIndex);
		}

		fncMouseButtonDown.push_back([&](FNC_MOUSE_BUTTON_DOWN_PARAMS)->FNC_MOUSE_BUTTON_DOWN_RET
		{
			if (button_ == KIR5::MOUSE_BUTTON_LEFT)
			{
				if (onPanel(x_, y_))
				{
					mouseHoldX = x_;
					mouseOnHold = true;
					mouseHoldIndex = targetTransition_;
					return true;
				}
			}
			return false;
		});

		fncMouseButtonUp.push_back([&](FNC_MOUSE_BUTTON_UP_PARAMS)->FNC_MOUSE_BUTTON_UP_RET
		{
			if (mouseOnHold)
			{
				if (mouseHoldIndex == targetTransition_)
				{
					std::float_t clickSide = (x_ - x()) / float(width());
					if (clickSide < 0.40)
					{
						setIndex(targetTransition_ - 1);
					}
					else if (clickSide > 0.60)
					{
						setIndex(targetTransition_ + 1);
					}
				}
				mouseOnHold = false;
				return true;
			}
			return false;
		});

		fncMouseAxes.push_back([&](FNC_MOUSE_AXES_PARAMS)->FNC_MOUSE_AXES_RET
		{
			if (mouseOnHold)
			{
				setIndex(mouseHoldIndex + ((mouseHoldX - x_) / float(width()) / 2.f * getNoOfBlueprints()));
			}
		});

		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			move();
		});

		fncKeyDown.push_back([&](FNC_KEY_DOWN_PARAMS)->FNC_KEY_DOWN_RET
		{
			if (key_ == 83)
			{
				setIndex(targetTransition_ + 1);
			}
			if (key_ == 82)
			{
				setIndex(targetTransition_ - 1);
			}
			return false;
		});

		fncTimer.push_back([&](FNC_TIMER_PARAMS)->FNC_TIMER_RET
		{
			constexpr float c_min_speed = 0.1f;
			if (currentTransition_ != targetTransition_)
			{
				if (currentTransition_ < targetTransition_)
				{
					currentTransition_ += CPStime<std::float_t>((std::max)(c_min_speed, (targetTransition_ - currentTransition_) / 8.f), cps_, 60.f);
					if (currentTransition_ > targetTransition_)
					{
						currentTransition_ = targetTransition_;
					}
				}
				else
				{
					currentTransition_ -= CPStime<std::float_t>((std::max)(c_min_speed, (currentTransition_ - targetTransition_) / 8.f), cps_, 60.f);
					if (currentTransition_ < targetTransition_)
					{
						currentTransition_ = targetTransition_;
					}
				}
				if (ifSync(middleIndex_, currentTransition_ + 0.5f))
				{
					sort();
				}
				move();
			}
		});
	}
}
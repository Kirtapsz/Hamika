#include "Tools.h"
#include "EditorSaveWorldDialog.h"
#include "EditorMainEvent.h"
#include "World.h"

#include <KIR/sys/KIR5_files.h>
#include <KIR/sys/KIR5_system.h>
#include <KIR/AL/KIR5_panel_control.h>

namespace UI::Editor
{
	SaveWorldDialog::SaveWorldDialog()
	{
		centerBox->show();
		centerBox->resize(400, 300);
		centerBox->fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			al_clear_to_color(KIR5::Color(20, 20, 20));
		}));

		KIR5::Shared<KIR5::Column<KIR5::Panel>> col;
		col->setGap(3);
		col->show();

		{
			KIR5::Shared<KIR5::Row<KIR5::Panel>> row;
			row->setGap(3);
			row->show();

			directoryLabel->setTextFont(Res::TimesNewRoman[14]);
			directoryLabel->setTextColor(KIR5::Color(100, 100, 100));
			directoryLabel->setTextAlignment(KIR5::CENTER);
			directoryLabel->show();
			directoryLabel->setText("Directory: ");
			directoryLabel->resize((std::max)(wordNameDirectoryLabel->getTextWidth(), directoryLabel->getTextWidth()), 26);
			row->pushBack(directoryLabel);
			directoryLabel->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				std::string directory_;
				{
					KIR5::CurrentDirectoryGuard<> dirGuard;
					directory_ = KIR5::browseForFolder();
				}
				if (directory_.length())
				{
					directory->setText(directory_);
				}
			});

			directory->setTextFont(Res::Consolas[12]);
			directory->show();
			directory->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
			directory->setTextColor(KIR5::Color(230, 120, 40));
			directory->resize(300, 26);
			row->pushBack(directory);

			col->pushBack(row);
		}

		{
			KIR5::Shared<KIR5::Row<KIR5::Panel>> row;
			row->setGap(3);
			row->show();

			wordNameDirectoryLabel->setTextFont(Res::TimesNewRoman[14]);
			wordNameDirectoryLabel->setTextColor(KIR5::Color(100, 100, 100));
			wordNameDirectoryLabel->setTextAlignment(KIR5::CENTER);
			wordNameDirectoryLabel->show();
			wordNameDirectoryLabel->setText("Word name: ");
			wordNameDirectoryLabel->resize((std::max)(wordNameDirectoryLabel->getTextWidth(), directoryLabel->getTextWidth()), 26);
			row->pushBack(wordNameDirectoryLabel);

			wordNameDirectory->setTextFont(Res::Consolas[12]);
			wordNameDirectory->show();
			wordNameDirectory->setTextAlignment(KIR5::LEFT | KIR5::VCENTER);
			wordNameDirectory->setTextColor(KIR5::Color(230, 120, 40));
			wordNameDirectory->resize(300, 26);
			row->pushBack(wordNameDirectory);

			col->pushBack(row);
		}

		{
			KIR5::Shared<KIR5::Row<KIR5::Panel>> row;
			row->setGap(3);
			row->show();

			mapTypeSelector->resize(130, 30);
			mapTypeSelector->setTextFont(Res::TimesNewRoman[21]);
			mapTypeSelector->setText("Select type");
			mapTypeSelector->setTextColor(KIR5::Color(152, 152, 152));
			mapTypeSelector->show();
			mapTypeSelector->setTextAlignment(KIR5::CENTER);
			mapTypeSelector->setColor(KIR5::Color(30, 30, 30));
			mapTypeSelector->listPanel->height(32 * 3);
			mapTypeSelector->listPanel->itemHeight(22);
			row->pushBack(mapTypeSelector);

			std::apply([&](auto&&... args)
			{
				(([&](const std::string &name)
				{
					KIR5::Shared<KIR5::FramedRectangleButton<KIR5::Button<KIR5::TextButton<>>>> btn;

					btn->setTextFont(Res::TimesNewRoman[11]);
					btn->setText(name);
					btn->setTextColor(KIR5::Color(152, 152, 152));
					btn->show();
					btn->setTextAlignment(KIR5::CENTER);
					btn->setColor(KIR5::Color(30, 30, 30));

					mapTypeSelector->listPanel->pushBack(btn);
				}(args.name)), ...);
			}, Res::World::handlers);

			col->pushBack(row);
		}

		{
			KIR5::Shared<KIR5::Row<KIR5::Panel>> row;
			row->setGap(3);
			row->show();

			saveButton->setTextFont(Res::TimesNewRoman[14]);
			saveButton->setTextColor(KIR5::Color(100, 100, 100));
			saveButton->setTextAlignment(KIR5::CENTER);
			saveButton->show();
			saveButton->setColor(KIR5::Color(50, 50, 50));
			saveButton->setText("Save");
			saveButton->resize(saveButton->getTextWidth() + 16, 26);
			saveButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				std::apply([&](auto&&... args)
				{
					(([&](auto &handler)
					{
						if (handler.name == mapTypeSelector->getText())
						{
							Res::World world{KIR5::pathCombine<>(directory->getText(), wordNameDirectory->getText())};

							world.title = mainEvent->worldi->worltTitle_TextBox->getText();

							auto &oBluePrints = mainEvent->worldi->list->items();
							world.bluePrints.resize(oBluePrints.size());
							for (std::size_t i = 0; i < oBluePrints.size(); ++i)
							{
								world.bluePrints[i] = oBluePrints[i]->getBluePrint();
							}

							KIR5::DynamicStream stream;
							if (SaveResource(world, stream, handler))
							{
								std::string aPath = KIR5::pathCombine(KIR5::getModuleDirectory(), world.path);
								if (WriteFile(aPath, stream))
								{

								}
							}
						}
					}(args)), ...);
				}, Res::World::handlers);

				hide();
			});
			row->pushBack(saveButton);

			cancelButton->setTextFont(Res::TimesNewRoman[14]);
			cancelButton->setTextColor(KIR5::Color(100, 100, 100));
			cancelButton->setTextAlignment(KIR5::CENTER);
			cancelButton->show();
			cancelButton->setColor(KIR5::Color(50, 50, 50));
			cancelButton->setText("Cancel");
			cancelButton->resize(cancelButton->getTextWidth() + 16, 26);
			cancelButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				hide();
			});
			row->pushBack(cancelButton);

			*centerBox << row;
			row->align(KIR5::RIGHT | KIR5::BOTTOM);
		}

		*centerBox << col;
		col->position(3, 3);

		*this << centerBox;
		fncDraw.push_back(KIR5::Event::FNC_DRAW([&](FNC_DRAW_PARAMS)
		{
			al_draw_filled_rectangle(x_, y_, w_, h_, KIR5::Color(0, 0, 0, 127).getAlphaColored());
		}));

		fncMoved.push_back([&](FNC_MOVED_PARAMS)
		{
			centerBox->align(KIR5::CENTER);
		});
	}
	SaveWorldDialog::~SaveWorldDialog()
	{

	}
	void SaveWorldDialog::show()
	{
		Panel::show();
		bringTop();

		directory->setText(KIR5::pathCombine(KIR5::getModuleDirectory(), "Hamika", "worlds"));
		wordNameDirectory->setText("word.dat");
	}
}
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

		KIR5::Shared<KIR5::Column<Panel>> col;
		col->setGap(3);
		col->show();

		{
			KIR5::Shared<KIR5::Row<Panel>> row;
			row->setGap(3);
			row->show();

			directoryLabel->setText("Directory: ");
			directoryLabel->height(26);
			directoryLabel->width((std::max)(wordNameDirectoryLabel->getTextWidth(), directoryLabel->getTextWidth()));
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

			directory->show();
			directory->resize(300, 26);
			row->pushBack(directory);

			col->pushBack(row);
		}

		{
			KIR5::Shared<KIR5::Row<Panel>> row;
			row->setGap(3);
			row->show();

			wordNameDirectoryLabel->setText("Word name: ");
			wordNameDirectoryLabel->height(26);
			wordNameDirectoryLabel->width((std::max)(wordNameDirectoryLabel->getTextWidth(), directoryLabel->getTextWidth()));
			row->pushBack(wordNameDirectoryLabel);

			wordNameDirectory->resize(300, 26);
			row->pushBack(wordNameDirectory);

			col->pushBack(row);
		}

		{
			KIR5::Shared<KIR5::Row<Panel>> row;
			row->setGap(3);
			row->show();

			mapTypeSelector->resize(130, 30);
			mapTypeSelector->setText("Select type");
			mapTypeSelector->listPanel->height(32 * 3);
			mapTypeSelector->listPanel->itemHeight(22);
			row->pushBack(mapTypeSelector);

			std::apply([&](auto&&... args)
			{
				(([&](const std::string &name)
				{
					KIR5::Shared<decltype(mapTypeSelector->listPanel)::element_type::item_t> btn;
					btn->setText(name);
					mapTypeSelector->listPanel->pushBack(btn);
				}(args.name)), ...);
			}, Res::World::handlers);

			jsonFormat->resize(130, 30);
			jsonFormat->buttonSign->setText("Select type");
			jsonFormat->buttonSquare->setBackgroundColor(KIR5::Color(255, 0, 0));
			jsonFormat->buttonSquare->setPinColor(KIR5::Color(0, 255, 0));
			row->pushBack(jsonFormat);

			col->pushBack(row);
		}

		{
			KIR5::Shared<KIR5::Row<Panel>> row;
			row->setGap(3);
			row->show();

			saveButton->setText("Save");
			saveButton->height(26);
			saveButton->width(saveButton->getTextWidth() + 16);
			saveButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				std::apply([&](auto&&... args)
				{
					(([&](auto &handler)
					{
						if (handler.name == mapTypeSelector->getText())
						{
							Res::World world{KIR5::pathCombine<>(directory->getText(), wordNameDirectory->getText())};

							world.title = MainEvent::s_object->worldi->worltTitle_TextBox->getText();

							auto &oBluePrints = MainEvent::s_object->worldi->list->items();
							world.bluePrints.resize(oBluePrints.size());
							for (std::size_t i = 0; i < oBluePrints.size(); ++i)
							{
								world.bluePrints[i] = oBluePrints[i]->getBluePrint();
							}

							if (jsonFormat->isChecked())
							{
								try
								{
									Json json;
									Res::Json::SaveResource(world, json, handler);
									std::string aPath = KIR5::pathCombine(KIR5::getModuleDirectory(), world.path);
									std::ofstream file(aPath);
									file << std::setw(4) << json << std::endl;
								}
								catch (const std::exception &e)
								{
									std::cout << "Failed to save resource: " << e.what() << std::endl;
								}
							}
							else
							{
								try
								{
									KIR5::DynamicStream stream;
									SaveResource(world, stream, handler);
									std::string aPath = KIR5::pathCombine(KIR5::getModuleDirectory(), world.path);
									if (WriteFile(aPath, stream))
									{

									}
								}
								catch (const std::exception &e)
								{
									std::cout << "Failed to save resource: " << e.what() << std::endl;
								}
							}
						}
					}(args)), ...);
				}, Res::World::handlers);

				hide();
			});
			row->pushBack(saveButton);

			cancelButton->setText("Cancel");
			cancelButton->height(26);
			cancelButton->width(cancelButton->getTextWidth() + 16);
			cancelButton->fncPress.push_back([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
			{
				hide();
			});
			row->pushBack(cancelButton);

			*centerBox << row;
			row->align(KIR5::RIGHT | KIR5::BOTTOM, -3, -3);
		}

		*centerBox << col;
		col->position(3, 3);

		*this << centerBox;
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
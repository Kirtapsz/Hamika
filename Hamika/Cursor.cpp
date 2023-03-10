#include "Cursor.h"

Cursor::Cursor(std::shared_ptr<KIR5::Display> &_display):
	display(_display)
{
	Res::Slides slides;
	slides.initialize(Res::uielements[Res::UIElements::Cursor], Res::bitmapBox);
	cursor.resize(slides.getCount());
	for (std::size_t i = 0; i < cursor.size(); i++)
	{
		cursor[i] = al_create_mouse_cursor(slides[i], 0, 0);
	}
	cursorAnimator.Initialize();
	cursorAnimator.SetNumberOfFrames(static_cast<std::int8_t>(cursor.size()));
	cursorAnimator.SetAnimationTime(1.f);

	fncTimer.push_back([&](FNC_TIMER_PARAMS)
	{
		cursorAnimator.UpdateTimer();
		if (cursorAnimator.UpdateDrawNumber())
		{
			al_set_mouse_cursor(*display, cursor[cursorAnimator.GetDrawNumber()]);
		}
	});
}
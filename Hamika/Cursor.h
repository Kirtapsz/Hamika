#pragma once

#include <KIR\AL\KIR5_event_engine.h>

#include "Bitmap.h"
#include "BaseFunctions.h"

class Cursor: public KIR5::Event
{
	private: std::shared_ptr<KIR5::Display> display;
	private: std::vector<ALLEGRO_MOUSE_CURSOR *> cursor;
	private: Object::Animator::EntityData cursorAnimator;

	public: Cursor(std::shared_ptr<KIR5::Display> &_display);
};
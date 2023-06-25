#include "SceneBackground.h"

namespace UI::Scene
{
	void SingleSeamlessBackground::setBitmap(const KIR5::Bitmap &_bitmap, float _scale)
	{
		source = _bitmap;
		source_scale = _scale;
		redraw = true;
	}

	void SingleSeamlessBackground::setCamera(Type::Camera _camera)
	{
		original_camera = _camera;
		camera = {_camera.x * camera_scale, _camera.y * camera_scale};

		camera_shift = camera - camera_snapshot;

		if (std::abs(camera_shift.x) > buffer_size_c ||
			std::abs(camera_shift.y) > buffer_size_c)
		{
			redraw = true;
		}
	}
	void SingleSeamlessBackground::setCameraScale(float _camera_scale)
	{
		camera_scale = _camera_scale;
		setCamera(original_camera);
		redraw = true;
	}

	Type::Camera SingleSeamlessBackground::getCamera() const
	{
		return camera;
	}

	SingleSeamlessBackground::SingleSeamlessBackground()
	{
		fncDraw.push_back([&](FNC_DRAW_PARAMS) -> FNC_DRAW_RET
		{
			if (redraw)
			{
				redraw = false;

				camera_snapshot = camera;
				camera_shift = {0,0};

				Type::_Dim<__int32> source_dim{
					__int32(source.width() * source_scale),
					__int32(source.height() * source_scale)};
				Type::_Dim<__int32> buffer_dim{
					buffer.width(),
					buffer.height()};
				Type::_Pos<__int32> draw_shift{
					__int32(camera.x - 0.5f) % source_dim.width,
					__int32(camera.y - 0.5f) % source_dim.height};
				Type::_Dim<__int32> hangs_dim{
					buffer_dim.width - std::abs(draw_shift.x),
					buffer_dim.height - std::abs(draw_shift.y)};
				Type::_Pos<__int32> count{
					__int32(std::ceil((buffer_dim.width + hangs_dim.width) / (float)source_dim.width)),
					__int32(std::ceil((buffer_dim.height + hangs_dim.height) / (float)source_dim.height))};

				KIR5::BitmapTarget target(buffer);
				al_hold_bitmap_drawing(true);
				for (int x = 0; x < count.x; ++x)
				{
					for (int y = 0; y < count.y; ++y)
					{
						source.drawScaled(x * source_dim.width - (source_dim.width - draw_shift.x) - buffer_size_c,
										  y * source_dim.height - (source_dim.height - draw_shift.y) - buffer_size_c,
										  source_dim.width,
										  source_dim.height);
					}
				}
				al_hold_bitmap_drawing(false);
			}

			buffer.draw(x_ + camera_shift.x - buffer_size_c, y_ + camera_shift.y - buffer_size_c, 0);
		});

		fncMoved.push_back([&](FNC_MOVED_PARAMS) -> FNC_MOVED_RET
		{
			buffer.resize(width() + buffer_size_c * 2, height() + buffer_size_c * 2);
			redraw = true;
		});
	}
}
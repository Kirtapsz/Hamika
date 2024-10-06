#ifndef MAP_DRAWNER_HPP__
#define MAP_DRAWNER_HPP__

// https://www.allegro.cc/manual/5/al_hold_bitmap_drawing

#include "SceneDrawer.h"
#include "Font.h"
#include "Tools.h"

#include <KIR\AL\KIR5_color.h>
#include <KIR\AL\KIR5_blender.h>
#include <KIR\KIR4_console.h>


template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::setGlobalGravity(bool _global_gravity)
{
	global_gravity_ = _global_gravity;
	global_gravity_updated_ = true;
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::setMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &_map)
{
	map_ = _map;
}

template <typename ACTIVE_BLOCK_T>
Type::Pixels BlocksDrawer<ACTIVE_BLOCK_T>::getBlockSize() const
{
	return block_size_;
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::setBlockSize(Type::Size _camera_size, Type::CameraSize _block_size_adjust)
{
	if (_block_size_adjust > 0.f)
	{
		Type::CameraSize ratio = _camera_size / _block_size_adjust;
		Type::CameraSize::base new_size = ratio.max();
		block_size_ = new_size;
	}
	else
	{
		block_size_ = blockSizeInPixel;
	}
}

template <typename ACTIVE_BLOCK_T>
Type::Pixels BlocksDrawer<ACTIVE_BLOCK_T>::getBitmapSize() const
{
	return bitmap_size_;
}

template <typename ACTIVE_BLOCK_T>
Type::Pixels BlocksDrawer<ACTIVE_BLOCK_T>::getDrawBoxOffset() const
{
	return draw_box_offset_;
}
template <typename ACTIVE_BLOCK_T>
Type::rec2D<Type::Coord> BlocksDrawer<ACTIVE_BLOCK_T>::getDrawBox() const
{
	return draw_box_;
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::enableLayer(unsigned int _layer_id, bool _enabled)
{
	layers_[_layer_id].enable_ = _enabled;
}
template <typename ACTIVE_BLOCK_T>
bool BlocksDrawer<ACTIVE_BLOCK_T>::isLayerEnabled(unsigned int _layer_id) const
{
	return layers_[_layer_id].enable_;
}

template <typename ACTIVE_BLOCK_T>
BlocksDrawer<ACTIVE_BLOCK_T>::BlocksDrawer()
{
	gravity_slides_.initialize(Res::tiles[Res::Tiles::_Gravitation], Res::tiles[Res::BitmapPool::Fallback]);
	gravity_animator_.Initialize();
	gravity_animator_.SetNumberOfFrames(gravity_slides_.getCount());
	gravity_animator_.SetAnimationTime(1.0f);
}

template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::updateConfiguration(Type::Size _number_of_blocks)
{
	draw_box_ = {{0,0},{0,0}};

	block_count_ = _number_of_blocks;
	block_count_.limit({0,0}, map_->size());
	bitmap_size_ = block_count_ * block_size_;

	for (auto &layer : layers_)
	{
		layer.bitmap_.resize(bitmap_size_.w(), bitmap_size_.h());
	}
	bitmap_temp.resize(bitmap_size_.w(), bitmap_size_.h());

	map_->foreach([&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		block.object->configureDrawOptions();
		block.remain->configureDrawOptions();
		block.redrawn_type_ = RedrawType::All;
	});
}

template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::shiftBlocks(Type::Coord box_shift)
{
	Type::Pixels draw_offset = box_shift * block_size_;

	for (auto &layer : layers_)
	{
		auto target_lock = bitmap_temp.target();
		al_clear_to_color(KIR5::Color::_transparent);
		layer.bitmap_.draw(draw_offset.x(), draw_offset.y());
		layer.bitmap_.swap(bitmap_temp);
	}
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::checkForDrawRequests()
{
	map_->forrange(draw_box_edges_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (
			(block.object->isExists && block.object->requests.draw) ||
			(block.remain->isExists && block.remain->requests.draw)
			)
		{
			block.object->requests.draw = false;
			block.remain->requests.draw = false;
			setRedrawOnBlock(coord);
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::fadeBitmap(KIR5::Bitmap &_bitmap)
{
	auto target_lock = _bitmap.target();
	KIR5::Blender blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);
	al_draw_filled_rectangle(0, 0, _bitmap.width(), _bitmap.height(), KIR5::Color(10, 10, 10, 10));
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::clearBlocks()
{
	auto target_lock = layers_[SceneLayer::blocks].bitmap_.clippingTarget();
	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		block.drawned_type_ = DrawnedType::None;
		if (block.redrawn_type_ == RedrawType::Object)
		{
			block.drawned_cycle_ = total_draw_cycle_++;
			block.drawned_type_ |= DrawnedType::Cleared;
			Type::Pixels draw_point = (coord - draw_box_.p1) * block_size_;
			target_lock.target.clipping.lock(draw_point.x(), draw_point.y(), block_size_.w(), block_size_.h());
			al_clear_to_color(KIR5::Color::_transparent);
		}
	});

	fadeBitmap(layers_[SceneLayer::erases].bitmap_);
	target_lock.target.lock(layers_[SceneLayer::erases].bitmap_);
	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.drawned_type_ & DrawnedType::Cleared)
		{
			Type::rec2D<Type::Pixels> draw_rec;
			draw_rec.p1 = (coord - draw_box_.p1) * block_size_;
			draw_rec.p2 = draw_rec.p1 + block_size_;
			al_draw_filled_rectangle(draw_rec.p1.x(), draw_rec.p1.y(), draw_rec.p2.x(), draw_rec.p2.y(), KIR5::Color(0, 255, 0, 20));
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::clearRedrawIndicator()
{
	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.redrawn_type_)
		{
			block.redrawn_type_ = RedrawType::None;
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::clearDrawnedIndicator()
{
	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.drawned_type_)
		{
			if (block.drawned_type_ & DrawnedType::Drawned)
			{
				block.object->DrawnedCount = 0;
				if (block.remain->isExists)
				{
					block.remain->DrawnedCount = 0;
				}
			}
			block.drawned_type_ = DrawnedType::None;
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::clearDrawCountIndicator()
{
	map_->forrange(draw_box_edges_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.redrawn_type_)
		{
			block.object->DrawnedCount = 0;
			if (block.remain->isExists)
			{
				block.remain->DrawnedCount = 0;
			}
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::drawBitmapOfRefresh()
{
	fadeBitmap(layers_[SceneLayer::refresh].bitmap_);
	auto target_lock = layers_[SceneLayer::refresh].bitmap_.target();

	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.drawned_type_ & DrawnedType::Drawned)
		{
			Type::rec2D<Type::Pixels> draw_rec;
			draw_rec.p1 = (coord - draw_box_.p1) * block_size_;
			draw_rec.p2 = draw_rec.p1 + block_size_;
			al_draw_filled_rectangle(draw_rec.p1.x(), draw_rec.p1.y(), draw_rec.p2.x(), draw_rec.p2.y(), KIR5::Color(255, 0, 0, 20));
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::drawBitmapOfCoords()
{
	char buffer[32];

	KIR5::Font font = Res::Consolas[block_size_.height() / 6];

	auto target_lock = layers_[SceneLayer::coord].bitmap_.target();
	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.redrawn_type_ & RedrawType::Shift)
		{
			Type::Pixels p = (coord - draw_box_.p1) * block_size_;

			sprintf_s(buffer, "(%03d;%03d)", coord.x(), coord.y());

			font.draw(p.x() + 1, p.y() + 1, buffer, al_map_rgb(0, 0, 0), KIR5::LEFT);
			font.draw(p.x(), p.y(), buffer, al_map_rgb(255, 255, 255), KIR5::LEFT);

			sprintf_s(buffer, "(%03d / %03d)", block.object->id, block.remain->id);
			font.draw(p.x() + 1, p.y() + 1 + (block_size_.height() / 6), buffer, al_map_rgb(0, 0, 0), KIR5::LEFT);
			font.draw(p.x(), p.y() + (block_size_.height() / 6), buffer, al_map_rgb(255, 255, 255), KIR5::LEFT);
		}
	});
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::drawGravity()
{
	auto clear_wrapper = [&](std::function<void()> _fnc)
	{
		auto target_lock = layers_[SceneLayer::gravity].bitmap_.target();
		al_clear_to_color(KIR5::Color::_transparent);
		al_hold_bitmap_drawing(true);
		_fnc();
		al_hold_bitmap_drawing(false);
	};
	auto update_wrapper = [&](std::function<void()> _fnc)
	{
		auto target_lock = layers_[SceneLayer::gravity].bitmap_.target();
		al_hold_bitmap_drawing(true);
		_fnc();
		al_hold_bitmap_drawing(false);
	};

	gravity_animator_.UpdateTimer();
	if (global_gravity_)
	{
		if (gravity_animator_.UpdateDrawNumber() || global_gravity_updated_)
		{
			clear_wrapper([&]()
			{
				map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
				{
					Type::Pixels p = (coord - draw_box_.p1) * block_size_;
					gravity_slides_[gravity_animator_.GetDrawNumber()].drawScaled(p.x(), p.y(), block_size_.w(), block_size_.h());
				});
			});
		}
		else
		{
			update_wrapper([&]()
			{
				map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
				{
					if (block.redrawn_type_ & RedrawType::Shift)
					{
						Type::Pixels p = (coord - draw_box_.p1) * block_size_;
						gravity_slides_[gravity_animator_.GetDrawNumber()].drawScaled(p.x(), p.y(), block_size_.w(), block_size_.h());
					}
				});
			});
		}
	}
	else
	{
		if (gravity_animator_.UpdateDrawNumber() || global_gravity_updated_)
		{
			clear_wrapper([&]()
			{
				map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
				{
					if (block.grid & GridFlags::Gravity)
					{
						Type::Pixels p = (coord - draw_box_.p1) * block_size_;
						gravity_slides_[gravity_animator_.GetDrawNumber()].drawScaled(p.x(), p.y(), block_size_.w(), block_size_.h());
					}
				});
			});
		}
		else
		{
			update_wrapper([&]()
			{
				map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
				{
					if (block.grid & GridFlags::Gravity && block.redrawn_type_ & RedrawType::Shift)
					{
						Type::Pixels p = (coord - draw_box_.p1) * block_size_;

						gravity_slides_[gravity_animator_.GetDrawNumber()].drawScaled(p.x(), p.y(), block_size_.w(), block_size_.h());
					}
				});
			});
		}
	}

	if (global_gravity_updated_)
	{
		global_gravity_updated_ = false;
	}
}

template <typename ACTIVE_BLOCK_T>
template <int DRAW_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::drawObjects()
{
	using TL = KIR5::Bitmap::TargetLock<KIR5::Bitmap::ClippingTarget<KIR5::Bitmap::Target, KIR5::Bitmap::Clipping>>;
	TL target_lock = layers_[SceneLayer::blocks].bitmap_.clippingTarget();

	struct ClippingRect
	{
		Type::rec2D<Type::Pixels> _rec;
		bool requested_;
		const Type::Coord &coord_;
		const ACTIVE_BLOCK_T &block_;
		const BlocksDrawer<ACTIVE_BLOCK_T> &sceen_drawer_;
		TL &target_lock_;
		ClippingRect(TL &_target_lock, const BlocksDrawer<ACTIVE_BLOCK_T> &_sceen_drawer, const Type::Coord &_coord, const ACTIVE_BLOCK_T &_block):
			target_lock_(_target_lock), sceen_drawer_(_sceen_drawer), requested_(false), coord_(_coord), block_(_block)
		{

		}
		~ClippingRect()
		{
			reset();
		}

		bool clip()
		{
			if (!target_lock_.target.clipping.isLocked())
			{
				if (!requested_)
				{
					requested_ = true;
					_rec = sceen_drawer_.getObjectRect(coord_, block_);
				}
				if (_rec.hasArea())
				{
					target_lock_.target.clipping.lock(_rec.x() - sceen_drawer_.draw_box_offset_.width(), _rec.y() - sceen_drawer_.draw_box_offset_.height(), _rec.width(), _rec.height());
					return true;
				}
			}
			return false;
		}
		void reset()
		{
			target_lock_.target.clipping.unlock();
		}
	};

	struct DrawConfig
	{
		ACTIVE_BLOCK_T::OBJECT_T *obj_;
		bool draw_;
		bool clipping_;
		int draw_type_;
	};

	auto do_draw_configs = [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block, const auto &draw_configs)
	{
		ClippingRect clipping_rect{target_lock, *this, coord, block};

		for (auto &draw_config : draw_configs)
		{
			if (draw_config.draw_ && draw_config.obj_->isExists)
			{
				if (draw_config.clipping_)
				{
					if (!clipping_rect.clip()) continue;
				}
				else
				{
					clipping_rect.reset();
				}

				Type::Coord draw_coord = draw_config.obj_->DrawCoord - draw_box_offset_;
				draw_config.obj_->Draw(draw_coord, block_size_);
				draw_config.obj_->DrawNumber = total_object_draw_count_++;
				block.drawned_type_ |= draw_config.draw_type_;
			}
		}
	};

	map_->forrange(draw_box_, [&](const Type::Coord &coord, ACTIVE_BLOCK_T &block)
	{
		if (block.redrawn_type_ & RedrawType::Object)
		{
			if constexpr (DRAW_T == TOP_DRAW)
			{
				const std::array<DrawConfig, 2> draw_configs{{
					{
						block.object,
						block.ComeFrom == coord,
						block.object->_draw_type == DrawType::Top ? false : true,
						DrawnedType::TopStand
					},
					{
						block.remain,
						block.remain->_draw_type == DrawType::Top,
						false,
						DrawnedType::TopRemain
					}
				}};

				do_draw_configs(coord, block, draw_configs);
			}
			if constexpr (DRAW_T == STANDARD_DRAW)
			{
				const std::array<DrawConfig, 3> draw_configs{{
					{
						block.remain,
						block.remain->_draw_type != DrawType::Top,
						true,
						DrawnedType::StandardRemain
					},
					{
						block.object,
						block.ComeFrom != coord,
						false,
						DrawnedType::StandardMove
					},
					{
						reach(map_)[block.GoTo].object,
						block.GoTo != coord && !draw_box_.isInside(block.GoTo),
						false,
						DrawnedType::StandardGoTo
					}
				}};

				do_draw_configs(coord, block, draw_configs);
			}
		}
	});
}

template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::updateBitmap()
{
	clearDrawnedIndicator();

	checkForDrawRequests();
	clearDrawCountIndicator();

	clearBlocks();
	drawObjects<STANDARD_DRAW>();
	drawObjects<TOP_DRAW>();
	drawBitmapOfRefresh();
	drawBitmapOfCoords();
	drawGravity();

	clearRedrawIndicator(); // Redraw indication can be set on tha timer loop, so it can be only cleared right wafter everything was drawned
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::updateDrawBox(Type::Coord _corner)
{
	Type::rec2D<Type::Coord> draw_box_tmp = draw_box_;
	draw_box_.p1 = _corner;
	draw_box_.p1.limit({0,0}, map_->size());

	draw_box_.p2 = draw_box_.p1 + block_count_;
	draw_box_.p2.limit({0,0}, map_->size());

	draw_box_edges_.p1 = draw_box_.p1 - 1;
	draw_box_edges_.p2 = draw_box_.p2 + 1;

	draw_box_edges_.p1.limit({0,0}, map_->size());
	draw_box_edges_.p2.limit({0,0}, map_->size());

	Type::Coord box_shift = draw_box_tmp.p1 - draw_box_.p1;

	auto iterateOverRowOfBlocks = [&](Type::Coord::base row, Type::Coord::base begin, Type::Coord::base end)
	{
		for (; begin < end; begin++)
		{
			Type::Coord coord{begin,row};
			auto &block = reach(map_)[coord];
			block.redrawn_type_ |= RedrawType::Shift;
			setRedrawOnBlock(coord);
		}
	};
	auto iterateOverColOfBlocks = [&](Type::Coord::base col, Type::Coord::base begin, Type::Coord::base end)
	{
		for (; begin < end; begin++)
		{
			Type::Coord coord{col,begin};
			auto &block = reach(map_)[coord];
			block.redrawn_type_ |= RedrawType::Shift;
			setRedrawOnBlock(coord);
		}
	};


	if (box_shift != 0)
	{
		if (box_shift.x() > 0)
		{
			for (Type::Coord::base col = draw_box_.p1.x(); col < draw_box_tmp.p1.x(); ++col)
			{
				iterateOverColOfBlocks(col, draw_box_.p1.y(), draw_box_.p2.y());
			}
		}
		else if (box_shift.x() < 0)
		{
			for (Type::Coord::base col = draw_box_tmp.p2.x(); col < draw_box_.p2.x(); ++col)
			{
				iterateOverColOfBlocks(col, draw_box_.p1.y(), draw_box_.p2.y());
			}
		}

		if (box_shift.y() > 0)
		{
			for (Type::Coord::base row = draw_box_.p1.y(); row < draw_box_tmp.p1.y(); ++row)
			{
				iterateOverRowOfBlocks(row, draw_box_.p1.x(), draw_box_.p2.x());
			}
		}
		else if (box_shift.y() < 0)
		{
			for (Type::Coord::base row = draw_box_tmp.p2.y(); row < draw_box_.p2.y(); ++row)
			{
				iterateOverRowOfBlocks(row, draw_box_.p1.x(), draw_box_.p2.x());
			}
		}

		shiftBlocks(box_shift);
	}

	draw_box_offset_ = draw_box_.p1 * getBlockSize();
}
template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::drawBitmaps(Type::Pixels _target)
{
	for (auto &layer : layers_)
	{
		if (layer.enable_)
		{
			layer.bitmap_.draw(_target.x(), _target.y());
		}
	}

	Type::rec2D<Type::Pixels> d;
	d.p1 = _target;
	d.p2 = d.p1 + bitmap_size_ - 1;
	al_draw_rectangle(d.p1.x(), d.p1.y(), d.p2.x(), d.p2.y(), KIR5::Color(255, 0, 0), 1.f);
}
template <typename ACTIVE_BLOCK_T>
Type::Coord BlocksDrawer<ACTIVE_BLOCK_T>::getTargetOfCursor(Type::Pixels _cur) const
{
	Type::Coord click_point = _cur / block_size_;
	click_point += draw_box_.p1;
	click_point.limiter({0,0}, (Type::Size)reach(map_) - 1);
	return click_point;
}

template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::setRedrawOnBlockRepeat(Type::Coord coord)
{
	auto &block = reach(map_)[coord];
	if (!(block.redrawn_type_ & RedrawType::Object))
	{
		block.redrawn_type_ |= RedrawType::Object;
		if (block.ComeFrom != coord) setRedrawOnBlockRepeat(block.ComeFrom);
		if (block.GoTo != coord) setRedrawOnBlockRepeat(block.GoTo);
	}
}

template <typename ACTIVE_BLOCK_T>
void BlocksDrawer<ACTIVE_BLOCK_T>::setRedrawOnBlock(Type::Coord coord)
{
	auto &block = reach(map_)[coord];
	block.redrawn_type_ |= RedrawType::Object;
	if (block.ComeFrom != coord) setRedrawOnBlockRepeat(block.ComeFrom);
	if (block.GoTo != coord) setRedrawOnBlockRepeat(block.GoTo);
}

template <typename ACTIVE_BLOCK_T>
Type::rec2D<Type::Pixels> BlocksDrawer<ACTIVE_BLOCK_T>::getObjectRect(const Type::Coord &coord, const ACTIVE_BLOCK_T &block) const
{
	Type::rec2D<Type::Pixels> rec;
	rec.p1 = coord * block_size_;
	rec.p2 = rec.p1 + block_size_;

	const ACTIVE_BLOCK_T::OBJECT_T *object = block.object;
	if (block.ComeFrom != coord && object->isExists)
	{
		const Type::Move &move = object->GetMove();

		if (object->IsMoveRight() && move.x() >= -1.f)
			rec.p1.x() += block_size_.width() * (move.x() + 1);
		else if (object->IsMoveLeft() && move.x() <= 1.f)
			rec.p2.x() += block_size_.width() * (move.x() - 1);

		if (object->IsMoveDown() && move.y() >= -1.f)
			rec.p1.y() += block_size_.height() * (move.y() + 1);
		else if (object->IsMoveUp() && move.y() <= 1.f)
			rec.p2.y() += block_size_.height() * (move.y() - 1);
	}

	object = reach(map_)[block.GoTo].object;
	if (block.GoTo != coord && object->isExists)
	{
		const Type::Move &move = object->GetMove();

		if (object->IsMoveRight() && move.x() <= -1.f)
			rec.p2.x() += block_size_.width() * move.x();
		else if (object->IsMoveLeft() && move.x() >= 1.f)
			rec.p1.x() += block_size_.width() * move.x();

		if (object->IsMoveDown() && move.y() <= -1.f)
			rec.p2.y() += block_size_.height() * move.y();
		else if (object->IsMoveUp() && move.y() >= 1.f)
			rec.p1.y() += block_size_.height() * move.y();
	}
	return rec;
}

template <typename ACTIVE_BLOCK_T>
inline Type::Pixels MoveDrawer<ACTIVE_BLOCK_T>::getBlockSize() const
{
	return blocks_drawer_.getBlockSize();
}
template <typename ACTIVE_BLOCK_T>
inline void MoveDrawer<ACTIVE_BLOCK_T>::setGlobalGravity(bool _global_gravity)
{
	blocks_drawer_.setGlobalGravity(_global_gravity);
}
template <typename ACTIVE_BLOCK_T>
inline void MoveDrawer<ACTIVE_BLOCK_T>::setRedrawOnBlock(Type::Coord coord)
{
	blocks_drawer_.setRedrawOnBlock(coord);
}
template <typename ACTIVE_BLOCK_T>
inline void MoveDrawer<ACTIVE_BLOCK_T>::enableLayer(unsigned int _layer_id, bool _enabled)
{
	blocks_drawer_.enableLayer(_layer_id, _enabled);
}
template <typename ACTIVE_BLOCK_T>
inline bool MoveDrawer<ACTIVE_BLOCK_T>::isLayerEnabled(unsigned int _layer_id) const
{
	return blocks_drawer_.isLayerEnabled(_layer_id);
}
template <typename ACTIVE_BLOCK_T>
inline Type::Pixels MoveDrawer<ACTIVE_BLOCK_T>::getBitmapSize() const
{
	return blocks_drawer_.getBitmapSize();
}

template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::setMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &_map)
{
	map_ = _map;
	blocks_drawer_.setMap(_map);
}

template <typename ACTIVE_BLOCK_T>
Type::Move MoveDrawer<ACTIVE_BLOCK_T>::getCameraSizeInBlocks() const
{
	return camera_count_;
}

template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::updateConfiguration(Type::Size _camera_size, Type::CameraSize _block_size_adjust, int _bufferred_blocks_count)
{
	bufferred_blocks_count_ = _bufferred_blocks_count;

	blocks_drawer_.setBlockSize(_camera_size, _block_size_adjust);

	camera_size_ = _camera_size;
	camera_radius_size_ = _camera_size / 2.f;

	camera_count_ = camera_size_ / (Type::Move)getBlockSize();
	camera_radius_count_ = camera_count_ / 2.f;

	camera_cage_.p1 = camera_radius_count_;
	camera_cage_.p2 = map_->size() - camera_radius_count_;

	blocks_drawer_.updateConfiguration(
		(camera_size_ / Type::Move(getBlockSize())).ceil<Type::Coord::base>() + 1 + bufferred_blocks_count_ * 2
	);
	updateCameraBox(true);
}

template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::updateCameraBox(bool _force_update_draw_box)
{
	camera_move_box_.p1 = camera_center_ - camera_radius_count_;
	camera_move_box_.p2 = camera_center_ + camera_radius_count_;

	camera_box_.p1 = camera_move_box_.p1.floor<Type::Coord::base>();
	camera_box_.p2 = camera_move_box_.p2.ceil<Type::Coord::base>();

	if (_force_update_draw_box || !camera_box_.isSubset(blocks_drawer_.getDrawBox()))
	{
		blocks_drawer_.updateDrawBox(camera_box_.p1 - bufferred_blocks_count_);
	}

	block_offset_ = (camera_move_box_.p1 * getBlockSize()) - blocks_drawer_.getDrawBoxOffset();
}

template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::moveCamera(Type::Move camera)
{
	update_camera_ = camera;
}
template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::updateCamera()
{
	camera_center_ = update_camera_ + 0.5f;
	camera_center_.limiter(camera_cage_.p1, camera_cage_.p2);
}
template <typename ACTIVE_BLOCK_T>
Type::Move MoveDrawer<ACTIVE_BLOCK_T>::getCameraCenter() const
{
	return camera_center_;
}

template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::updateBitmap()
{
	updateCamera();
	updateCameraBox(false);
	blocks_drawer_.updateBitmap();
}
template <typename ACTIVE_BLOCK_T>
void MoveDrawer<ACTIVE_BLOCK_T>::drawBitmaps(Type::Pixels _target)
{
	blocks_drawer_.drawBitmaps(_target - block_offset_);
	al_draw_rectangle(_target.x(), _target.y(), _target.x() + camera_size_.w() - 1, _target.y() + camera_size_.h() - 1, KIR5::Color(255, 255, 0), 1.f);
}

template <typename ACTIVE_BLOCK_T>
Type::Coord MoveDrawer<ACTIVE_BLOCK_T>::getTargetOfCursor(Type::Pixels _cur) const
{
	return blocks_drawer_.getTargetOfCursor(_cur + block_offset_);
}

template <typename ACTIVE_BLOCK_T>
inline Type::Pixels SceneDrawer<ACTIVE_BLOCK_T>::getBlockSize() const
{
	return move_drawer_.getBlockSize();
}
template <typename ACTIVE_BLOCK_T>
inline void SceneDrawer<ACTIVE_BLOCK_T>::setGlobalGravity(bool _global_gravity)
{
	move_drawer_.setGlobalGravity(_global_gravity);
}
template <typename ACTIVE_BLOCK_T>
inline void SceneDrawer<ACTIVE_BLOCK_T>::setRedrawOnBlock(Type::Coord coord)
{
	move_drawer_.setRedrawOnBlock(coord);
}
template <typename ACTIVE_BLOCK_T>
inline void SceneDrawer<ACTIVE_BLOCK_T>::enableLayer(unsigned int _layer_id, bool _enabled)
{
	move_drawer_.enableLayer(_layer_id, _enabled);
}
template <typename ACTIVE_BLOCK_T>
inline bool SceneDrawer<ACTIVE_BLOCK_T>::isLayerEnabled(unsigned int _layer_id) const
{
	return move_drawer_.isLayerEnabled(_layer_id);
}
template <typename ACTIVE_BLOCK_T>
inline void SceneDrawer<ACTIVE_BLOCK_T>::moveCamera(Type::Move camera)
{
	move_drawer_.moveCamera(camera);
}
template <typename ACTIVE_BLOCK_T>
inline Type::Move SceneDrawer<ACTIVE_BLOCK_T>::getCameraCenter() const
{
	return move_drawer_.getCameraCenter();
}
template <typename ACTIVE_BLOCK_T>
inline void SceneDrawer<ACTIVE_BLOCK_T>::updateBitmap()
{
	move_drawer_.updateBitmap();
}
template <typename ACTIVE_BLOCK_T>
inline void SceneDrawer<ACTIVE_BLOCK_T>::setMap(std::shared_ptr<Matrix<ACTIVE_BLOCK_T>> &_map)
{
	move_drawer_.setMap(_map);
}
template <typename ACTIVE_BLOCK_T>
inline Type::Move SceneDrawer<ACTIVE_BLOCK_T>::getCameraSizeInBlocks() const
{
	return move_drawer_.getCameraSizeInBlocks();
}

template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::updateConfiguration(Type::Size _camera_size, Type::CameraSize _block_size_adjust, int _bufferred_blocks_count)
{
	_camera_size = _camera_size;
	//_camera_size = Type::Pixels::max(_camera_size+280, 0);
	move_drawer_.updateConfiguration(_camera_size, _block_size_adjust, _bufferred_blocks_count);
	panel_shift_ = Type::Pixels::apply([](const Type::Pixels::base &bitmap_size, const Type::Pixels::base &_camera_size)
	{
		if (bitmap_size < _camera_size)
		{
			return (_camera_size - bitmap_size) / 2;
		}
		else
		{
			return 0;
		}
	}, move_drawer_.getBitmapSize(), _camera_size);
	//panel_shift_ += 140;
}
template <typename ACTIVE_BLOCK_T>
void SceneDrawer<ACTIVE_BLOCK_T>::drawBitmaps(Type::Pixels _target)
{
	move_drawer_.drawBitmaps(_target + panel_shift_);
}
template <typename ACTIVE_BLOCK_T>
Type::Coord SceneDrawer<ACTIVE_BLOCK_T>::getTargetOfCursor(Type::Pixels _cur) const
{
	return move_drawer_.getTargetOfCursor(_cur - panel_shift_);
}


template <typename ACTIVE_BLOCK_T>
Object::Animator::EntityData BlocksDrawer<ACTIVE_BLOCK_T>::gravity_animator_;

template <typename ACTIVE_BLOCK_T>
Res::Slides BlocksDrawer<ACTIVE_BLOCK_T>::gravity_slides_;

#endif
#pragma once

#include <vector>
#include "Types.h"
#include <KIR/KIR4_console.h>

template<class Data>
class Matrix
{
	private: Type::Size size_ = {0,0};

	protected: std::vector<std::vector<Data>> data;

	public: inline operator Type::Size() const
	{
		return size_;
	}
	public: inline Type::Size size() const
	{
		return size_;
	}
	public: inline Type::Size::base width() const
	{
		return size_.width();
	}
	public: inline Type::Size::base height() const
	{
		return size_.height();
	}
	public: inline Data &operator[](Type::Coord coord)
	{
#ifdef _DEBUG
		if (coord.x() >= size_.width() || coord.y() >= size_.height() || coord.x() < 0 || coord.y() < 0)
			clog << KIR4::LRED << "Coord ERROR!: " << coord << KIR4::eol << KIR4::pp;
#endif // _DEBUG
		return data[coord.x()][coord.y()];
	}
	public: inline const Data &operator[](Type::Coord coord) const
	{
#ifdef _DEBUG
		if (coord.x() >= size_.width() || coord.y() >= size_.height() || coord.x() < 0 || coord.y() < 0)
			clog << KIR4::LRED << "Coord ERROR!: " << coord << KIR4::eol << KIR4::pp;
#endif // _DEBUG
		return data[coord.x()][coord.y()];
	}

	public: Matrix(Type::Size _size)
	{
		if (_size.width() > 0 && _size.height() > 0)
		{
			size_ = _size;

			data.resize(size_.width());
			for (Type::Size::base i = 0; i < size_.width(); i++)
				data[i].resize(size_.height());
		}
	}
	public: Matrix()
	{

	}
	public: Matrix(Matrix &&array2D):
		size_(array2D.size_),
		data(std::move(array2D.data))
	{

	}
	public: virtual ~Matrix()
	{
		data.clear();
		size_ = {0,0};
	}
	public: inline bool Exists() const
	{
		return size_.width() > 0;
	}
	public: inline bool Test(Type::Coord coord) const
	{
		return
			coord.x() < size_.width() &&
			coord.y() < size_.height() &&
			coord.x() >= 0 &&
			coord.y() >= 0;
	}

	public: void resize(Type::Size _size)
	{
		if (_size.width() > 0 && _size.height() > 0)
		{
			size_ = _size;
			data.clear();
			data.resize(size_.width());
			for (Type::Size::base i = 0; i < size_.width(); i++)
				data[i].resize(size_.height());
		}
	}


	public: template<typename F> constexpr void foreach(F &f)
	{
		Type::Coord coord;
		auto x_ = &data.front();
		for (coord.x() = 0; coord.x() < size_.width(); ++coord.x(), ++x_)
		{
			auto y_ = &x_->front();
			for (coord.y() = 0; coord.y() < size_.height(); ++coord.y(), ++y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void foreach(F &f) const
	{
		Type::Coord coord;
		auto x_ = &data.front();
		for (coord.x() = 0; coord.x() < size_.width(); ++coord.x(), ++x_)
		{
			auto y_ = &x_->front();
			for (coord.y() = 0; coord.y() < size_.height(); ++coord.y(), ++y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void reverse_foreach(F &f)
	{
		Type::Coord coord;
		auto x_ = &data.back();
		for (coord.x() = size_.width() - 1; coord.x() >= 0; --coord.x(), --x_)
		{
			auto y_ = &x_->back();
			for (coord.y() = size_.height() - 1; coord.y() >= 0; --coord.y(), --y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void forrange(const Type::Coord &start, const Type::Coord &end, F &f)
	{
		Type::Coord coord;
		auto x_ = &data.front() + start.x();
		for (coord.x() = start.x(); coord.x() < end.x(); ++coord.x(), ++x_)
		{
			auto y_ = &x_->front() + start.y();
			for (coord.y() = start.y(); coord.y() < end.y(); ++coord.y(), ++y_)
			{
				f(coord, *y_);
			}
		}
	}
	public: template<typename F> constexpr void forrange(const Type::rec2D<Type::Coord> &area, F &f)
	{
		forrange(area.p1, area.p2, f);
	}
};

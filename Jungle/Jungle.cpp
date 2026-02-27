#include "pch.h"
#include "jungle.h"


// RANDIT CLASS *************************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max <= min)return -1;

	std::uniform_int_distribution distrib(min, max);

	return(distrib(*twister));
}
float dll::RANDIT::operator()(float min, float max)
{
	if (max <= min)return -1;

	std::uniform_real_distribution<float> distrib(min, max);

	return(distrib(*twister));
}

/////////////////////////////////////////////////////

// EXCEPTION CLASS *************************************

dll::EXCEPTION::EXCEPTION(int which_err)
{
	_err_code = which_err;
}

const char* dll::EXCEPTION::get() const
{
	switch (_err_code)
	{
	case BAG_BAD_ARG:
		return "Bad argument passed to a function !";

	case BAG_BAD_PTR:
		return "Bad pointer passed to a function !";

	case BAG_BAD_INDEX:
		return "Wrong index passed to a function !";

	case BAG_BAD_ERR:
		return "Unknown error occurred !";

	case BAG_NO_ELEMENTS:
		return "Container does not have elements !";
	}

	return "Undescribed error occurred !";
}

/////////////////////////////////////////////////////

// PROTON CLASS *************************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}
dll::PROTON::PROTON(float _sx, float _sy) :start{ _sx, _sy }
{
	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}
dll::PROTON::PROTON(float _sx, float _sy, float _s_width, float _s_height) :start{ _sx, _sy }, _width{ _s_width }, _height{ _s_height }
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}

float dll::PROTON::get_width() const
{
	return _width;
}
float dll::PROTON::get_height() const
{
	return _height;
}

void dll::PROTON::set_width(float new_width)
{
	_width = new_width;
	
	end.x = start.x + _width;

	x_rad = _width / 2.0f;

	center.x = start.x + x_rad;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;

	end.y = start.y + _height;

	y_rad = _height / 2.0f;

	center.y = start.y + y_rad;
}
void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}
void dll::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}

bool dll::PROTON::in_heap() const
{
	return _in_heap;
}

dll::PROTON* dll::PROTON::create(float sx, float sy, float s_width, float s_height)
{
	PROTON* ret{ nullptr };

	ret = new PROTON(sx, sy, s_width, s_height);

	if (ret)ret->_in_heap = true;

	return ret;
}

//////////////////////////////////////////////////////

//TILE CLASS ***************************************

dll::TILE::TILE(tiles _type, float _where_x, float _where_y, dirs _dir) :PROTON(_where_x, _where_y, 50.0f, 50.0f)
{
	type = _type;
	dir = _dir;
}

bool dll::TILE::move(dirs to_where, float gear)
{
	float my_speed = _speed + gear / 5.0f;

	switch (dir)
	{
	case dirs::left:
		start.x -= my_speed;
		set_edges();
		if (end.x - my_speed <= -scr_width)return false;
		break;

	case dirs::right:
		start.x -= my_speed;
		set_edges();
		if (start.x - my_speed >= 2.0f * scr_width)return false;
		break;
	}

	return true;
}

void dll::TILE::Release()
{
	delete this;
}

dll::TILE* dll::TILE::create(tiles type, float where_x, float where_y, dirs dir)
{
	TILE* ret{ nullptr };

	ret = new TILE(type, where_x, where_y, dir);
}

///////////////////////////////////////////////////











// FUNCTIONS *************************************

float dll::Distance(FPOINT first, FPOINT second)
{
	float a = static_cast<float>(pow(abs(second.x - first.x), 2));
	float b = static_cast<float>(pow(abs(second.y - first.y), 2));

	return static_cast<float>(sqrt(a + b));
}
void dll::Sort(BAG<FPOINT>& bag, FPOINT criterion, bool ascending = true)
{
	if (bag.empty())throw EXCEPTION(BAG_NO_ELEMENTS);

	if (ascending)
	{
		bool ok = false;

		while (!ok)
		{
			ok = true;

			for (size_t i = 0; i < bag.size() - 1; ++i)
			{
				if (Distance(bag[i], criterion) > Distance(bag[i + 1], criterion))
				{
					FPOINT temp = bag[i];
					bag[i] = bag[i + 1];
					bag[i + 1] = temp;
					ok = false;
				}
			}
		}
	}
	else
	{
		bool ok = false;

		while (!ok)
		{
			ok = true;

			for (size_t i = 0; i < bag.size() - 1; ++i)
			{
				if (Distance(bag[i], criterion) > Distance(bag[i + 1], criterion))
				{
					FPOINT temp = bag[i];
					bag[i] = bag[i + 1];
					bag[i + 1] = temp;
					ok = false;
				}
			}
		}
	}
}
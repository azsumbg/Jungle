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

//PLATFORM CLASS ***************************************

dll::PLATFORM::PLATFORM(platforms _type, float _where_x, float _where_y, dirs _dir) :PROTON(_where_x, _where_y)
{
	type = _type;
	dir = _dir;

	switch (_type)
	{
	case platforms::flat_platform1:
		new_dims(140.0f, 80.0f);
		break;

	case platforms::flat_platform2:
		new_dims(150.0f, 94.0f);
		break;

	case platforms::high_platform:
		new_dims(110.0f, 90.0f);
		break;
	}
}

bool dll::PLATFORM::move(dirs to_where, float gear)
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

void dll::PLATFORM::Release()
{
	delete this;
}

dll::PLATFORM* dll::PLATFORM::create(platforms type, float where_x, float where_y, dirs dir)
{
	PLATFORM* ret{ nullptr };

	ret = new PLATFORM(type, where_x, where_y, dir);
}

///////////////////////////////////////////////////

// HERO CLASS ************************************

dll::HERO::HERO(float _sx, float _sy) :PROTON(_sx, _sy, 45.0f, 50.0f) {};
	
void dll::HERO::move(float gear)
{
	float my_speed = _speed + gear / 10.0f;

	if (state != RUN)return;

	switch (dir)
	{
	case dirs::left:
		if (start.x - my_speed >= 0)
		{
			start.x -= my_speed;
			set_edges();
		}
		break;

	case dirs::right:
		if (end.x + my_speed <= scr_width)
		{
			start.x += my_speed;
			set_edges();
		}
		break;
	}
}
void dll::HERO::jump(float gear)
{
	float my_speed = _speed + gear / 5.0f;

	if (!in_jump)
	{
		jump_sx = start.x;
		jump_sy = start.y;
		jump_ey = start.y - 100.0f;
		
		state = JUMP_UP;

		if (dir == dirs::left)jump_ex = jump_sx - 20.0f;
		else jump_ex = jump_sx + 20.0f;
	}
	else
	{
		if (state == JUMP_UP)
		{
			switch (dir)
			{
			case dirs::right:
				if (end.x + my_speed >= jump_ex && end.x + my_speed <= scr_width)start.x += my_speed;
				if (start.y - my_speed >= jump_ey)start.y -= my_speed;
				set_edges();
				if (start.y < jump_ey)
				{
					state = JUMP_DOWN;

					jump_ex = jump_sx + 40.0f;
					jump_ey = jump_sy;

					jump_sx = start.x;
					jump_sy = start.y;
				}
				break;


			case dirs::left:
				if (start.x - my_speed >= jump_ex && start.x - my_speed >= 0)start.x -= my_speed;
				if (start.y - my_speed >= jump_ey)start.y -= my_speed;
				set_edges();
				if (start.y < jump_ey)
				{
					state = JUMP_DOWN;
					
					jump_ex = jump_sx - 40.0f;
					jump_ey = jump_sy;

					jump_sx = start.x;
					jump_sy = start.y;

				}
				break;
			}
		}
		else if (state == JUMP_DOWN)
		{
			switch (dir)
			{
			case dirs::right:
				if (end.x + my_speed >= jump_ex && end.x + my_speed <= scr_width)start.x += my_speed;
				if (start.y + my_speed < jump_ey)start.y += my_speed;
				set_edges();
				if (start.y >= jump_ey)
				{
					state = RUN;
					in_jump = false;

					start.y = jump_ey;
					set_edges();
				}
				break;

			case dirs::left:
				if (start.x - my_speed >= jump_ex && start.x - my_speed >= 0)start.x -= my_speed;
				if (start.y + my_speed < jump_ey)start.y += my_speed;
				set_edges();
				if (start.y >= jump_ey)
				{
					state = RUN;
					in_jump = false;

					start.y = jump_ey;
					set_edges();
				}
				break;
			}
		}
	}
}
int dll::HERO::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = 15;
		++frame;
		if (frame > 3)frame = 0;
	}
	return frame;
}

void dll::HERO::Release()
{
	delete this;
}

dll::HERO* dll::HERO::create(float sx, float sy)
{
	HERO* ret{ nullptr };

	ret = new HERO(sx, sy);

	return ret;
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

bool dll::Intersect(FRECT first, FRECT second)
{
	if (!(first.left > second.right || first.right < second.left
		|| first.up>second.down || first.down < second.up))return true;
	return false;
}
bool dll::Intersect(FPOINT first, FPOINT second, float x_rad1, float x_rad2, float y_rad1, float y_rad2)
{
	if (abs(second.x - first.x) <= x_rad1 + x_rad2 && abs(second.y - first.y) <= y_rad1 + y_rad2)return true;
	return false;
}
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
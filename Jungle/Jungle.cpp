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
	}

	return "Undescribed error occurred !";
}

/////////////////////////////////////////////////////
#pragma once

#ifdef JUNGLE_EXPORTS
#define JUNGLE_API __declspec(dllexport)
#else
#define JUNGLE_API __declspec(dllimport)
#endif

#include <random>

constexpr float scr_width{ 900.0f };
constexpr float scr_height{ 600.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 550.0f };

constexpr int BAG_BAD_PTR{ 666 };
constexpr int BAG_BAD_INDEX{ 667 };
constexpr int BAG_BAD_ARG{ 668 };
constexpr int BAG_BAD_ERR{ 669 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};
struct FRECT
{
	float left { 0 };
	float up{ 0 };
	float right{ 0 };
	float down{ 0 };
};

namespace dll
{
	class JUNGLE_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
		float operator()(float min, float max);
	};

	class JUNGLE_API EXCEPTION
	{
	private:
		int _err_code{ 0 };

	public:
		EXCEPTION(int which_err);

		const char* get() const;
	};

	template<typename T> class BAG
	{
	private:
		T* m_ptr{ nullptr };
		size_t size{ 0 };
		size_t next_pos{ 0 };
		bool has_elements{ false };

	public:
		BAG() :size{ 1 }, m_ptr{ reinterpret_cast<T*>(calloc(size, sizeof(T))) } {};
		BAG(size_t capacity) :size{ capacity }, m_ptr{ reinterpret_cast<T*>(calloc(size, sizeof(T))) } {};
		BAG(BAG& other)
		{
			if (!other)throw EXCEPTION(BAG_BAD_ARG);

			size = other.size;
			m_ptr = reinterpret_cast<T*>(calloc(size, sizeof(T));
			
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else if (size > 0)for (size_t i = 0; i < size; ++i)m_ptr[i] = other[i];
		}





	};












}
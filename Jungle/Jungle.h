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
constexpr int BAG_NO_ELEMENTS{ 670 };

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
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);

			size = other.size;
			m_ptr = reinterpret_cast<T*>(calloc(size, sizeof(T));
			
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else if (size > 0)
			{
				for (size_t i = 0; i < size; ++i)m_ptr[i] = other.m_ptr[i];
				has_elements = true;
			}
		}
		BAG(BAG&& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);
			
			size = other.size;
			m_ptr = other.m_ptr;
		
			other.m_ptr = nullptr;
		}

		~BAG()
		{
			free(m_ptr);
		}

		BAG& operator=(BAG& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);
			if (m_ptr == other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);
			
			free(m_ptr);

			size = other.size;
			m_ptr = reinterpret_cast<T*>(calloc(size, sizeof(T));

			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else if (size > 0)
			{
				for (size_t i = 0; i < size; ++i)m_ptr[i] = other.m_ptr[i];
				has_elements = true;
			}
		}
		BAG& operator=(BAG&& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);

			free(m_ptr);
			size = other.size;
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
		}

		T& operator[](size_t index)
		{
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (!has_elements)throw EXCEPTION(BAG_NO_ELEMENTS);

			return m_ptr[i];
		}

		size_t capacity()const
		{
			return size;
		}
		size_t size() const
		{
			return next_pos;
		}
		bool empty() const
		{
			if (has_elements)return false;

			return true;
		}

		void clear()
		{
			free(m_ptr);
			size = 1;
			next_pos = 0;
			has_elements = false;

			m_ptr = reinterpret_cast<T*>(calloc(size, sizeof(T)));
		}

		void push_back(T element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos + 1 <= size)
				{
					m_ptr[next_pos] = element;
					++next_pos;
					has_elements = true;
				}
				else
				{
					++size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						m_ptr[next_pos] = element;
						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos + 1 <= size)
				{
					m_ptr[next_pos] = *element;
					++next_pos;
					has_elements = true;
				}
				else
				{
					++size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						m_ptr[next_pos] = *element;
						++next_pos;
					}
				}
			}
		}

		void push_front(T element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = element;
					has_elements = true;
				}
				else
				{
					if (next_pos + 1 < size)
					{
						for (size_t next_index = next_pos; next_index >= 1; --next_index)
						{
							m_ptr[next_index] = m_ptr[next_index - 1];
						}
						++next_pos;
						*m_ptr = element;
					}
					else
					{
						++size;
						m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * size));
						if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
						else
						{
							for (size_t next_index = next_pos; next_index >= 1; --next_index)
							{
								m_ptr[next_index] = m_ptr[next_index - 1];
							}
						}
						++next_pos;
						*m_ptr = element;
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = *element;
					has_elements = true;
				}
				else
				{
					if (next_pos + 1 < size)
					{
						for (size_t next_index = next_pos; next_index >= 1; --next_index)
						{
							m_ptr[next_index] = m_ptr[next_index - 1];
						}
						++next_pos;
						*m_ptr = *element;
					}
					else
					{
						++size;
						m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * size));
						if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
						else
						{
							for (size_t next_index = next_pos; next_index >= 1; --next_index)
							{
								m_ptr[next_index] = m_ptr[next_index - 1];
							}
						}
						++next_pos;
						*m_ptr = *element;
					}
				}
			}
		}

		void insert(size_t index, T element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			if (next_pos == 0)
			{
				*m_ptr = element;
				has_elements = true;
			}
			else
			{
				if (next_pos + 1 < size)
				{
					for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[index] = element;
					++next_pos;
				}
				else
				{
					++size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
						m_ptr[index] = element;
						++next_pos;
					}
				}
			}
		}
		void insert(size_t index, T* element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			if (next_pos == 0)
			{
				*m_ptr = *element;
				has_elements = true;
			}
			else
			{
				if (next_pos + 1 < size)
				{
					for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[index] = *element;
					++next_pos;
				}
				else
				{
					++size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
						m_ptr[index] = *element;
						++next_pos;
					}
				}
			}
		}

		void remove(size_t index)
		{
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				for (size_t count = index; count <= next_pos - 1; ++count)
				{
					m_ptr[count] = m_ptr[count + 1];
				}
			}

			--next_pos;
		}
	};












}
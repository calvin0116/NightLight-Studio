
#pragma once

#include <vector>
//#include <algorithm>

#include <iostream>

//#define N 100

template<typename T, size_t N>
class LocalVector
{
	T data[N];
	size_t currentSize;

	//std::vector<T> ggvec;

public:

	LocalVector() :
		data{},
		currentSize(0)
	{
	}

	class Iterator
	{
		friend LocalVector;
		LocalVector* vec;
		size_t currentIndex;

	public:
		Iterator(LocalVector* invec) :
			vec(invec),
			currentIndex(0)
		{
		}

		Iterator& operator++()
		{
			++currentIndex;
			return *this;
		}

		Iterator& operator++(int)
		{
			Iterator temp = *this;
			++currentIndex;
			return temp;
		}

		bool operator!=(Iterator& itr)
		{
			return currentIndex != itr.currentIndex;
		}

		T& operator*()
		{
			return vec->at(currentIndex);
		}
	};

	void init()
	{
		currentSize = 0;
		//ggvec = 
	}

	void push_back(T in)
	{
		// [ ] [ ] [ ] [ ]
		//	    | <- current size 2
		//      | <- current index 1
		//          | <- push back size 3 = current size 2 + 1
		//          | <- push back index 2 = current size 2

		if (currentSize >= N)
		{
			// gg
			throw;
		}

		data[currentSize] = in;
		++currentSize;
	}

	size_t size()
	{
		return currentSize;
	}

	T& at(size_t index)
	{
		if (index >= N)
		{
			// gg
			throw;
		}

		return data[index];
	}

	void erase(Iterator itr)
	{
		//size_t size = std::min((size_t)(N - 1), currentSize);
		size_t size = currentSize;
		size = size < (size_t)(N - 1) ? size : (size_t)(N - 1);

		for (size_t i = itr.currentIndex; i < size; ++i)
		{
			data[i] = data[i + 1];
		}

		if (currentSize >= N)
		{
			// gg
			throw;
		}

		--currentSize;
	}

	void erase(size_t index)
	{
		//size_t size = std::min((size_t)(N - 1), currentSize);
		size_t size = currentSize;
		size = size < (size_t)(N - 1) ? size : (size_t)(N - 1);

		for (size_t i = index; i < size; ++i)
		{
			data[i] = data[i + 1];
		}

		if (currentSize >= N)
		{
			// gg
			throw;
		}

		--currentSize;
	}

	void clear()
	{
		//ggvec.clear();
		currentSize = 0;
	}

	Iterator begin()
	{
		return Iterator(this);
	}

	Iterator end()
	{
		Iterator itr(this);
		itr.currentIndex = currentSize;
		return itr;
	}

};


// Test Driver

class TestVector
{
public:

	//typedef std::vector<int> vecT;
	typedef LocalVector<int, 100> vecT;

	int testMyVector()
	{
		std::cout << "Hello World!\n";

		vecT vec;

		vec.push_back(1);
		std::cout << "sz : " << vec.size() << " ";
		std::cout << "at : " << vec.at(0) << std::endl;

		vec.push_back(2);
		std::cout << "sz : " << vec.size() << " ";
		std::cout << "at : " << vec.at(1) << std::endl;

		vec.push_back(3);
		std::cout << "sz : " << vec.size() << " ";
		std::cout << "at : " << vec.at(2) << std::endl;


		std::cout << "itr:" << std::endl;
		auto itr = vec.begin();
		auto itrEnd = vec.end();
		while (itr != itrEnd)
		{

			std::cout << *itr << std::endl;
			++itr;
		}

		std::cout << std::endl;

		itr = vec.begin();
		vec.erase(itr);
		itr = vec.begin();
		itrEnd = vec.end();
		while (itr != itrEnd)
		{

			std::cout << *itr << std::endl;
			++itr;
		}

		vec.clear();

		return 0;
	}

};
